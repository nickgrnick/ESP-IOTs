const char css_style[] PROGMEM = R"rawliteral(
  <style>
	body { 
      margin: 5px; 
      font-family: Arial,Helvetica,sans-serif; 
      color: blue; 
      background-color: #e6f2ff;
    }
    #main-container {
      text-align: center; 
    }
    .center { 
      margin-left: auto; 
      margin-right: auto; 
    }
    .centered { 
      position: fixed; 
      top: 50%; 
      left: 50%; 
      transform: translate(-50%, -50%); 
    } 
    .big-font { 
      font-size: 1.7em; 
      font-weight: bold; 
    }
    h1 { 
      margin-top: 0.3em; 
      margin-bottom: 0.3em; 
      font-size: 1.9em; 
    } 
    h2 { 
      margin-top: 0.3em; 
      margin-bottom: 0.3em; 
      font-size: 1.2em; 
    } 
    h3 { 
      margin-top: 0.3em; 
      margin-bottom: 0.3em; 
      font-size: 1.0em; 
    } 
    h4 { 
      margin-top: 0.3em; 
      margin-bottom: 0.3em; 
      font-size: 0.8em; 
    } 
    #thermo-clock { 
      font-size: 1.2em; 
      font-weight: bold; 
    } 
    .on { 
      color: red; 
    } 
    .off { 
      color: limegreen; 
    } 
    .topnav { 
      overflow: hidden; 
      background-color: #99ccff; 
      border-bottom-left-radius: 5px; 
      border-bottom-right-radius: 5px; 
    } 
    .topnav a { 
      float: left; 
      color: blue; 
      text-align: center; 
      padding: 0.3em 0.5em; 
      text-decoration: none; 
      font-size: 1.0em; 
    } 
    .topnav a:hover { 
      background-color: deepskyblue; 
      color: white; 
    } 
    .topnav a.active { 
      background-color: lightblue; 
      color:blue; 
    } 
    table tr, td { 
      padding: 0.2em 0.5em 0.2em 0.5em; 
      font-size: 1.0em; 
      font-family: Arial,Helvetica,sans-serif; 
    }
    col:first-child { 
      background: #66ffff; 
    } 
    col:nth-child(7) { 
      background: #CCC; 
    } 
    col:nth-child(8) { 
      background: #CCC; 
    } 
    tr:first-child { 
      background: #66ffff; 
    } 
    .medium { 
      font-size: 1.1em; 
      padding: 0; 
      margin: 0; 
    } 
    .ps { 
      font-size: 0.7em; 
      padding:0; 
      margin: 0 
    } 
    .outer { 
      width: 100%; 
      display: flex; 
      justify-content: left; 
      overflow-x: auto; 
    } 
    footer { 
      padding: 0.1em; 
      background-color: #99ccff; 
      font-size: 1.1em; 
      border-radius: 5px; 
    } 
  
    #wifi-perc { 
      padding-left: 5px; 
      display: inline-block; 
    } 
    .wifi-sign { 
      padding: 3px; 
      position: relative; 
      top: 1em; 
      left: 0.36em; 
      display: inline-block; 
    } 
    .wifi-sign, .wifi-sign:before { 
      border: 9px double transparent; 
      border-top-color: currentColor; 
      border-radius: 50%; 
      display: inline-block; 
    } 
    .wifi-sign:before { 
      content: ''; 
      width: 0; 
      height: 0; 
      display: inline-block; 
    } 
    .wifi-div { 
      float: left; 
      display: inline-block; 
    } 
  
    .copyright { 
      display: inline-block; 
    } 
  
    input[type=submit] { 
      font-size: large; 
      padding: 10px 20px; 
      background: #3498DB;
      color: white;
      border: 1 solid black; 
      -webkit-border-radius: 5px; 
      border-radius: 5px; 
    } 
    .help { 
      text-align: left; 
      font-size: 1.0em; 
    } 

    .label-edit { 
      text-align: left; 
      margin-top: 10px; 
      margin-left: 10px; 
    } 
    .cont-edit { 
      text-align: left; 
      margin-top: 5px; 
      margin-left: 15px; 
    } 
    .cont-edit input { 
      max-width: 500px; 
      height: auto; 
      font-size: 1.0em; 
    } 
    .cont-edit select { 
      max-width: 500px; 
      height: auto; 
      font-size: 1.0em; 
    } 

    .w-30 { 
      width: 30% 
    } 
    .w-40 { 
      width: 40% 
    } 
    .w-50 { 
      width: 50% 
    } 
    .w-60 { 
      width: 60% 
    } 
    .w-70 {
      width: 70% 
    } 
    .w-80 { 
      width: 80% 
    } 
    .w-90 { 
      width: 90% 
    } 
    .w-95 { 
      width: 95% 
    } 
    .w-100 { 
      width: 100% 
    } 

    .circle { 
      border-radius: 50%; 
      width: 5em; 
      height: 2.7em; 
      text-align: center; 
      font-size: 4em; 
      display: inline-flex; 
      justify-content: center; 
      align-items: center; 
    } 

    @-moz-keyframes heat { 
      0% { 
        opacity: 1.0; 
      } 
      50% { 
        opacity: 0.3; 
      } 
      100% { 
        opacity: 1.0; 
      } 
    }
    @-webkit-keyframes heat { 
      0% { 
        opacity: 1.0; 
      } 
      50% { 
        opacity: 0.3; 
      } 
      100% { 
        opacity: 1.0; 
      } 
    }
    @keyframes heat { 
      0% { 
        opacity: 1.0; 
      } 
      50% { 
        opacity: 0.3; 
      } 
      100% { 
        opacity: 1.0; 
      } 
    }

    .heat-disab { 
      border: 0.11em solid blue; 
      padding: 0.2em; 
      color: blue; 
    }
    .heat-off { 
      border: 0.11em solid blue; 
      padding: 0.2em; 
      color: blue; 
      background: #99ff33; 
    }
    .heat-on { 
      border: 0.11em solid red; 
      padding: 0.2em; 
      color: red; 
      background: orange; 

      -webkit-animation-name: heat; 
      -webkit-animation-duration: 2s; 
      -webkit-animation-timing-function: linear; 
      -webkit-animation-iteration-count: infinite; 
      
      -moz-animation-name: heat; 
      -moz-animation-duration: 2s; 
      -moz-animation-timing-function: linear; 
      -moz-animation-iteration-count: infinite; 

      animation-name: heat; 
      animation-duration: 2s; 
      animation-timing-function: linear; 
      animation-iteration-count: infinite; 
    }
    /* iframe */
    #frame-wrap {
      width: 100%;
      height: 100vh;
      padding: 0;
      overflow: auto;
    }
    #the-frame {
      width: 100%;
      height: 100vh;
      border: 0px;
    }
    #the-frame {
      zoom: 1;
      -moz-transform: scale(1);
      -moz-transform-origin: 0 0;
      -o-transform: scale(1);
      -o-transform-origin: 0 0;
      -webkit-transform: scale(1);
      -webkit-transform-origin: 0 0;
    }
    @media screen and (-webkit-min-device-pixel-ratio:0) {
      #the-frame {
        zoom: 1;
      }
    }
  /* end iframe */
  /* menu */
    .dropbtn-menu {
      background-color: #3498DB;
      color: white;
      padding: 7px;
      font-size: 1.1em;
      border: none;
      border-top-left-radius: 5px;
      border-top-right-radius: 5px;
      cursor: pointer;
    }

    .dropbtn-menu:hover, .dropbtn-menu:focus {
      background-color: #2980B9;
    }

    .devices-menu {
      position: relative;
      border-bottom: 3px solid #3498DB;
    }

    .devices-menu-content {
      display: none;
      position: absolute;
      background-color: #f1f1f1;
      min-width: 160px;
      overflow: auto;
      box-shadow: 0px 8px 16px 0px rgba(0, 0, 0, 0.2);
      z-index: 1;
    }

    .devices-menu-content a {
      color: black;
      padding: 12px 16px;
      text-decoration: none;
      display: block;
    }

    .devices-menu-content a:hover {
      background-color: #ddd;
    }

    .toggle-show-menu {
      display: block;
    }
  /* end menu */
  </style>
)rawliteral";

