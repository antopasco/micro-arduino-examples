#include <Wire.h>
#include <LSM303DLM.h>
#include <avr/pgmspace.h>

LSM303 accelero;
const char httpResponse[] PROGMEM = {
"HTTP/1.1 200 OK\r\n"
"Content-Length: \r\n"
"Connection: keep-alive\r\n"
"\r\n"}; //end of httpResponse[]
const char htmlStart[] PROGMEM = { //Aistin logo, icon & javascript functions
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
//"<link rel='shortcut icon' href='data:image/jpeg;base64,AAABAAEAICAQAAEABADoAgAAFgAAACgAAAAgAAAAQAAAAAEABAAAA"
//"AAAAAAAAAAAAAAAAAAAAAAAAAAAAACXOgAAnUUBAKVWFgCzbzcAMGPvAMCHWgBPevEA0qqJAIum9"
//"wDdwKoAwtL7AO7f0gDj6/wA/PjyAPz//gAAAAAA7u7u7u7u7u7t7u2+3u7u7u7u7u7u7u7u7u3rD"
//"e7u7u7u7u7u7u7u7u7u4iLu7u7u7u7u7u7u7u7u7tEinu7u7u7u7u7u7u7u7u6yMg3t7u7u7u7u7"
//"u7u7u3uMtIn7u7u7u7u7u7u7u7u7QHeI+7u7u7u7u7u7u7u7usl7THu7u7u7u7u7u7u7u7nLu2Rn"
//"u7u7u7u7u7u7u7t4y7u4H7u7u7u7u7u7u7u7uC+7uGe7u7e7u7u7u7u7u2y7u7hvu7u7u7u7u7u7"
//"u7tee7t4e7u7u7t7d7u3u7u7n7e7uXu7u7e7e7u7e7u7d7e7t597u7u7uhkRq7u3t7pUhEl3u3u7"
//"t2GSIhI7u7rITESEgve7u7uRt7u5K7uUSI77uUj7u7u3Gju7u2N0CI+7u7uMe7u7uxI3e7uqxIr7"
//"u7u3iHt7u7uZu7u7pIh3u7u3tsl3u7u7uRu7dciLe7u7u7iHe7u7u7shGySIu7u7u7u4l7u7u7u7"
//"e7rIj7e7u7t7ZG97u7l7u7usSLu7u7u7u4w7e7ue+7d6wIt7u7u7u7uMu7t2X7u7pEi3u7u7u7u7"
//"nDe7VXu7uIiHu7u7u7u7u7iEiCe7uIiF+7u7u7u7u7u7dm97u4iE97u7u7u7u7u7u7u7u7uF77u7"
//"u7u7u7u7u7u7u7u7u7u7u7u7u7u7u7u7u7u7u4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
//"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
//"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=='/>\r\n"
"<title>Aistin</title>\r\n"
"<img src='data:image/jpeg;base64,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"
"n1mXyMFPpSlUFcX/9k='/>\r\n"
"<script>\r\n"
"function setClass(name,value) {\r\n"
"elements=document.getElementsByName(name);\r\n"
"for(i=0;i<elements.length;i++)\r\n"
	"elements[i].className=value;\r\n"
"}\r\n"
"function xmlToElement(url){\r\n"
	"var xH=new XMLHttpRequest();\r\n"
	"xH.onreadystatechange=function(){\r\n"
		"if(xH.readyState==4&&xH.status==200){\r\n"
			"var xD=xH.responseXML;\r\n"
			"x=xD.childNodes[0].childNodes;\r\n"
			"for (i=0;i<x.length;i++){\r\n"
				"var tag=x[i].nodeName;\r\n"
				"var element=document.getElementsByName(tag);\r\n"
				"if(tag!='#text'&&document.body.contains(element[0])){\r\n"
					"value=xD.getElementsByTagName(tag)[0].childNodes[0].nodeValue.split(',');\r\n"
					"if(tag=='WAKEY'||tag=='WSKEY'||tag=='WSLQ'){\r\n"
						"value=[value[0]+','+value[1],value[2]];\r\n"
					"}\r\n"
					"for(k=0;(k<value.length&&k<element.length);k++)\r\n"
						"element[k].value=value[k];\r\n"
				"}\r\n"
			"}\r\n"
		"}\r\n"
		"updateHidden();\r\n"
		"wmode();\r\n"
		"setEleVal('clientIP', document.getElementsByName('WANN')[1].value);\r\n"
	"}\r\n"
	"xH.open('POST',url,true);\r\n"
	"xH.send();\r\n"
"}\r\n"
"function updateHidden(){\r\n"
	"inputs=document.getElementsByTagName('input');\r\n"
	"for(var i=0;i<inputs.length;i++){\r\n"
		"if(inputs[i].type.toLowerCase()=='hidden'){\r\n"
			"id=inputs[i].id;\r\n"
			"element=document.getElementsByName(id+'sub')[0];\r\n"
			"if(document.body.contains(element)){\r\n"
				"elementType = element.type.toLowerCase();\r\n"
				"if(elementType=='radio'){\r\n"
					"radio = document.getElementById(inputs[i].name+inputs[i].value);\r\n"
						"if(document.body.contains(radio))\r\n"
							"radio.checked=true;\r\n"
				"}\r\n"
				"else\r\n"
					"element.value=inputs[i].value;\r\n"
		"}}\r\n"
	"}\r\n"
"}\r\n"
"function setEleVal(id,value){document.getElementById(id).value=value;}\r\n"
"function sendForm(url,form){\r\n"
	"var xH = new XMLHttpRequest();\r\n"
	"var prevName;\r\n"
	"var data='';\r\n"
	"console.log(document.getElementById(form));\r\n"
	"form=document.getElementById(form).getElementsByTagName('input');\r\n"
	"for(i=0;i<form.length;i++){\r\n"
		"if(form[i].type.toLowerCase()!='radio'&&form[i].parentNode.className!='hidden'){\r\n"
			"if(form[i].name!=prevName)\r\n"
				"data+='\\r\\n'+form[i].name+'='+form[i].value;\r\n"
			"else\r\n"
				"data+=','+form[i].value;\r\n"
			"prevName=form[i].name;\r\n"
		"}\r\n"
	"}\r\n"
	"data=data.substring(2)+'\\r\\n';\r\n"
	"xH.open('POST',url,true);\r\n"
	"xH.setRequestHeader('Content-type','application/x-www-form-urlencoded');\r\n"
	"xH.send(data);\r\n"
"}\r\n"
"function sndCmd(command, id){\r\n"
	"var xH=new XMLHttpRequest();\r\n"
	"xH.onreadystatechange=function(){\r\n"
		"if(xH.readyState==4&&xH.status==200)\r\n"
			"setEleVal(id,xH.responseText);\r\n"
	"}\r\n"
	"xH.open('POST','cmd',true);\r\n"
	"xH.send(command+'\\r\\n');\r\n"
"}\r\n"
"function wmode(){\r\n"
	"value = document.getElementById('WMODE').value;\r\n"
	"if(value.indexOf('AP')>-1)\r\n"
		"setClass('ap','');\r\n"
	"else\r\n"
		"setClass('ap','hidden');\r\n"
	"if(value.indexOf('STA')>-1)\r\n"
		"setClass('sta','');\r\n"
	"else\r\n"
		"setClass('sta','hidden');\r\n"
"}\r\n"
"</script>\r\n"
"<p><a href='index'>Index</a> <a href='cfg'>Config</a></p>"
"</head>\r\n"
}; //end of htmlStart[]
const char mainPage[] PROGMEM = { //timer reading accelerometer value every 2s
"<body>\r\n"
"<textarea rows='1' cols='50' id='data'></textarea><br>\r\n"
"<button type='button' onclick=\"timer2sec = setInterval(function(){sndCmd('accel', 'data')}, 2000);\">Get data</button>\r\n"
"<button type='button' onclick='clearInterval(timer2sec);'>Stop</button>\r\n"
"</body></html>\r\n"
}; //end of mainPage[]
const char cfgPage[] PROGMEM  = { //wlan module configuration page
"<body onload=\"setTimeout(function(){xmlToElement('cfg.xml')},100);\">\r\n"
"<h1>Aistin cfg</h1>\r\n"
"<style>\r\n"
".hidden{display:none}\r\n"
"form, div{\r\n"
	"float: left;\r\n"
	"width: 350px;\r\n"
	"padding: 10px;\r\n"
	"}\r\n"
"form input,form select,form label{float:right;}\r\n"
"h2{color: navy;}\r\n"
"</style>\r\n"
"<button onclick=\"setClass('advanced','hidden')\">Basic</button>\r\n"
"<button onclick=\"setClass('advanced','')\">Advanced</button><br>\r\n"
"<button onclick=\"sendForm('reboot','dummy')\">Reboot</button>\r\n"
"<form id='dummy' class='hidden'></form>\r\n"
"<button type='button' onclick=\"xmlToElement('cfg.xml')\">Read config</button><br>\r\n"
"<form name='misc' id='misc'>\r\n"
"<h2>MISC</h2>\r\n"
"<b>Module mode:</b><input type='hidden' name='WMODE' id='WMODE'>\r\n"
	"<label for='WMODEAP'>Access point</label>\r\n"
	"<input type='radio' name='WMODEsub' id='WMODEAP' onclick=\"setEleVal('WMODE','AP');wmode();\"><br>\r\n"
	"<label for='WMODESTA'>Client</label>\r\n"
	"<input type='radio' name='WMODEsub' id='WMODESTA' onclick=\"setEleVal('WMODE','STA');wmode();\"><br>\r\n"
	"<label for='WMODEAPSTA'>Both</label>\r\n"
	"<input type='radio' name='WMODEsub' id='WMODEAPSTA' onclick=\"setEleVal('WMODE','APSTA');wmode();\"><br>\r\n"
"<p name='advanced' class='hidden'>\r\n"
"Protocol:<input type='hidden' name='NETP' id='NETPprtcl'>\r\n"
	"<label for='NETPTCP'>TCP</label>\r\n"
	"<input type='radio' name='NETPprtclsub' id='NETPTCP' onclick=\"setEleVal('NETPprtcl','TCP')\"><br>\r\n"
	"<label for='NETPUDP'>UDP</label>\r\n"
	"<input type='radio' name='NETPprtclsub' id='NETPUDP' onclick=\"setEleVal('NETPprtcl','UDP')\"><br><br>\r\n"
"Network mode:<input type='hidden' name='NETP' id='NETPntwrk'>\r\n"
	"<label for='NETPServer'>Server</label>\r\n"
	"<input type='radio' name='NETPntwrksub' id='NETPServer' onclick=\"setEleVal('NETPntwrk','Server')\"><br>\r\n"
	"<label for='NETPClient'>Client</label>\r\n"
	"<input type='radio' name='NETPntwrksub' id='NETPClient' onclick=\"setEleVal('NETPntwrk','Client')\"><br><br>\r\n"
"Port:<input name='NETP'><br><br>\r\n"
"Connect to IP:<input name='NETP'><br><br>\r\n"
"</p>\r\n"
"<button type='button' onclick=\"sendForm('config=','misc')\">Save</button>\r\n"
"</form>\r\n"
"<form id='ap' name='ap'>\r\n"
"<h2>Access point settings:</h2>\r\n"
"<p name='advanced' class='hidden'>\r\n"
"IP address:<input name='LANN'><br><br>\r\n"
"Subnet mask:<input name='LANN'><br><br>\r\n"
"DHCP:<input type='hidden' name='WADHCP' id='WADHCPdhcp'>\r\n"
	"<label for='WADHCPon'>On</label>\r\n"
	"<input type='radio' name='WADHCPdhcpsub' id='WADHCPon' onclick=\"setEleVal('WADHCPdhcp','on')\"><br>\r\n"
	"<label for='WADHCPoff'>Off</label>\r\n"
	"<input type='radio' name='WADHCPdhcpsub' id='WADHCPoff' onclick=\"setEleVal('WADHCPdhcp','off')\"><br><br>\r\n"
"Start address:<input name='WADHCP'><br><br>\r\n"
"End address:<input name='WADHCP'><br><br>\r\n"
"Wifi mode:<input type='hidden' name='WAP' id='WAPmode'>\r\n"
	"<label for='apMode11B'>11B</label>\r\n"
	"<input type='radio' name='WAPmodesub' id='WAP11B' onclick=\"setEleVal('WAPmode','11B')\"><br>\r\n"
	"<label for='apMode11BG'>11BG</label>\r\n"
	"<input type='radio' name='WAPmodesub' id='WAP11BG' onclick=\"setEleVal('WAPmode','11BG')\"><br>\r\n"
	"<label for='apMode11BGN'>11BGN</label>\r\n"
	"<input type='radio' name='WAPmodesub' id='WAP11BGN' onclick=\"setEleVal('WAPmode','11BGN')\"><br><br>\r\n"
"SSID: <input name='WAP'><br><br>\r\n"
"Channel:<input type='hidden' name='WAP' id='WAPch' value='Automatic'>\r\n"
	"<select name='WAPchsub' onChange=\"setEleVal('WAPch',document.getElementsByName('WAPchsub')[0].value)\">\r\n"
	"<option value='AUTO'>Automatic</option>\r\n"
	"<option value='CH1'>1</option>\r\n"
	"<option value='CH2'>2</option>\r\n"
	"<option value='CH3'>3</option>\r\n"
	"<option value='CH4'>4</option>\r\n"
	"<option value='CH5'>5</option>\r\n"
	"<option value='CH6'>6</option>\r\n"
	"<option value='CH7'>7</option>\r\n"
	"<option value='CH8'>8</option>\r\n"
	"<option value='CH9'>9</option>\r\n"
	"<option value='CH10'>10</option>\r\n"
	"<option value='CH11'>11</option>\r\n"
	"</select><br></p>\r\n"
"Ecryption:<input type='hidden' name='WAKEY' id='WAKEYencr' value='OPEN,NONE'>\r\n"
	"<select name='WAKEYencrsub' onChange=\"setEleVal('WAKEYencr',document.getElementsByName('WAKEYencrsub')[0].value)\">\r\n"
	"<option value='OPEN,NONE'>Open</option>\r\n"
	"<option value='WPA2PSK,AES'>WPA2 AES</option>\r\n"
	"</select><br><br>\r\n"
"Key:<input name='WAKEY'><br><br>\r\n"
"<button type='button' onclick=\"sendForm('config=','ap')\">Save</button>\r\n"
"</form>\r\n"
"<form name='sta' id='sta'>\r\n"
"<h2>WiFi client settings:</h2>\r\n"
"SSID:<input name='WSSSID'><br><br>\r\n"
"Encryption:<input type='hidden' name='WSKEY' id='WSKEYencr' value='OPEN,NONE'>\r\n"
	"<select name='WSKEYencrsub' onChange=\"setEleVal('WSKEYencr',document.getElementsByName('WSKEYencrsub')[0].value)\">\r\n"
	"<option value='OPEN,NONE'>Open</option>\r\n"
	"<option value='OPEN,WEP-H'>WEP open HEX</option>\r\n"
	"<option value='OPEN,WEP-A'>WEP open ASCII</option>\r\n"
	"<option value='SHARED,WEP-H'>WEP shared HEX</option>\r\n"
	"<option value='OPEN,WEP-H'>WEP shared ASCII</option>\r\n"
	"<option value='WPAPSK,TKIP'>WPA TKIP</option>\r\n"
	"<option value='WPAPSK,AES'>WPA AES</option>\r\n"
	"<option value='WPA2PSK,TKIP'>WPA2 TKIP</option>\r\n"
	"<option value='WPA2PSK,AES'>WPA2 AES</option>\r\n"
	"</select><br><br>\r\n"
"Key:<input name='WSKEY'><br>\r\n"
"<p name='advanced' class='hidden'>\r\n"
"Mode:<input type='hidden' name='WANN' id='WANNmode'>\r\n"
	"<label for='WANNDHCP'>DHCP</label>\r\n"
	"<input type='radio' name='WANNmodesub' id='WANNDHCP' onClick=\"setEleVal('WANNmode','DHCP')\"> \r\n"
	"<label for='WANNstatic'>Static</label>\r\n"
	"<input type='radio' name='WANNmodesub' id='WANNstatic' onClick=\"setEleVal('WANNmode','static')\"><br><br>\r\n"
"IP address:<input name='WANN'><br><br>\r\n"
"Subnet mask:<input name='WANN'><br><br>\r\n"
"Default gateway:<input name='WANN'>\r\n"
"</p>\r\n"
"<button type='button' onclick=\"sendForm('config=','sta')\">Save</button>\r\n"
"</form>\r\n"
"<br><div id='status'>\r\n"
"<h2>Status:</h2>\r\n"
"Client IP:<input id='clientIP' readonly><br>\r\n"
"Link quality:<input name='WSLQ' readonly><br>\r\n"
"<textarea rows='10' cols='60' id='wscan' readonly></textarea>\r\n"
"<button type='button' onclick=\"sndCmd('AT+WSCAN', 'wscan')\">Scan available networks</button><br></div>\r\n"
"<div name='advanced' id='customcmd' class='hidden'>\r\n"
"<h2>Custom commands:</h2>\r\n"
"<input id='command'>\r\n"
"<button type='button' onclick=\"sndCmd(document.getElementById('command').value, 'response')\">Send</button><br>\r\n"
"<textarea rows='10' cols='60' id='response'></textarea>\r\n"
"</div></body></html>\r\n"
}; //end of cfgPage[]

