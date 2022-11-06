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