const char help_html[] PROGMEM = R"rawliteral(
  <div class="help">
    <u><b>General</b></u>
    <p>The device starts, for first time, as "Access Point" with SSID name 'thermostat-thermostat' and password 'changemepls'. These values are loaded from settings:
    <br/> - the name of access-point (SSID) [device type] -> thermostat-thermostat <- [the server name]
    <br/> - the SSID password is the password mentionated in settings and used also with the username to login on device</p>
    <p>When device started as "Access Point" this will have static IP: "192.168.4.1" and can be accesed from browser, after connect to the "Access Point", like: "http://192.168.4.1/"</p>
    <p>After you set an wifi SSID and password, of youre LAN network, the device, will try the new credentials. If it succed the new credentials the device can be accesed in LAN with LAN DHCP IP.</p>
    <p>This code aims to make your device work even when there is no internet connection. In this situation, the device will actually turn on only after a date and current time have been set for it to be able to function according to the schedules made. In order not to have problems with maintaining the date and time of the device, it would be good for it to be protected by using a battery for power or an RTC module (need suplimentary implementations). After restarting the device without internet access, it needs to set the date and time. It is enough to access it as an Access Point and it will ask you to enter the current date and time.</p>

    <u><b>Status module</b></u>
    <p>Displays the current ambient temperature and humidity, the heater temperature and current working mode. Also it figurate with some colors animations if the device is heateng or no.</p>

    <u><b>Graph module</b></u>
    <p>Displays the the evolution of temperatures (ambient, target and heater) and humidity for last 48 hours, readed every 10 min.</p>
    <p>This module works (for this version) only if device can acces the internet. It use google online library "https://www.gstatic.com/charts/loader.js".</p>

    <u><b>Schedule module</b></u>
    <p>Determines the heating temperature for each day of the week and up to 4 heating periods in a day. To set the heating to come on at 06:00 and off at 09:00 with a temperature of 20° enter 20 then the required start/end times. Repeat for each day of the week and heating period within the day for the required heat profile.</p>

    <u><b>Setup module</b></u>
    <p>This module set some important device parameters like:</p>
    <p><i>Thermostat name</i> - this name will apear as name of device in every device page.</p>
    <p><i>Server name</i> - this name can be used in some network configuration to acces the device with link like "http://[server-name].local".</p>
    <p><i>Username an Password</i> - this is important for device security and need to be used for validating periodicaly. The initial password is 'changemepls' and must be changed and memorized.</p>
    <p><i>Thermostat power</i> - will be used in the future versions to make graphicaly representation of the history power consumption.</p>
    <p><i>Timezone</i> - is used for keep the wright hour of the device, when the device time is sincronized with online time server "time.nist.gov".</p>
    <p><i>Hysteresis</i> - this setting is used to prevent unwanted rapid switching on/off of the heating as the room temperature nears or falls towards the set/target-point temperature. A normal setting is 0.5°C, the exact value depends on the environmental characteristics, for example, where the thermostat is located and how fast a room heats or cools.</p>
    <p><i>Frost protection temperature</i> - this setting is used to protect from low temperatures and pipe freezing in cold conditions. It helps prevent low temperature damage by turning on the heating until the risk of freezing has been prevented.</p>
    <p><i>Early start duration</i> - if greater than 0, begins heating earlier than scheduled so that the scheduled temperature is reached by the set time.</p>
    <p><i>Heating manual override</i> - switch the heating on and control to the desired temperature, switched-off when the next timed period begins.</p>
    <p><i>Heating manual override temperature</i> - used to set the desired manual override temperature.</p>
    <p><i>Max ambient temperature</i> - used to set the maximum ambient temperature desired.</p>
    <p><i>Max heater temperature</i> - used to set the maximum heater temperature desired. This temperature is very important to protect heater to overheating. The overheat sensor must be in the proximity of the heater.</p>
    <p><i>Temperature units</i> - can be (C)elsius or (F)ahrenheit. Until now was tested only for Celsius and if this value will change to (F)ahrenheit, may need to change procedure for reading temperatures from sensors.</p>
    
  </div>
)rawliteral";

