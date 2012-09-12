<?php

// Microsoft SQL Server specific install functions

class DatabaseTasks_sqlsrv extends DatabaseTasks {

  protected $pdoDriver = 'sqlsrv';

  public function name() {
    return 'Microsoft SQL Server';
  }

  public function __construct() {
    // Core tasks are using a table without primary key, they need to be
    // completely rewritten.
    $this->tasks = array();

    // Create the user-defined functions we need to be Drupal friendly.
    $this->tasks[] = array(
      'function' => 'initializeDatabase',
      'arguments' => array(),
    );
  }

  /**
   * Make SQL Server Drupal friendly.
   */
  function initializeDatabase() {
    $database = Database::getConnection();
    $database->bypassQueryPreprocess = TRUE;
    try {
      // SUBSTRING() function.
      $database->query(<<< EOF
IF OBJECT_ID (N'SUBSTRING', N'FN') IS NOT NULL BEGIN DROP FUNCTION SUBSTRING END
EOF
      );
      $database->query(<<< EOF
CREATE FUNCTION [SUBSTRING](@op1 nvarchar(max), @op2 sql_variant, @op3 sql_variant) RETURNS nvarchar(max) AS
BEGIN
  RETURN CAST(SUBSTRING(CAST(@op1 AS nvarchar(max)), CAST(@op2 AS int), CAST(@op3 AS int)) AS nvarchar(max))
END
EOF
      );

      // SUBSTRING_INDEX() function.
      $database->query(<<< EOF
IF OBJECT_ID (N'SUBSTRING_INDEX', N'FN') IS NOT NULL BEGIN DROP FUNCTION SUBSTRING_INDEX END
EOF
      );
      $database->query(<<< EOF
CREATE FUNCTION [SUBSTRING_INDEX](@string varchar(8000), @delimiter char(1), @count int) RETURNS varchar(8000) AS
BEGIN
  DECLARE @result varchar(8000)
  DECLARE @end int
  DECLARE @part int
  SET @end = 0
  SET @part = 0
  WHILE (@part < @count)
  BEGIN
    SET @end = CHARINDEX(@delimiter, @string, @end + 1)
    IF (@end = 0)
    BEGIN
      SET @end = LEN(@string) + 1
      BREAK
    END
    SET @part = @part + 1
  END
  SET @result = SUBSTRING(@string, 1, @end - 1)
  RETURN @result
END
EOF
      );

      // GREATEST() function.
      $database->query(<<< EOF
IF OBJECT_ID (N'GREATEST', N'FN') IS NOT NULL BEGIN DROP FUNCTION GREATEST END
EOF
      );
      $database->query(<<< EOF
CREATE FUNCTION [GREATEST](@op1 real, @op2 real) RETURNS real AS
BEGIN
  DECLARE @result real
  SET @result = CASE WHEN @op1 >= @op2 THEN @op1 ELSE @op2 END
  RETURN @result
END
EOF
      );

      // CONCAT() function.
      $database->query(<<< EOF
IF OBJECT_ID (N'CONCAT', N'FN') IS NOT NULL BEGIN DROP FUNCTION CONCAT END
EOF
      );
      $database->query(<<< EOF
CREATE FUNCTION [CONCAT](@op1 sql_variant, @op2 sql_variant) RETURNS nvarchar(4000) AS
BEGIN
  DECLARE @result nvarchar(4000)
  SET @result = CAST(@op1 AS nvarchar(4000)) + CAST(@op2 AS nvarchar(4000))
  RETURN @result
END
EOF
      );

      // IF(expr1, expr2, expr3) function.
      $database->query(<<< EOF
IF OBJECT_ID (N'IF', N'FN') IS NOT NULL BEGIN DROP FUNCTION [IF] END
EOF
      );
      $database->query(<<< EOF
CREATE FUNCTION [IF](@expr1 sql_variant, @expr2 sql_variant, @expr3 sql_variant) RETURNS sql_variant AS
BEGIN
  DECLARE @result sql_variant
  SET @result = CASE WHEN CAST(@expr1 AS int) != 0 THEN @expr2 ELSE @expr3 END
  RETURN @result
END
EOF
      );
      $database->bypassQueryPreprocess = FALSE;
    }
    catch (Exception $e) {
      $this->fail(st('Drupal could not be correctly setup with the existing database. Revise any errors.'));
    }
  }
}
