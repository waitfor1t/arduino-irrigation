const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head> 
  <script>
    function loadData(url, callback) { // From http://www.martyncurrey.com/esp8266-and-the-arduino-ide-part-6-javascript-and-ajax/
      var request = new HMLHttpRequest();
      nocache = "&nocache=" + Math.random() * 1000000; // This was in another bit of code
      request.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        if (this.response.Text != null) {document.getElementById("02").innerHTML = this.responseText;};
        callback.apply(request);
        }
      };
      request.open("GET", "url" + nocache, true);
      request.send(null);
      // setTimeout('getstates()', 1000);
    };
    function updateData() {
      document.getElementById("02").innerHTML = this.responseText;
    };
  </script>
</head>
<body onload="loadData();">

<h1>States</h1>
<span id='label00'>Valve</span>&nbsp;<a href='?0=0'>00 On</a>&nbsp;<a href='/?0=1'>00 Off</a> D0 Unused <span id="00">State</span><br>
<span id='label01'>Valve</span>&nbsp;<a href='?1=0'>01 On</a>&nbsp;<a href='/?1=1'>01 Off</a> D1 Unused (TX - D10 disabled in debug) <span id="01">State</span><br>
<span id='label02'>Valve</span>&nbsp;<a href='?2=0'>02 On</a>&nbsp;<a href='/?2=1'>02 Off</a> D2  <span id="02">State</span><br>
<span id='label03'>Valve</span>&nbsp;<a href='?3=0'>03 On</a>&nbsp;<a href='/?3=1'>03 Off</a> D3 Unused <span id="03">State</span><br> //GPIO 1 swap the pin to a GPIO.
<span id='label04'>Valve</span>&nbsp;<a href='?4=0'>04 On</a>&nbsp;<a href='?4=1'>04 Off</a> D4 <span id="04">State</span><br>
<span id='label05'>Valve</span>&nbsp;<a href='?5=0'>05 On</a>&nbsp;<a href='?5=1'>05 Off</a> D5 <span id="05">State</span><br>
<span id='label06'>Valve</span>&nbsp;<a href='?6=0'>06 On</a>&nbsp;<a href='?6=1'>06 Off</a> D6 unuseable <span id="06">State</span><br>
<span id='label07'>Valve</span>&nbsp;<a href='?7=0'>07 On</a>&nbsp;<a href='?7=1'>07 Off</a> D7 unuseable <span id="07">State</span><br>
<span id='label08'>Valve</span>&nbsp;<a href='?8=0'>08 On</a>&nbsp;<a href='?8=1'>08 Off</a> D8 unuseable <span id="08">State</span><br>
<span id='label09'>Valve</span>&nbsp;<a href='?9=0'>09 On</a>&nbsp;<a href='?9=1'>09 Off</a> D0 - controls flash <span id="09">State</span><br>
<span id='label10'>Valve</span>&nbsp;<a href='?10=0'>10 On</a>&nbsp;<a href='?10=1'>10 Off</a> D10 - input only <span id="10">State</span><br>
<span id='label11'>Valve</span>&nbsp;<a href='?11=0'>11 On</a>&nbsp;<a href='?11=1'>11 Off</a> D11 <span id="11">State</span><br>
<span id='label12'>Valve</span>&nbsp;<a href='?12=0'>12 On</a>&nbsp;<a href='?12=1'>12 Off</a> D12 <span id="12">State</span><br>
<span id='label13'>Valve</span>&nbsp;<a href='?13=0'>13 On</a>&nbsp;<a href='?13=1'>13 Off</a> D13 <span id="13">State</span><br>
<span id='label14'>Valve</span>&nbsp;<a href='?14=0'>14 On</a>&nbsp;<a href='?14=1'>14 Off</a> D14 <span id="14">State</span><br>
<span id='label15'>Valve</span>&nbsp;<a href='?15=0'>15 On</a>&nbsp;<a href='?15=1'>15 Off</a> D15 <span id="15">State</span><br>
<span id='label16'>Valve</span>&nbsp;<a href='?16=0'>16 On</a>&nbsp;<a href='?16=1'>16 Off</a> D16 <span id="16">State</span><br>
</body>
</html>)rawliteral";