const char status_html[] PROGMEM = R"rawliteral(
  <table class="center w-90">
    <tr>
      <td>Humid.</td>
      <td>Target</td>
      <td>Heater</td>
      <td>Mode</td>
    </tr>
    <tr>
      <td><div id="humid-ambient">...</div></td>
      <td><div id="temp-target">...</div></td>
      <td><div id="temp-heater">...</div></td>
      <td><div id="work-mode">...</div></td>
    </tr>
  </table>
  <br/>
  <div id="thermo-clock">...</div>
)rawliteral";

const char info_html[] PROGMEM = R"rawliteral(
    <h2>Thermo-status</h2><br/>
    <div class="circle" id="temp-ambient">...</div>
    <br/>
    <br/>
    [status]
    <br/>
)rawliteral";

const char setup_html[] PROGMEM = R"rawliteral(
  <h2>System Setup</h2>

  <a href="javascript: window.location.assign('/ssid')">Change SSID credentials</a>

  <form action="/post-setup" method="post">
    <input type="hidden" name="token" value="[token]">

    <div class="label-edit">
      <label for="dev-name">Thermostat name</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-90" name="dev-name" value="[dev-name]">
    </div>

    <div class="label-edit">
      <label for="srv-name">Server name</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-90" name="srv-name" value="[srv-name]">
    </div>

    <div class="label-edit">
      <label for="login-user">Username</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-90" name="login-user" value="[login-user]">
    </div>

    <div class="label-edit">
      <label for="login-pass">Password</label>
    </div>
    <div class="cont-edit">  
      <input type="password" class="w-90" name="login-pass" value="[login-pass]">
    </div>

    <div class="label-edit">
      <label for="thermo-power">Thermostat power (Watt)</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-50" pattern="[0-9]**" name="thermo-power" value="[thermo-power]">
    </div>

    <div class="label-edit">
      <label for="time-zone">Select timezone</label>
    </div>
    <div class="cont-edit">
      <select class="w-90" name="time-zone">
        <option [time-zone-selected-0] value="0">West of Europe</option>
        <option [time-zone-selected-1] value="1">Central Europe</option>
        <option [time-zone-selected-2] value="2">Est of Europe</option>
        <option [time-zone-selected-3] value="3">EST USA</option>
        <option [time-zone-selected-4] value="4">CST USA</option>
        <option [time-zone-selected-5] value="5">MST USA</option>
        <option [time-zone-selected-6] value="6">Auckland</option>
        <option [time-zone-selected-7] value="7">Asia</option>
        <option [time-zone-selected-8] value="8">Australia</option>
      </select>
    </div>

    <div class="label-edit">
      <label for="hysteresis">Hysteresis value (e.g. 0 - 1.0°) [N.N]</label>
    </div>
    <div class="cont-edit">
      <input type="text" class="w-50" pattern="[0-9][.][0-9]" name="hysteresis" value="[hysteresis]">
    </div>

    <div class="label-edit">
      <label for="frost-temp">Frost Protection Temperature° [NN.N]</label>
    </div>
    <div class="cont-edit">
      <input type="text" class="w-50" pattern="[0-9]*[.][0-9]" name="frost-temp" value="[frost-temp]">
    </div>

    <div class="label-edit">  
      <label for="early-start">Early start duration (mins) [NN]</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-50" pattern="[0-9]*" name="early-start" value="[early-start]">
    </div>

    <div class="label-edit">
      <label for="manual-override">Manual heating override </label>
    </div>
    <div class="cont-edit">  
      <select class="w-50" name="manual-override">
        <option [manual-override-selected-on] value="ON">ON</option>
        <option [manual-override-selected-off] value="OFF">OFF</option>
      </select>
    </div>

    <div class="label-edit">
      <label for="manual-override-temp">Manual Override Temperature° [NN.N]</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-50" pattern="[0-9]*[.][0-9]" name="manual-override-temp" value="[manual-override-temp]">
    </div>

    <div class="label-edit">
      <label for="max-ambient-temp">Max ambient temperature° [NN.N]</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-50" pattern="[0-9]*[.][0-9]" name="max-ambient-temp" value="[max-ambient-temp]">
    </div>

    <div class="label-edit">
      <label for="max-heater-temp">Max heater temperature° [NN.N]</label>
    </div>
    <div class="cont-edit">  
      <input type="text" class="w-50" pattern="[0-9]*[.][0-9]" name="max-heater-temp" value="[max-heater-temp]">
    </div>

    <div class="label-edit">
      <label for="temp-units">Temperature units (Celsius [°C] / Fahrenheit [°F])</label>
    </div>
    <div class="cont-edit">  
      <select class="w-50" name="temp-units">
        <option [temp-unit-selected-c] value="C">Celsius</option>
        <option [temp-unit-selected-f] value="F">Fahrenheit</option>
      </select>
    </div>
    <br/>
    <input type="submit" value="Save">
    <br/>
    <br/>
  </form>
)rawliteral";

