### Testing 

mysql -u $MYSQL_USER -h 127.0.0.1 -P -p$MYSQL_PASSWORD


mysql -u $MYSQL_USER$ -p$MYSQL_PASSWORD -h 127.0.0.1 -P 3306 -e "SHOW SLAVE STATUS\G"

mysql -u admin -proot -h 127.0.0.1 -P 6032


colima start --arch x86_64 --vm-type=qemu --cpu 4 --memory 6