<?php
/**
 * @file
 *   Implementation of abstract class ChartsGraphsCanvas for Flot library.
 *
 */

/**
 * Implementation of abstract class ChartsGraphsCanvas for Flot library.
 */
class ChartsGraphsFlot extends ChartsGraphsCanvas {

  /*
   * Set some defaults
   */
  var $width = 450;
  var $height = 200;
  var $title = '';



  /**
   * Function that renders data.
   */
  public function get_chart() {
    $x_labels = $this->x_labels;
    // TODO: Add the title of the X Axis to the class
    $series = $this->series;


    $datas = array();
    $ticks = array();
    $table = array();

    /*
     *  Ok, this table is weird. It is also extremely helpful to debug the
     *  data. So, we leave it, and dont print it out at the end.
     *  Its a copy from what we found in the Bluff charts_graphs implementation
     */

    $table[] = sprintf(<<<TABLE
      <table id="%s" class="bluff-data-table">
        <caption>%s</caption>
        <thead>
          <tr>
            <th scope="col"></th>
TABLE
,
        'flot-data-table',
        $this->title
    );
    $serie_keys = array_keys($series);

    foreach ($serie_keys as $col) {
      $table[] = sprintf("<th scope='col'>%s</th>\n", $col);
    }

    $table[] = "</tr></thead><tbody>\n";

    $row = 0; // These are rows in the resultset, each row value is on the y axis.
    foreach ($x_labels as $key => $label) {
      $table[] = "<tr>\n";
      $cols = array($label);

      foreach ($serie_keys as $serie_key) {
        $cols[$serie_key] = array_shift($series[$serie_key]);
      }

      $thiscols = array_shift($cols);
      $table[] = sprintf("<th scope='row'>%s</th>\n", $thiscols);
      $ticklabel = $thiscols;

      $column = 0;  // $column are columns in the resultset. each column is a line (or bar) in the chart.
      foreach ($cols as $label => $col) {
        $table[] = sprintf("<td>%s</td>\n", (string) $col);
        if ($this->type != 'pie') {
          $datas[$column]['label'] = $label;
          $datas[$column]['order'] = $column + 1;
          if ($this->x_type == 'date') {
            $datas[$column]['data'][$row] = array($ticklabel * 1000, (string) $col);
          }
          elseif ($this->x_type == 'number') {
            $datas[$column]['data'][$row] = array($ticklabel, (string) $col);
          }
          else {
            $datas[$column]['data'][$row] = array($row, (string) $col);
          }
        }
        else {
          $datas[$row]['data']= $col;
          $datas[$row]['label']= $ticklabel;
        }
        $column++;
      }

      $ticks[$row] = array($row, $ticklabel);


      $row++;
      $table[] = "</tr>\n";
    }

    $table[] = "</tbody></table>\n";

    /*
     *  Based on the type of graph, get a default style
     *  and set some additional options if we need them
     */
    switch ($this->type) {
      case 'point':
        $options = new flotStylePoint();
        break;
      case 'line':
        $options = new flotStyleLine();
        $options->series->lines->fill = FALSE;
        break;
      case 'pointline':
        $options = new flotStyleLine();
        $options->series->lines->fill = FALSE;
        $options->series->points->show = TRUE;
        break;
      case 'area':
        $options = new flotStyleLine();
        break;
      case 'linestack':
        $options = new flotStyleLine();
        break;
      case 'bar':
        $options = new flotStyleBar();
        $options->series->bars->barWidth = 0.8;
        $options->series->bars->align = "center";
        break;
      case 'sidebar':
        $options = new flotStyleBar();
        $options->series->bars->horizontal = TRUE;
        $options->series->bars->barWidth = 0.8;
        $options->series->bars->align = "center";
        $datas = $this->reversedata($datas);
        break;
      case 'barstack':
        $options = new flotStyleBar();
        $options->series->bars->barWidth = 0.8;
        $options->series->bars->align = "center";
        break;
      case 'sidebarstack':
        $options = new flotStyleBar();
        $options->series->bars->horizontal = TRUE;
        $options->series->bars->barWidth = 0.8;
        $options->series->bars->align = "center";
        $datas = $this->reversedata($datas);
        break;
      case 'pie':
        $options = new flotStylePie();
        break;
      default:
        $options = new flotStyleLine();

    }


    /*
     *  Massage our data arrays into a propor flotData
     */
    $datasets = array();
    foreach ($datas as $data) {
      $myset = new flotData($data['data']);
      $myset->label = $data['label'];
      if (($this->type == 'bar') && count($datas > 1)) {
        // This loads the flot orderBars plugin and only works on a vertical bar chart with more than 1 dataset
        $myset->bars->order = $data['order'];
        $myset->bars->show = TRUE;
        $myset->bars->barWidth = (0.8/(count($datas)));
        $myset->bars->autoScale = TRUE;
        $myset->bars->fillOpacity = 1;
        $myset->bars->align = "center";
      }


      if (($this->type == 'linestack') || ($this->type == 'barstack') || ($this->type == 'sidebarstack')) {
        $myset->stack = 'stack';
      }
      $datasets[] = $myset;
    }

    /*
     *  Set which axis to plot the date on
     */
    if (($this->type == 'sidebar') || ($this->type == 'sidebarstack')) {
      if ((isset($ticks) && ($this->x_type != 'date'))) {
        $options->axis_ticks('yaxis', $ticks);
      }
      else {
        drupal_set_message(t("Using dates on the Y-Axis has not been very succesful to date. This probably won't work at all."), 'warning');
      }
      $dateaxis = 'yaxis';
    }
    else {
      if ((isset($ticks) && ($this->x_type != 'date'))) {
        $options->axis_ticks('xaxis', $ticks);
      }
      $options->yaxis->min = $this->y_min;
      $options->yaxis->max = $this->y_max;
      $options->yaxis->tickSize = $this->y_step;
      $dateaxis = 'xaxis';
    }


      /*
       *  Date graphs are different for Flot.
       *  We made ChartsGraphs understand
       */
      if ($this->x_type == 'date') {
        $options->$dateaxis->mode = "time";
        $options->$dateaxis->timeformat = "%d/%m/%y";
//        $options->$dateaxis->minTickSize = array("1" => "hour");
        if (($this->type == 'bar') || ($this->type == 'sidebar')) {
            drupal_set_message(t("Using dates and barcharts is not supported."), 'warning');
        }
      }

      $width = is_numeric($this->width) ? $this->width . 'px' : $this->width;
      $height = is_numeric($this->height) ? $this->height . 'px' : $this->width;

      $options->xaxis->autoscaleMargin = .10;

      $options->legend->show = $this->showlegend;
      $options->resize = TRUE;
      $options->grid->tickColor = '#eee';

    $element = array(
        '#theme' => 'flot_graph',
        '#element' => array('style' => "width:" . $width . ";height:" . $height . ";"),
        '#data' => $datasets,
        '#options' => $options,
        '#zoom' => $this->zoom,
        '#legend' => $this->showlegend,
    );

    /*
     *  To print a table that shows which raw data goes into flot
     *  very helpful when debugging
     */
//    print(implode('', $table));

    return $element;

  }

  /**
   *  Reverses the order of each datapoint
   *  Used to show barcharts horizontally, instead of vertically
   */
  public function reversedata($datasets) {
    //reverse data
    $reverse = array();
    foreach ($datasets as $key => $value) {
      $reverse[$key] = $value;
      foreach ($value['data'] as $datakey => $data) {
        $tmp = $data[0];
        $data[0] = $data[1];
        $data[1] = $tmp;
        $reverse[$key]['data'][$datakey] = $data;
      }
    }
    return $reverse;
  }

}