const char sched_html[] PROGMEM = R"rawliteral(
  <h2>Thermo-schedule</h2><br/>
  <form action="/post-sched" method="post">
    <input type="hidden" name="token" value="[token]">
    <div class="outer">
      [vals-table]
    </div>
    <div class="center">
      <br/>
      <input type="submit" value="Save">
      <br/>
      <br/>
    </div>
  </form>
)rawliteral";  

const char header_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>[dev-name]</title>
    [css-style]
  </head>
  <body>
)rawliteral";  

const char graph_html[] PROGMEM = R"rawliteral(
    <h2>Thermo-readings</h2>
    <script src="https://www.gstatic.com/charts/loader.js"></script>
    <div class="outer">
      <table>
        <tr>
          <td><div id="divGraphT1" style="width: 100%"></div></td>
          <td><div id="divGraphH1" style="width: 100%"></div></td>
        </tr>
      </table>
    </div>
    <br/>
    [status]
    <br/>
)rawliteral";

const char menu_html[] PROGMEM = R"rawliteral(
  <div id="devices-menu" class="devices-menu">
    <button id="selected-device" onclick="menuFun();" class="dropbtn-menu"></button>
    <div id="devices-menu-content" class="devices-menu-content"></div>
  </div>  

  <div id="main-container">
    <div class="topnav">
      <a href="/home">Status</a>
      <a href="/graphs">Graph</a>
      <a href="/sched">Schedule</a>
      <a href="/setup">Setup</a>
      <a href="/help">Help</a>
    </div>
)rawliteral";  