void setup() {
  Wire.begin();
  Serial.begin(38400);
  accelero.init(); //initiate motion sensor
  //while(!Serial);
  Serial1.begin(115200);
  while(!Serial1);
  if(enterAtMode()){ //set few default configs
    Serial1.print("AT+WMODE=APSTA\r");
    Serial1.find("+ok");
    Serial1.print("AT+UART=38400,8,1,None,NFC\r");
    Serial1.find("+ok");
    Serial1.print("AT+NETP=TCP,Server,33170,87.92.73.117\r");
    Serial1.find("+ok");
    Serial1.print("AT+Z\r");
    delay(1000);
  }
  Serial1.end();
  Serial1.begin(38400); //Start serial1 connection 38400Bd
}

void loop() {
  if(Serial1.available())
    readWifi();
  if(Serial.available()){
    String serialData = Serial.readString();
    if(serialData == "AT") 
      //type "AT" into serial to enter AT command mode
      enterAtMode();
    else
      Serial1.print(serialData + "\r");
      //send commands from serial wifi module
  }
}

void printFromPROGMEM(const char *PROGMEM_message, int messageSize) {
  //read & send data from program memory to serial1 
  for(int k=0;k<messageSize;k++)
    Serial1.write(pgm_read_byte_near(PROGMEM_message + k));
}
void sendHttpResponse(int contentLength=0){
  //send http response & content length
  printFromPROGMEM(&httpResponse[0], 33);
  Serial1.print(contentLength);
  printFromPROGMEM(&httpResponse[33],strlen(httpResponse)-33);
}
void readWifi(void){
  String htmlHeader = String();
  while(Serial1.available()){
    //read serial1 while there is data incoming or we got the whole header
    while(htmlHeader.indexOf("\r\n\r\n")==-1 && Serial1.available()){
      char c = Serial1.read();
      htmlHeader += c;
      //slow down to make sure there is no data yet to arrvie in buffer
      if(!Serial1.available())
        delay(1);
    }
    Serial.println(htmlHeader); //print the header
    htmlHeader = htmlHeader.substring(0,htmlHeader.indexOf("\r\n")); //shorten the string
    if(htmlHeader.indexOf(">S")>-1 || htmlHeader.indexOf("~_s")>-1)
      aistinSensorResponse();
    else if(htmlHeader.indexOf("@18A806")>-1)
      aistinAccelData();
    else if((htmlHeader.indexOf("^Aistin@800501:01")>-1) || (htmlHeader.indexOf("factoryreset")>-1)){
      enterAtMode();
      Serial1.print("AT+RELD\r");
    }
    else if(htmlHeader.indexOf("HTTP")>-1){
      //check what browser requested
      if(htmlHeader.indexOf("cfg.xml")>-1)
        readConfig(); //read wifi module config and return it as xml response
      else if(htmlHeader.indexOf("config=")>-1)
        setConfig(); //read browser sent config data
      else if(htmlHeader.indexOf("favicon.ico")>-1)
        sendHttpResponse();//only return HTTP 200 for favicon requests
      else if(htmlHeader.indexOf("cmd")>-1)
        customCommand();
      else if(htmlHeader.indexOf("cfg")>-1){
        sendHttpResponse((strlen(htmlStart)+strlen(cfgPage)));
        //send http response with content size & config page data
        printFromPROGMEM(&htmlStart[0],strlen(htmlStart));
        printFromPROGMEM(&cfgPage[0],strlen(cfgPage));
      }
      else{
        sendHttpResponse((strlen(htmlStart)+strlen(mainPage)));
        //send http response with content size & main page data
        printFromPROGMEM(&htmlStart[0],strlen(htmlStart));
        printFromPROGMEM(&mainPage[0],strlen(mainPage));
      }
    }
    htmlHeader = '\0';
  }
}
void readConfig(void)
{
  enterAtMode();
  //read current settings from wifi module 
  String configResponse = "<RESPONSE>\r\n";
  configResponse += atValueToXML("NETP");
  configResponse += atValueToXML("WMODE");
  configResponse += atValueToXML("LANN");
  configResponse += atValueToXML("WADHCP");
  configResponse += atValueToXML("WAP");
  configResponse += atValueToXML("WAKEY");
  configResponse += atValueToXML("WSSSID");
  configResponse += atValueToXML("WSKEY");
  configResponse += atValueToXML("WANN");
  configResponse += atValueToXML("WSLQ");
  configResponse += atValueToXML("WSDNS");
  configResponse += "</RESPONSE>";
  Serial1.print("AT+ENTM\r");
  
  delay(500);
  sendHttpResponse(configResponse.length());
  Serial1.println(configResponse);
}
void setConfig(void)
{ //read form data and set config accordingly
  String configData = Serial1.readString();
  sendHttpResponse(); //response we got the data
  enterAtMode();
  int lastCRLN = configData.lastIndexOf("\r\n");
  int currentCRLN = 0;
  int nextCRLN = 0;
  while(nextCRLN<lastCRLN){
    nextCRLN = configData.indexOf("\r\n", currentCRLN)+1;
    //start with 'AT+' add next command, value & \r and send it to wifi module  
    Serial1.print("AT+"+configData.substring(currentCRLN, nextCRLN));
    Serial.print("AT+"+configData.substring(currentCRLN, nextCRLN));
    if(Serial1.find("+ok"))
      Serial.println(" ok!");
    else
      Serial.println(" error!");
    //wait for +ok before sending next command and inform to serial if there were problems
    currentCRLN = nextCRLN+1;
    //skip the \n
  }
  Serial1.print("AT+Z\r");
}
void customCommand(void){
  String cmd = Serial1.readString();
  String response = String();
  boolean atModeOn = false;
  cmd = cmd.substring(0, cmd.indexOf("\r\n"));  
  if(cmd.indexOf("AT+")==0){
    enterAtMode();
    Serial1.print(cmd+'\r');
    Serial.println(cmd);
      Serial1.find("+ok");
      if(Serial1.peek() == '=')
        Serial1.read();
      response = Serial1.readString();
      Serial1.print("AT+ENTM\r");
    //}
    delay(100);
  }
  else if(cmd.indexOf("accel")==0){
    int x,y,z;
    accelero.readRawXYZ(x,y,z);
    response = String(x) +' '+ String(y) +' '+ String(z);
  }
  sendHttpResponse(response.length());
  Serial1.print(response);
}
bool enterAtMode(void)
{
  //Serial.println("Entering AT mode");
  delay(250);
  Serial1.write('+');
  delay(1);
  Serial1.write('+');
  delay(1);
  Serial1.write('+');
  if(Serial1.find("a"));
    Serial1.write('a');
  if(Serial1.find("+ok")){
    Serial.println("AT mode");
    return true;
  }
  else{
    Serial.println("Error");
    Serial1.print('\r');
    return false;
  }
}
String atValueToXML(String command){
  Serial1.print("AT+"+command+'\r');
  String value;
  if(Serial1.find("+ok="))
    value = Serial1.readStringUntil('\r');
  else
    value = "error";
  return '<'+command+'>'+value+"</"+command+">\r\n";
}
void aistinSensorResponse(void){
  //responsing we(Aistin) have sensor 0x18(accelerometer)
  Serial1.println(">D~Aistin@801400$Aistin");
  Serial1.println(">D~Aistin@8038:8018");
}
void aistinAccelData(void){
  int signedX, signedY, signedZ;
  unsigned int x,y,z;
  accelero.readRawXYZ(signedX,signedY,signedZ);
  x = signedX;
  y = signedY;
  z = signedZ;
  //responsing our (aistin) sensor 0x18 (accelerometer) reg values 0x18 - 0x1D (A806) are
  Serial1.print(">D~aistin@18A806:");
  //smashing sensor values to hex and rearraging them to same order as in the register
  Serial1.print((x>>4)%16,HEX);
  Serial1.print((x>>0)%16,HEX);
  Serial1.print((x>>12)%16,HEX);
  Serial1.print((x>>8)%16,HEX);
  Serial1.print((y>>4)%16,HEX);
  Serial1.print((y>>0)%16,HEX);
  Serial1.print((y>>12)%16,HEX);
  Serial1.print((y>>8)%16,HEX);
  Serial1.print((z>>4)%16,HEX);
  Serial1.print((z>>0)%16,HEX);
  Serial1.print((z>>12)%16,HEX);
  Serial1.println((z>>8)%16,HEX);
}
