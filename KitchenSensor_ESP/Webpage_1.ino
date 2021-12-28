
/*
	HTML Page, test here: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_intro
	Convert here: http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
*/




void Prep_webpage1()
{
  
  String timeStr = timeClient.getFormattedTime();

    // !!! ELements that don't change !!!
  String page_head ="<!DOCTYPE html><html><head><style>td,th{ border: 1px solid #dddddd; text-align: left; padding: 8px;} tr:nth-child(even){background-color: #dddddd;}</style></head>";
  String body_start = "<body>";
  String body_table_start = String("<h2>") + DeviceHostName + String("</h2><h4>Updated: ") + timeStr + String("</h4><table>");
  String body_table_row00 ="<tr><th>#</th><th>Description</th><th>Value</th><th>Unit</th></tr>";
  String body_end = "</table></body></html>";
  
  int mins_left, secs_left;
  long TMR_time_elapsed = millis()/1000 - TMR_start_time;
  int sp_secs = sp_mins * 60;
  switch(TMR_Status)
  {
    case 0: 
        mins_left = 0;
        secs_left = 0;
        break;

    case 1: 
        mins_left = ((sp_secs) - TMR_time_elapsed) / 60;    
        secs_left = ((sp_secs) - TMR_time_elapsed) - (TMR_mins_left_display*60);
        break;

    case 2:
    case 3: 
        mins_left = -1 * (TMR_time_elapsed - sp_secs) / 60; 
        secs_left = (TMR_time_elapsed - sp_secs) - (TMR_mins_left_display*60);
        break;
  }

  // Start chaning here onwards.....
  String body_table_row01 =String("<tr><td>01</td><td>Motion State</td><td>")     + String(MQTT_PIR_last_ON_msg_state)  + String("</td><td>-</td></tr>");
  String body_table_row02 =String("<tr><td>03</td><td>Timer: Setpoint</td><td>")  + String(sp_mins)             + String("</td><td>mins</td></tr>");
  String body_table_row03 =String("<tr><td>02</td><td>Timer: Time left</td><td>") + String(mins_left) + ":" +  String(secs_left)  + String("</td><td>mm:ss</td></tr>");  
  //String body_table_row04 =String("<tr><td>04</td><td>City Humidity</td><td>")           + String(City_Humidity)      + String("</td><td>%</td></tr>");
  //String body_table_row05 =String("<tr><td>05</td><td>DHT11 Temperature</td><td>")       + String(DHT_temperature)    + String("</td><td>&#8451;</td></tr>");
  
  

  webpage1 = page_head +
  			 body_start +
  			 body_table_start +
  			 body_table_row00 +
  			 body_table_row01 +
  			 body_table_row02 +
         body_table_row03 +
         //body_table_row04 +
         //body_table_row05 +
  			 body_end; 
         
}


/*

Source webpage:

<!DOCTYPE html>
<html>
<head>
<style>
td,th{
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}
tr:nth-child(even){
  background-color: #dddddd;
}
</style>
</head>
<body>

<h2>Device Name</h2>
<h4>Updated: Time</h4>
<table>
  <tr>
    <th>#</th>
    <th>Description</th>
    <th>Value</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>1</td>
    <td>Voltage</td>
    <td>231.1</td>
    <td>V</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Current</td>
    <td>0.21</td>
    <td>A</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Power</td>
    <td>3212</td>
    <td>W</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Energy</td>
    <td>241</td>
    <td>kWh</td>
  </tr>
  <tr>
    <td>5</td>
    <td>Frequeny</td>
    <td>50</td>
    <td>hz</td>
  </tr>
  
</table>

</body>
</html>





*/