//---
const char footer_html[] PROGMEM = R"rawliteral(
    <footer>
      <div onclick="startSearch()" class="wifi-div">
        <div class="wifi-sign"></div>
        <div id="wifi-perc">...</div>
      </div>
      <div class="copyright">
        <p class="medium">ESP Smart Thermostat</p>
        <p class="ps"><i>Copyright &copy; [vers-year] GSN v.[vers-no]</i></p>
      </div>
    </footer>
  </div>
  <script type="text/javascript">
    function startSearch() {
      window.location.assign('/search');
    };
  </script>

  </body>
</html>
)rawliteral";

const char forb_html[] PROGMEM = R"rawliteral(
  <div class="centered"><h1>forbidden</h1></div>
)rawliteral";

const char ChartsTimer_html[] PROGMEM = R"rawliteral(
  <script type="text/javascript">
    function disableAJAXDrawTimer() {
      if (draw_timer != null) { 
        clearInterval(draw_timer);
        draw_timer = null;
      }
    }

    function activeAJAXDrawTimer() { 
      disableAJAXDrawTimer(); 

      var charts_ajax = new XMLHttpRequest();
      charts_ajax.open('POST', '/fresh-draw', true);
      charts_ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      charts_ajax.onreadystatechange = function() {
        if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
          var data = JSON.parse(charts_ajax.response);
          google.charts.load('current', { 'packages': ['corechart'] }); 
          function drawGraphT1() { 
            var data_table = google.visualization.arrayToDataTable(data['dt']); 
            var options = { 
              title: 'Temperature', 
              titleFontSize: 14, 
              backgroundColor: 'gainsboro', 
              legendTextStyle: { color: 'black' }, 
              titleTextStyle:  { color: 'purple' }, 
              hAxis: { color: '#FFF' }, 
              vAxis: { color: '#FFF', title: '°[temp-units]' }, 
              curveType: 'function', 
              pointSize: 1, 
              lineWidth: 1, 
              width: 600, 
              height: 400,
              colors: ['orange', 'red', 'purple'], 
              legend: { position: 'right' } 
            }; 
            var chart = new google.visualization.LineChart(document.getElementById('divGraphT1')); 
            chart.draw(data_table, options); 
          };

          google.charts.setOnLoadCallback(drawGraphT1); 

          function drawGraphH1() { 
            var data_table = google.visualization.arrayToDataTable(data['dh']);
            var options = { 
            title: 'Humidity', 
              titleFontSize: 14, 
              backgroundColor: 'gainsboro', 
              legendTextStyle: { color: 'black' }, 
              titleTextStyle:  { color: 'purple' }, 
              hAxis: { color: '#FFF' }, 
              vAxis: { color: '#FFF', title: '%' }, 
              curveType: 'function', 
              pointSize: 1, 
              lineWidth: 1, 
              width: 600, 
              height: 400, 
              colors: ['blue'], 
              legend: { position: 'right' } 
            }; 
            var chart = new google.visualization.LineChart(document.getElementById('divGraphH1')); 
            chart.draw(data_table, options); 
          }; 

          google.charts.setOnLoadCallback(drawGraphH1); 

          elem = document.getElementById('humid-ambient');
          elem.innerHTML = data['humid-ambient'];

          elem = document.getElementById('temp-target');
          elem.innerHTML = data['temp-target'];

          elem = document.getElementById('temp-heater');
          elem.innerHTML = data['temp-heater'];

          elem = document.getElementById('work-mode');
          elem.innerHTML = data['work-mode'];

          elem = document.getElementById('thermo-clock');
          elem.innerHTML = data['thermo-clock'];

          if (draw_timer == null) { 
            draw_timer = setInterval(activeAJAXDrawTimer, [timer-check-duration]); 
          } 
        }
      }
      charts_ajax.send("token=[token]");
    }
    document.addEventListener('DOMContentLoaded', function(){
      clearTimers();
      activeAJAXDrawTimer(); 
    });
  </script>
)rawliteral";

