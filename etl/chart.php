<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">

<?php
ini_set('display_errors', 'On');
error_reporting(E_ALL);
include('php/getlocation.php');
?>

<html>

  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>amCharts examples</title>
    <link rel="stylesheet" href="amcharts/style.css" type="text/css">
    <script src="amcharts/amcharts.js" type="text/javascript"></script>
    <script src="amcharts/serial.js" type="text/javascript"></script>
    <!-- scripts for exporting chart as an image -->
    <!-- Exporting to image works on all modern browsers except IE9 (IE10 works fine) -->
    <!-- Note, the exporting will work only if you view the file from web server -->
    <!--[if (!IE) | (gte IE 10)]> -->
    <script src="amcharts/exporting/amexport.js" type="text/javascript"></script>
    <script src="amcharts/exporting/rgbcolor.js" type="text/javascript"></script>
    <script src="amcharts/exporting/canvg.js" type="text/javascript"></script>
    <script src="amcharts/exporting/jspdf.js" type="text/javascript"></script>
    <script src="amcharts/exporting/filesaver.js" type="text/javascript"></script>
    <script src="amcharts/exporting/jspdf.plugin.addimage.js" type="text/javascript"></script>
    <!-- <![endif]-->
    <script type="text/javascript">
      var chart ;
    
      AmCharts.loadJSON = function(url) {
        // create the request
        if (window.XMLHttpRequest) {
          // IE7+, Firefox, Chrome, Opera, Safari
          var request = new XMLHttpRequest();
        } else {
          // code for IE6, IE5
          var request = new ActiveXObject('Microsoft.XMLHTTP');
        }

        // load it
        // the last "false" parameter ensures that our code will wait before the
        // data is loaded
        request.open('GET', url, false);
        request.send();

        // parse adn return the output
        return eval(request.responseText);
      };
      
      // create chart
      AmCharts.ready(function() {

        // load the data
        var chartData = AmCharts.loadJSON('php/chartdata.php');

        // SERIAL CHART    
        chart = new AmCharts.AmSerialChart();
        chart.pathToImages = "amcharts/images/";
        chart.dataProvider = chartData;
        chart.categoryField = "category";
        chart.dataDateFormat = "YYYY-MM";
        
        chart.addListener("dataUpdated", zoomChart);

        var categoryAxis = chart.categoryAxis;
        categoryAxis.parseDates = true; 
        // as our data is date-based, we set parseDates to true
        categoryAxis.minPeriod = "MM"; 
        // our data is daily, so we set minPeriod to DD
        categoryAxis.minorGridEnabled = true;
        categoryAxis.axisColor = "#DADADA";
        categoryAxis.twoLineMode = true;
        categoryAxis.dateFormats = [{
          period: 'MM',
          format: 'MMM'
        }, {
          period: 'YYYY',
          format: 'YYYY'
        }];
        
        // first value axis (on the left)
        var valueAxis1 = new AmCharts.ValueAxis();
        valueAxis1.axisColor = "#FF6600";
        valueAxis1.axisThickness = 2;
        valueAxis1.gridAlpha = 0;
        chart.addValueAxis(valueAxis1);

        // second value axis (on the right)
        var valueAxis2 = new AmCharts.ValueAxis();
        valueAxis2.position = "right"; 
        valueAxis2.axisColor = "#FCD202";
        valueAxis2.gridAlpha = 0;
        valueAxis2.axisThickness = 2;
        chart.addValueAxis(valueAxis2);
        
        // GRAPHS
        var graph1 = new AmCharts.AmGraph();
        graph1.valueAxis = valueAxis1;
        graph1.title = "PCP";
        graph1.valueField = "PCP";
        graph1.bullet = "round";
        graph1.bulletBorderColor = "#FFFFFF";
        graph1.bulletBorderThickness = 2;
        graph1.lineThickness = 2;
        graph1.lineAlpha = 0.5;
        chart.addGraph(graph1);

        var graph2 = new AmCharts.AmGraph();
        graph2.valueAxis = valueAxis1;
        graph2.title = "CDD";
        graph2.valueField = "CDD";
        graph2.bullet = "round";
        graph2.bulletBorderColor = "#FFFFFF";
        graph2.bulletBorderThickness = 2;
        graph2.lineThickness = 2;
        graph2.lineAlpha = 0.5;
        chart.addGraph(graph2);

        var graph3 = new AmCharts.AmGraph();
        graph3.valueAxis = valueAxis1;
        graph3.title = "HDD";
        graph3.valueField = "HDD";
        graph3.bullet = "round";
        graph3.bulletBorderColor = "#FFFFFF";
        graph3.bulletBorderThickness = 2;
        graph3.lineThickness = 2;
        graph3.lineAlpha = 0.5;
        chart.addGraph(graph3);
        
        var graph4 = new AmCharts.AmGraph();
        graph4.valueAxis = valueAxis2;
        graph4.title = "TMIN";
        graph4.valueField = "TMIN";
        graph4.bullet = "round";
        graph4.bulletBorderColor = "#FFFFFF";
        graph4.bulletBorderThickness = 2;
        graph4.lineThickness = 2;
        graph4.lineAlpha = 0.5;
        chart.addGraph(graph4);
        
        var graph5 = new AmCharts.AmGraph();
        graph5.valueAxis = valueAxis2;
        graph5.title = "TMAX";
        graph5.valueField = "TMAX";
        graph5.bullet = "round";
        graph5.bulletBorderColor = "#FFFFFF";
        graph5.bulletBorderThickness = 2;
        graph5.lineThickness = 2;
        graph5.lineAlpha = 0.5;
        chart.addGraph(graph5);
        
        var graph6 = new AmCharts.AmGraph();
        graph6.valueAxis = valueAxis2;
        graph6.title = "TAVG";
        graph6.valueField = "TAVG";
        graph6.bullet = "round";
        graph6.bulletBorderColor = "#FFFFFF";
        graph6.bulletBorderThickness = 2;
        graph6.lineThickness = 2;
        graph6.lineAlpha = 0.5;
        chart.addGraph(graph6);

        // CURSOR
        var chartCursor = new AmCharts.ChartCursor();
        chartCursor.cursorAlpha = 0.1;
        chartCursor.fullWidth = true;
        chart.addChartCursor(chartCursor);

        // SCROLLBAR
        var chartScrollbar = new AmCharts.ChartScrollbar();
        chartScrollbar.graph = graph1;
        chartScrollbar.scrollbarHeight = 30;
        chart.addChartScrollbar(chartScrollbar);

        // LEGEND
        var legend = new AmCharts.AmLegend();
        legend.marginLeft = 110;
        legend.useGraphSettings = true;
        chart.addLegend(legend);

        chart.amExport = {
          top: 0,
          right: 0,
          buttonColor: '#EFEFEF',
          buttonRollOverColor:'#DDDDDD',
          exportPDF:true,
          enabled:true,
          libs:{
            path:"amcharts/libs/"
          }
        }
        
        // CATEGORY AXIS 
        chart.categoryAxis.parseDates = true;

        // WRITE
        chart.write("chartdiv");
      });
      function zoomChart(){
        chart.zoomToIndexes(10, 20);
        //chart.zoomToIndexes(chart.dataProvider.length - 40, chart.dataProvider.length - 1);
      }
      function loadData(str) {
        if (str == "") {
          return;
        } else { 
          if (window.XMLHttpRequest) {
              // code for IE7+, Firefox, Chrome, Opera, Safari
              xmlhttp = new XMLHttpRequest();
          } else {
              // code for IE6, IE5
              xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
          }
          xmlhttp.onreadystatechange = function() {
              if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                  chart.dataProvider = eval(xmlhttp.responseText);
                  chart.validateData();
              }
          }
          xmlhttp.open("GET","php/chartdata.php?code="+str,true);
          xmlhttp.send();
        }
      }
    </script>
  </head>

  <body>
    <div>
      <h2> ASQL ETL Chart </h2>
      <select name="location" onchange="loadData(this.value)">
        <?php
          echo $companydata;
        ?>
      </select>
      
      <br/>
      <select name="location" onchange="">
        	<option value="monthly">Monthly</option>
          <option value="yearly">Yearly</option>
          <option value="quarterly">Quarterly</option>
      </select>
      <br/><br/>
    </div>
    
    <div id="chartdiv" style="width: 100%; height: 400px;"></div>
   
  </body>

</html>