const char WifiTimer_html[] PROGMEM = R"rawliteral(
<script type="text/javascript">
  function disableAJAXWifiTimer() { 
    if (wifi_timer != null) { 
      clearInterval(wifi_timer); 
      wifi_timer = null; 
    } 
  } 

  function activeAJAXWifiTimer() { 
    disableAJAXWifiTimer(); 

    var wifi_ajax = new XMLHttpRequest();
    wifi_ajax.open('POST', '/fresh-wifi', true);
    wifi_ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    wifi_ajax.onreadystatechange = function() {
      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
        var data = JSON.parse(wifi_ajax.response);
        var elem = document.getElementById('wifi-perc');
        try {
          elem.innerHTML = data['wifi-perc'];
        } catch (err) { }
        if (wifi_timer == null) { 
          wifi_timer = setInterval(activeAJAXWifiTimer, [timer-check-duration]); 
        }         
      }
    }
    wifi_ajax.send("token=[token]");
  } 
  document.addEventListener('DOMContentLoaded', function(){
    clearTimers();
    activeAJAXWifiTimer();
  });
</script>
)rawliteral";

const char StatusTimer_html[] PROGMEM = R"rawliteral(
<script type="text/javascript">
  function disableAJAXStatusTimer() { 
    if (status_timer != null) { 
      clearInterval(status_timer); 
      status_timer = null; 
    } 
  } 

  function activeAJAXStatusTimer() { 
    disableAJAXStatusTimer(); 

    var status_ajax = new XMLHttpRequest();
    status_ajax.open('POST', '/fresh-data', true);
    status_ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    status_ajax.onreadystatechange = function() {
      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
        var data = JSON.parse(status_ajax.response);
        
        var elem;
        
        elem = document.getElementById('temp-ambient');
        elem.innerHTML = data['temp-ambient'];

        elem.classList.remove('heat-on');
        elem.classList.remove('heat-off');
        elem.classList.remove('heat-disab');

        if (data['relay-state'] == 'on') { 
          elem.classList.add('heat-on');
        } else { 
          if (data['work-mode'] == 'program' || data['work-mode'] == 'manual') { 
            elem.classList.add('heat-off');
          } else { 
            elem.classList.add('heat-disab');
          } 
        } 
        try {
          elem = document.getElementById('humid-ambient');
          elem.innerHTML = data['humid-ambient'];

          elem = document.getElementById('temp-target');
          elem.innerHTML = data['temp-target'];

          elem = document.getElementById('temp-heater');
          elem.innerHTML = data['temp-heater'];

          elem = document.getElementById('work-mode');
          elem.innerHTML = data['work-mode'];

          elem = document.getElementById('thermo-clock');
          elem.innerHTML = data['thermo-clock'];

          elem = document.getElementById('selected-device');
          elem.innerHTML = devicesJSON[0]['name'];
        } catch (err) { }

        if (status_timer == null) { 
          status_timer = setInterval(activeAJAXStatusTimer, [timer-check-duration]); 
        } 
      }
    }
    status_ajax.send("token=[token]");
  } 
  document.addEventListener('DOMContentLoaded', function(){
    clearTimers();
    activeAJAXStatusTimer(); 
  });
</script>
)rawliteral";

const char funs_after_load_doc[] PROGMEM = R"rawliteral(
  var draw_timer;
  var wifi_timer;
  var status_timer;

  function menuFun() {
    document.getElementById('devices-menu-content').classList.toggle('toggle-show-menu');
  }

  function clickDevice(name, url) {
    document.getElementById('selected-device').innerHTML = name;
    window.location.assign('/ifrm?dev-url=' + url + '&dev-name=' + name); 
    return false;
  };

  function fillDevicesMenu(devices) {
    const menu_content = document.getElementById('devices-menu-content');
    if (!!menu_content) {
      for (var key in devices) {
        if (key == 0) {
          var sel_elem = document.getElementById('selected-device');
          if (!!sel_elem)
            sel_elem.innerHTML = devices[key]['name'];
        } else {
          const devLink = document.createElement('a');
          const devText = document.createTextNode(devices[key]['name']);
          devLink.appendChild(devText);
          devLink.setAttribute('href', 'javascript: clickDevice("' + devices[key]['name'] + '", "http://' + devices[key]['url'] + ':' + devices[key]['port'] + '");');
          menu_content.appendChild(devLink);
        }
      }
    }
  }

  function clearTimers() {
    if (!!draw_timer && draw_timer != null) { 
      clearInterval(draw_timer);
      draw_timer = null;
    }
    if (!!wifi_timer && wifi_timer != null) { 
      clearInterval(wifi_timer);
      wifi_timer = null;
    }
    if (!!status_timer && status_timer != null) { 
      clearInterval(status_timer);
      status_timer = null;
    }
  }
)rawliteral";

const char OnDocReady_html[] PROGMEM = R"rawliteral(
<script type="text/javascript">
  
  [funs-after-load-doc]

  document.addEventListener('DOMContentLoaded', function(){
    // Close the dropdown if the user clicks outside of it
    window.onclick = function(event) {
      if (!event.target.matches('.dropbtn-menu')) {
        var dropdowns = document.getElementsByClassName('devices-menu-content');
        for (var i = 0; i < dropdowns.length; i++) {
          var openDropdown = dropdowns[i];
          if (openDropdown.classList.contains('toggle-show-menu')) {
            openDropdown.classList.remove('toggle-show-menu');
          }
        }
      }
    }

    if (window.location !== window.parent.location) {
      if (!!document.getElementById('devices-menu')) {
        document.getElementById('devices-menu').remove();
      }
    }

    clearTimers();

    var devicesJSON = JSON.parse('[devices]');
    fillDevicesMenu(devicesJSON);

    window.addEventListener('focus', function() {
      setTimeout(function() {
          location.reload();
        }, 1000);
    });
  });
</script>
)rawliteral";

const char iframe_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>[dev-name]</title>
    [css-style]
  </head>
  <body>
    <div id="devices-menu" class="devices-menu">
      <button id="selected-device" onclick="menuFun()" class="dropbtn-menu"></button>
      <div id="devices-menu-content" class="devices-menu-content"></div>
    </div>  

    <div id="frame-wrap">
      <iframe src="[dev-url]" id="the-frame""></iframe>
    </div>

    <script type="text/javascript">
      [funs-after-load-doc]
      document.addEventListener('DOMContentLoaded', function(){
        var devicesJSON = JSON.parse('[devices]');
        fillDevicesMenu(devicesJSON);
      });
    </script>

  </body>
</html>
)rawliteral";

const char search_html[] PROGMEM = R"rawliteral(
  <script type="text/javascript">
    var lan_url = 'http://192.168.1.';

    for (let i = 0; i < 255; i++) {
      var dev_url = lan_url + i;
      //console.log('try ... ' + dev_url);
      try {
        var searcher = new XMLHttpRequest();
        searcher.open('POST', dev_url + '/who-im', true, '[user]', '[pass]');
        searcher.setRequestHeader('Content-Type', 'text/plain');
        searcher.withCredentials = true;
        searcher.timeout = 1000;
        searcher.ontimeout = function () {
          //ajaxCallback_TO(dev_url);
        }        
        searcher.onreadystatechange = function() {
          if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
            //var data = JSON.parse(this.responseText);
        
            console.log(this.responseText);
          } else {
            //console.log(dev_url + ' xxx ');
          }
        }
        searcher.send();
      } catch (error) {
        console.log(error); 
      }
    }
  </script>
)rawliteral";

const char set_ssid_html[] PROGMEM = R"rawliteral(
  <div id="main-container">
    <br/>
    <h3>Enter wifi SSID crdentials</h3>
    <p>After you enter and save the new credentials, the device will be restarted and will try the new credentials. If it succed the new credentials the device can be accesed in LAN with LAN DHCP IP.</p>
    <br/>
    <form action="/post-ssid" method="post">
      <input type="hidden" name="token" value="[token]">

      <div class="label-edit">
        <label for="wifi-ssid">SSID name</label>
      </div>
      <div class="cont-edit">  
        <input type="text" class="w-90" name="wifi-ssid" value="[wifi-ssid]">
      </div>

      <div class="label-edit">
        <label for="wifi-pass">SSID password</label>
      </div>
      <div class="cont-edit">  
        <input type="password" class="w-90" name="wifi-pass" value="[wifi-pass]">
      </div>
      <br/>
      <input type="submit" value="Save">
      <br/>
      <br/>
    </form>
)rawliteral";

const char set_rtc_html[] PROGMEM = R"rawliteral(
  <div id="main-container">
    <br/>
    <h3>Set RTC to device</h3>
    <br/>
    <form action="/post-rtc" method="post">
      <input type="hidden" name="token" value="[token]">

      <div class="label-edit">
        <label for="date-time-local">Input date & time</label>
      </div>
      <div class="cont-edit">  
        <input type="datetime-local" class="w-70" id="date-time-local" name="date-time-local">
      </div>

      <br/>
      <input type="submit" value="Save">
      <br/>
      <br/>
    </form>

  <script type="text/javascript">
    document.addEventListener('DOMContentLoaded', function(){
      window.addEventListener('focus', function() {
        setTimeout(function() {
            location.reload();
            document.getElementById('date-time-local').value = new Date().toLocaleString();
          }, 1000);
      });
    });
  </script>

)rawliteral";
