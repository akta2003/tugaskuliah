/*
ESP32-CAM Tracking.js Color Detection
Author : ChungYi Fu (Kaohsiung, Taiwan)  2020-11-8 12:00
https://www.facebook.com/francefu

Color List
https://en.wikipedia.org/wiki/Web_colors

首頁
http://APIP
http://STAIP

自訂指令格式 :  
http://APIP/?cmd=P1;P2;P3;P4;P5;P6;P7;P8;P9
http://STAIP/?cmd=P1;P2;P3;P4;P5;P6;P7;P8;P9

預設AP端IP： 192.168.4.1
http://192.168.xxx.xxx?ip
http://192.168.xxx.xxx?mac
http://192.168.xxx.xxx?restart
http://192.168.xxx.xxx?digitalwrite=pin;value        //value= 0 or 1
http://192.168.xxx.xxx?analogwrite=pin;value        //value= 0~255
http://192.168.xxx.xxx?flash=value        //value= 0~255 閃光燈
http://192.168.xxx.xxx?getstill                 //取得視訊影像
http://192.168.xxx.xxx?framesize=size     //size= UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA 改變影像解析度
http://192.168.xxx.xxx?quality=value    // value = 10 to 63
http://192.168.xxx.xxx?brightness=value    // value = -2 to 2
http://192.168.xxx.xxx?contrast=value    // value = -2 to 2 
http://192.168.xxx.xxx/?tcp=domain;port;request;wait
--> request = /xxxxx
--> wait = 0 ro 1
http://192.168.xxx.xxx/?linenotify=token;request
--> request = message=xxxxx
--> request = message=xxxxx&stickerPackageId=xxxxx&stickerId=xxxxx
http://192.168.xxx.xxx?sendCapturedImageToLineNotify=token  //傳送影像截圖至LineNotify，最大解析度是SXGA

查詢Client端IP：
查詢IP：http://192.168.4.1/?ip
重設網路：http://192.168.4.1/?resetwifi=ssid;password
*/

//Masukkan ssid pass WIFI
const char* ssid     = "Ppp";   //ssid Wi-Fi
const char* password = "yayayaya";   //Password Wi-Fi

//Masukkan kata sandi akun koneksi AP
const char* apssid = "ESP32-CAM";   //Namanya bisa dikustomisasi untuk menampilkan nama dan IP jaringan lokal di daftar hotspot WiFi
const char* appassword = "12345678";    //Kata sandi AP harus terdiri dari minimal 8 karakter

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"         //Video
#include "soc/soc.h"            //Digunakan ketika catu daya tidak stabil dan tidak dapat dihidupkan ulang.
#include "soc/rtc_cntl_reg.h"   //Digunakan ketika catu daya tidak stabil dan tidak dapat dihidupkan ulang.

String Feedback="";   //Kembalikan pesan klien
//nilai parameter perintah
String Command="",cmd="",P1="",P2="",P3="",P4="",P5="",P6="",P7="",P8="",P9="";
//Nilai status pembongkaran instruksi
byte ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;

// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled

//Pengaturan pin modul ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WiFiServer server(80);

void ExecuteCommand()
{
  //Serial.println("");
  //Serial.println("Command: "+Command);
  if (cmd!="getstill") {
    Serial.println("cmd= "+cmd+" ,P1= "+P1+" ,P2= "+P2+" ,P3= "+P3+" ,P4= "+P4+" ,P5= "+P5+" ,P6= "+P6+" ,P7= "+P7+" ,P8= "+P8+" ,P9= "+P9);
    Serial.println("");
  }
  
  if (cmd=="your cmd") {
    // You can do anything.
    // Feedback="<font color=\"red\">Hello World</font>";
  }
  else if (cmd=="ip") {
    Feedback="AP IP: "+WiFi.softAPIP().toString();    
    Feedback+=", ";
    Feedback+="STA IP: "+WiFi.localIP().toString();
  }  
  else if (cmd=="mac") {
    Feedback="STA MAC: "+WiFi.macAddress();
  }  
  else if (cmd=="resetwifi") {  //Setel ulang koneksi WIFI
    WiFi.begin(P1.c_str(), P2.c_str());
    Serial.print("Connecting to ");
    Serial.println(P1);
    long int StartTime=millis();
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        if ((StartTime+5000) < millis()) break;
    } 
    Serial.println("");
    Serial.println("STAIP: "+WiFi.localIP().toString());
    Feedback="STAIP: "+WiFi.localIP().toString();
  }    
  else if (cmd=="restart") {
    ESP.restart();
  }
  else if (cmd=="digitalwrite") {
    ledcDetachPin(P1.toInt());
    pinMode(P1.toInt(), OUTPUT);
    digitalWrite(P1.toInt(), P2.toInt());
  }   
  else if (cmd=="analogwrite") {
    if (P1="4") {
      ledcAttachPin(4, 4);  
      ledcSetup(4, 5000, 8);   
      ledcWrite(4,P2.toInt());  
    }
    else {
      ledcAttachPin(P1.toInt(), 5);
      ledcSetup(5, 5000, 8);
      ledcWrite(5,P2.toInt());
    }
  }  
  else if (cmd=="flash") {
    ledcAttachPin(4, 4);  
    ledcSetup(4, 5000, 8);   
     
    int val = P1.toInt();
    ledcWrite(4,val);  
  }  
  else if (cmd=="framesize") { 
    sensor_t * s = esp_camera_sensor_get();  
    if (P1=="QQVGA")
      s->set_framesize(s, FRAMESIZE_QQVGA);
    else if (P1=="HQVGA")
      s->set_framesize(s, FRAMESIZE_HQVGA);
    else if (P1=="QVGA")
      s->set_framesize(s, FRAMESIZE_QVGA);
    else if (P1=="CIF")
      s->set_framesize(s, FRAMESIZE_CIF);
    else if (P1=="VGA")
      s->set_framesize(s, FRAMESIZE_VGA);  
    else if (P1=="SVGA")
      s->set_framesize(s, FRAMESIZE_SVGA);
    else if (P1=="XGA")
      s->set_framesize(s, FRAMESIZE_XGA);
    else if (P1=="SXGA")
      s->set_framesize(s, FRAMESIZE_SXGA);
    else if (P1=="UXGA")
      s->set_framesize(s, FRAMESIZE_UXGA);           
    else 
      s->set_framesize(s, FRAMESIZE_QVGA);     
  }
  else if (cmd=="quality") { 
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt(); 
    s->set_quality(s, val);
  }
  else if (cmd=="contrast") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt(); 
    s->set_contrast(s, val);
  }
  else if (cmd=="brightness") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt();  
    s->set_brightness(s, val);  
  }
  else if (cmd=="detectCount") {
    Serial.println(P1+" = "+P2); 
  }
  else if (cmd=="serial") { 
    if (P1!=""&P1!="stop") Serial.println(P1);
    if (P2!=""&P2!="stop") Serial.println(P2);
    if (P3!=""&P3!="stop") Serial.println(P3);
    if (P4!=""&P4!="stop") Serial.println(P4);
    if (P5!=""&P5!="stop") Serial.println(P5);
    if (P6!=""&P6!="stop") Serial.println(P6);
    if (P7!=""&P7!="stop") Serial.println(P7);
    if (P8!=""&P8!="stop") Serial.println(P8);
    if (P9!=""&P9!="stop") Serial.println(P9);
    Serial.println();
  } 
  else {
    Feedback="Command is not defined.";
  }
  if (Feedback=="") Feedback=Command;  
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //Matikan daya dan nyalakan kembali jika daya tidak stabil
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);  //Aktifkan keluaran diagnostik
  Serial.println();

  //Pengaturan konfigurasi video
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);  //UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA  Tetapkan resolusi gambar awal

  //flash
  ledcAttachPin(4, 4);  
  ledcSetup(4, 5000, 8);    
  
  WiFi.mode(WIFI_AP_STA);
  
  //Tentukan IP statis klien
  //WiFi.config(IPAddress(192, 168, 201, 100), IPAddress(192, 168, 201, 2), IPAddress(255, 255, 255, 0));

  WiFi.begin(ssid, password);    //Lakukan koneksi jaringan

  delay(1000);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  long int StartTime=millis();
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      if ((StartTime+10000) < millis()) break;    //等待10秒連線
  } 

  if (WiFi.status() == WL_CONNECTED) {    //若連線成功
    WiFi.softAP((WiFi.localIP().toString()+"_"+(String)apssid).c_str(), appassword);   //設定SSID顯示客戶端IP         
    Serial.println("");
    Serial.println("STAIP address: ");
    Serial.println(WiFi.localIP());  

    for (int i=0;i<5;i++) {   //若連上WIFI設定閃光燈快速閃爍
      ledcWrite(4,10);
      delay(200);
      ledcWrite(4,0);
      delay(200);    
    }         
  }
  else {
    WiFi.softAP((WiFi.softAPIP().toString()+"_"+(String)apssid).c_str(), appassword);         

    for (int i=0;i<2;i++) {    //若連不上WIFI設定閃光燈慢速閃爍
      ledcWrite(4,10);
      delay(1000);
      ledcWrite(4,0);
      delay(1000);    
    }
  }     

  //指定AP端IP    
  //WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0)); 
  Serial.println("");
  Serial.println("APIP address: ");
  Serial.println(WiFi.softAPIP());    

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);  

  server.begin();          
}

//自訂網頁首頁管理介面
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
  <!DOCTYPE html>
  <head>
  <title>tracking.js - color with camera</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <script src="https:\/\/ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js"></script>
  <script src="https:\/\/fustyles.github.io/webduino/Tracking_20190917/tracking-min.js"></script>  
  </head><body>
  <img id="ShowImage" src="" style="display:none">
  <canvas id="canvas" width="0" height="0"></canvas><canvas id="canvas_custom" style="display:none"></canvas>
  <table style="border:0px">
  <tr style="border:0px">
    <td><input type="button" id="restart" value="Restart"></td> 
    <td colspan="2"><input type="button" id="getStill" value="Start Detection"><input type="checkbox" id="showpix" value="Show Pixel" onclick="if (this.checked) canvas_custom.style.display='block'; else canvas_custom.style.display='none';">Show Pixel</td>
  </tr>
  <tr style="border:0px">
    <td style="text-align:center;background-color:red;"><a style="color:white;" onclick="changeTab('red');">Rect (Red)</a>
    <td style="text-align:center;background-color:green;"><a style="color:white" onclick="changeTab('green');">Rect (Green)</a></td>
    <td style="text-align:center;background-color:blue;"><a style="color:white" onclick="changeTab('blue');">Rect (Blue)</a></td>
  </td>
  </tr>
  <tr>
    <td colspan="3">
    <div id="divColor1">
      <table style="border:4px red solid;width:100%;">
      <tr>
        <td align="right">R</td>
        <td colspan="2">min<input type="range" id="myColor_r_min1" min="0" max="255" value="61" step="1" onchange="myColor_r_min_v1.innerHTML=this.value;"><span id="myColor_r_min_v1">61</span><br>
          max<input type="range" id="myColor_r_max1" min="0" max="255" value="255" step="1" onchange="myColor_r_max_v1.innerHTML=this.value;"><span id="myColor_r_max_v1">255</span>
        </td>
      </tr>
      <tr>
        <td align="right">G</td>
        <td colspan="2">min<input type="range" id="myColor_g_min1" min="0" max="255" value="0" step="1" onchange="myColor_g_min_v1.innerHTML=this.value;"><span id="myColor_g_min_v1">0</span><br>
          max<input type="range" id="myColor_g_max1" min="0" max="255" value="57" step="1" onchange="myColor_g_max_v1.innerHTML=this.value;"><span id="myColor_g_max_v1">57</span>
        </td>
      </tr>
      <tr>
        <td align="right">B</td>
        <td colspan="2">min<input type="range" id="myColor_b_min1" min="0" max="255" value="0" step="1" onchange="myColor_b_min_v1.innerHTML=this.value;"><span id="myColor_b_min_v1">0</span><br>
          max<input type="range" id="myColor_b_max1" min="0" max="255" value="39" step="1" onchange="myColor_b_max_v1.innerHTML=this.value;"><span id="myColor_b_max_v1">39</span>
        </td>
      </tr>
      </table>
    </div>
    <div id="divColor2" style="display:none">
      <table style="border:4px green solid;width:100%;">
      <tr>
        <td align="right">R</td>
        <td colspan="2">min<input type="range" id="myColor_r_min2" min="0" max="255" value="0" step="1" onchange="myColor_r_min_v2.innerHTML=this.value;"><span id="myColor_r_min_v2">0</span><br>
          max<input type="range" id="myColor_r_max2" min="0" max="255" value="0" step="1" onchange="myColor_r_max_v2.innerHTML=this.value;"><span id="myColor_r_max_v2">0</span>
        </td>
      </tr>
      <tr>
        <td align="right">G</td>
        <td colspan="2">min<input type="range" id="myColor_g_min2" min="0" max="255" value="0" step="1" onchange="myColor_g_min_v2.innerHTML=this.value;"><span id="myColor_g_min_v2">0</span><br>
          max<input type="range" id="myColor_g_max2" min="0" max="255" value="0" step="1" onchange="myColor_g_max_v2.innerHTML=this.value;"><span id="myColor_g_max_v2">0</span>
        </td>
      </tr>
      <tr>
        <td align="right">B</td>
        <td colspan="2">min<input type="range" id="myColor_b_min2" min="0" max="255" value="0" step="1" onchange="myColor_b_min_v2.innerHTML=this.value;"><span id="myColor_b_min_v2">0</span><br>
          max<input type="range" id="myColor_b_max2" min="0" max="255" value="0" step="1" onchange="myColor_b_max_v2.innerHTML=this.value;"><span id="myColor_b_max_v2">0</span>
        </td>
      </tr>
      </table>
    </div>
    <div id="divColor3" style="display:none">
      <table style="border:4px blue solid;width:100%;">
      <tr>
        <td align="right">R</td>
        <td colspan="2">min<input type="range" id="myColor_r_min3" min="0" max="255" value="0" step="1" onchange="myColor_r_min_v3.innerHTML=this.value;"><span id="myColor_r_min_v3">0</span><br>
          max<input type="range" id="myColor_r_max3" min="0" max="255" value="0" step="1" onchange="myColor_r_max_v3.innerHTML=this.value;"><span id="myColor_r_max_v3">0</span>
        </td>
      </tr>
      <tr>
        <td align="right">G</td>
        <td colspan="2">min<input type="range" id="myColor_g_min3" min="0" max="255" value="0" step="1" onchange="myColor_g_min_v3.innerHTML=this.value;"><span id="myColor_g_min_v3">0</span><br>
          max<input type="range" id="myColor_g_max3" min="0" max="255" value="0" step="1" onchange="myColor_g_max_v3.innerHTML=this.value;"><span id="myColor_g_max_v3">0</span>
        </td>
      </tr>
      <tr>
        <td align="right">B</td>
        <td colspan="2">min<input type="range" id="myColor_b_min3" min="0" max="255" value="0" step="1" onchange="myColor_b_min_v3.innerHTML=this.value;"><span id="myColor_b_min_v3">0</span><br>
          max<input type="range" id="myColor_b_max3" min="0" max="255" value="0" step="1" onchange="myColor_b_max_v3.innerHTML=this.value;"><span id="myColor_b_max_v3">0</span>
        </td>
      </tr>
      </table>
    </div>
    </td>
  </tr>
  <tr>
    <td>Flash</td>
    <td colspan="2"><input type="range" id="flash" min="0" max="255" value="0"></td>
  </tr>
  <tr>
    <td>Quality</td>
    <td colspan="2"><input type="range" id="quality" min="10" max="63" value="10"></td>
  </tr>
  <tr>
    <td>Brightness</td>
    <td colspan="2"><input type="range" id="brightness" min="-2" max="2" value="0"></td>
  </tr>
  <tr>
    <td>Contrast</td>
    <td colspan="2"><input type="range" id="contrast" min="-2" max="2" value="0"></td>
  </tr>
  <tr>
    <td>Resolution</td> 
    <td colspan="2">
      <select id="framesize">
        <option value="UXGA">UXGA(1600x1200)</option>
        <option value="SXGA">SXGA(1280x1024)</option>
        <option value="XGA">XGA(1024x768)</option>
        <option value="SVGA">SVGA(800x600)</option>
        <option value="VGA">VGA(640x480)</option>
        <option value="CIF">CIF(400x296)</option>
        <option value="QVGA" selected="selected">QVGA(320x240)</option>
        <option value="HQVGA">HQVGA(240x176)</option>
        <option value="QQVGA">QQVGA(160x120)</option>
      </select> 
    </td>
  </tr>  
  <tr>
    <td>MirrorImage</td> 
    <td colspan="2">  
      <select id="mirrorimage">
        <option value="1">Y</option>
        <option value="0">N</option>
      </select>
    </td>
  </tr>     
  <tr>
    <td>Rotate</td>
    <td align="left" colspan="2">
        <select onchange="document.getElementById('canvas').style.transform='rotate('+this.value+')';">
          <option value="0deg">0deg</option>
          <option value="90deg">90deg</option>
          <option value="180deg">180deg</option>
          <option value="270deg">270deg</option>
        </select>
    </td>
  </tr> 
  </table>
  <div id="result" style="color:red;display:none"></div>
  <div id="red" style="color:red"></div>
  <div id="green" style="color:green"></div>
  <div id="blue" style="color:blue"></div>   
  <div id="magenta" style="color:magenta"></div>
  <div id="cyan" style="color:cyan"></div>
  <div id="yellow" style="color:yellow"></div>      
  </body>
  </html> 
  
  <script>
    var getStill = document.getElementById('getStill');
    var ShowImage = document.getElementById('ShowImage');
    var canvas = document.getElementById("canvas");
    var context = canvas.getContext("2d"); 
    var canvas_custom = document.getElementById('canvas_custom');
    var context_custom = canvas_custom.getContext('2d');    
    var myColor = document.getElementById('myColor');
    var mirrorimage = document.getElementById("mirrorimage");   
    var result = document.getElementById('result');
    var red = document.getElementById('red');
    var green = document.getElementById('green');
    var blue = document.getElementById('blue');
    var magenta = document.getElementById('magenta');
    var cyan = document.getElementById('cyan');
    var yellow = document.getElementById('yellow');
    var flash = document.getElementById('flash');
    var myTimer;
    var restartCount=0;  
    var myColor_r_min1,myColor_r_max1,myColor_g_min1,myColor_g_max1,myColor_b_min1,myColor_b_max1;
    var myColor_r_min2,myColor_r_max2,myColor_g_min2,myColor_g_max2,myColor_b_min2,myColor_b_max2;
    var myColor_r_min3,myColor_r_max3,myColor_g_min3,myColor_g_max3,myColor_b_min3,myColor_b_max3;

    var tracker = new tracking.ColorTracker();
  
    tracking.ColorTracker.registerColor('red', function(r, g, b) {
      if ((r>=myColor_r_min1&&r<=myColor_r_max1)&&(g>=myColor_g_min1&&g<=myColor_g_max1)&&(b>=myColor_b_min1&&b<=myColor_b_max1)) {
        return true;
      }
      return false;
    });
  
    tracking.ColorTracker.registerColor('green', function(r, g, b) {
      if ((r>=myColor_r_min2&&r<=myColor_r_max2)&&(g>=myColor_g_min2&&g<=myColor_g_max2)&&(b>=myColor_b_min2&&b<=myColor_b_max2)) {
        return true;
      }
      return false;
    });
  
    tracking.ColorTracker.registerColor('blue', function(r, g, b) {
      if ((r>=myColor_r_min3&&r<=myColor_r_max3)&&(g>=myColor_g_min3&&g<=myColor_g_max3)&&(b>=myColor_b_min3&&b<=myColor_b_max3)) {
        return true;
      }
      return false;
    });
  
    var trackedColors = {
      custom: true
    };
  
    Object.keys(tracking.ColorTracker.knownColors_).forEach(function(color) {
      trackedColors[color] = true;
    });
  
    var colors = [];
    for (var color in trackedColors) {
      if (trackedColors[color]) {
      colors.push(color);
      }
    }
    tracker.setColors(colors);
  
    function changeTab(tab) {
      if (tab=='red') divColor1.style.display="block"; else divColor1.style.display="none";
      if (tab=='green') divColor2.style.display="block"; else divColor2.style.display="none";
      if (tab=='blue') divColor3.style.display="block"; else divColor3.style.display="none";
    }   

    getStill.onclick = function (event) {  
      clearInterval(myTimer);  
      myTimer = setInterval(function(){error_handle();},5000);
      ShowImage.src=location.origin+'/?getstill='+Math.random();      
    }

    function error_handle() {
      restartCount++;
      clearInterval(myTimer);
      if (restartCount<=2) {
        result.innerHTML = "Get still error. <br>Restart ESP32-CAM "+restartCount+" times.";
        myTimer = setInterval(function(){getStill.click();},10000);
        //$.ajax({url: document.location.origin+'?restart', async: false});
      }
      else
        result.innerHTML = "Get still error. <br>Please close the page and check ESP32-CAM.";
    }  

    ShowImage.onload = function (event) {
      clearInterval(myTimer);
      restartCount=0;      
      canvas.setAttribute("width", ShowImage.width);
      canvas.setAttribute("height", ShowImage.height);
      canvas_custom.setAttribute("width", ShowImage.width);
      canvas_custom.setAttribute("height", ShowImage.height);      
      
      if (mirrorimage.value==1) {
        context.translate((canvas.width + ShowImage.width) / 2, 0);
        context.scale(-1, 1);
        context.drawImage(ShowImage, 0, 0, ShowImage.width, ShowImage.height);
        context.setTransform(1, 0, 0, 1, 0, 0);
      }
      else
        context.drawImage(ShowImage,0,0,ShowImage.width,ShowImage.height);

      myColor_r_min1 = document.getElementById('myColor_r_min1').value;
      myColor_r_max1 = document.getElementById('myColor_r_max1').value;
      myColor_g_min1 = document.getElementById('myColor_g_min1').value;
      myColor_g_max1 = document.getElementById('myColor_g_max1').value;
      myColor_b_min1 = document.getElementById('myColor_b_min1').value;
      myColor_b_max1 = document.getElementById('myColor_b_max1').value;

      myColor_r_min2 = document.getElementById('myColor_r_min2').value;
      myColor_r_max2 = document.getElementById('myColor_r_max2').value;
      myColor_g_min2 = document.getElementById('myColor_g_min2').value;
      myColor_g_max2 = document.getElementById('myColor_g_max2').value;
      myColor_b_min2 = document.getElementById('myColor_b_min2').value;
      myColor_b_max2 = document.getElementById('myColor_b_max2').value;

      myColor_r_min3 = document.getElementById('myColor_r_min3').value;
      myColor_r_max3 = document.getElementById('myColor_r_max3').value;
      myColor_g_min3 = document.getElementById('myColor_g_min3').value;
      myColor_g_max3 = document.getElementById('myColor_g_max3').value;
      myColor_b_min3 = document.getElementById('myColor_b_min3').value;
      myColor_b_max3 = document.getElementById('myColor_b_max3').value; 
             
      var imgData=context.getImageData(0,0,canvas.width,canvas.height);
      for (var i=0;i<imgData.data.length;i+=4) {
        var r=0;
        var g=0;
        var b=0;
        if ((imgData.data[i]>=myColor_r_min1&&imgData.data[i]<=myColor_r_max1)&&(imgData.data[i+1]>=myColor_g_min1&&imgData.data[i+1]<=myColor_g_max1)&&(imgData.data[i+2]>=myColor_b_min1&&imgData.data[i+2]<=myColor_b_max1)) {
          r=255;
        }
        if ((imgData.data[i]>=myColor_r_min2&&imgData.data[i]<=myColor_r_max2)&&(imgData.data[i+1]>=myColor_g_min2&&imgData.data[i+1]<=myColor_g_max2)&&(imgData.data[i+2]>=myColor_b_min2&&imgData.data[i+2]<=myColor_b_max2)) {
          g=255;
        }
        if ((imgData.data[i]>=myColor_r_min3&&imgData.data[i]<=myColor_r_max3)&&(imgData.data[i+1]>=myColor_g_min3&&imgData.data[i+1]<=myColor_g_max3)&&(imgData.data[i+2]>=myColor_b_min3&&imgData.data[i+2]<=myColor_b_max3)) {
          b=255;
        }
  
        imgData.data[i]=r;
        imgData.data[i+1]=g;
        imgData.data[i+2]=b;
        imgData.data[i+3]=255;
      }
      context_custom.putImageData(imgData,0,0);
  
      tracking.track('#canvas', tracker);
      
      try { 
        document.createEvent("TouchEvent");
        setTimeout(function(){getStill.click();},250);
      }
      catch(e) { 
        setTimeout(function(){getStill.click();},150);
      }             
    } 

    tracker.on('track', function(event) {
      result.innerHTML = "";
      red.innerHTML = "";
      green.innerHTML = "";
      blue.innerHTML = "";
      magenta.innerHTML = "";
      cyan.innerHTML = "";
      yellow.innerHTML = "";
            
      event.data.forEach(function(rect) {
        context.strokeStyle = rect.color;
        context.strokeRect(rect.x, rect.y, rect.width, rect.height);
        //context.font = '11px Helvetica';
        //context.fillStyle = "#fff";
        //context.fillText('x: ' + rect.x + 'px', rect.x + rect.width + 5, rect.y + 11);
        //context.fillText('y: ' + rect.y + 'px', rect.x + rect.width + 5, rect.y + 22);

        //回傳辨識結果，網址後要加 ";stop" 可加快立即斷線減少執行等待時間
        $.ajax({url:document.location.origin+'?serial='+rect.color+";"+rect.x+";"+rect.y+";"+rect.width+";"+rect.height+';stop', async: false});
        
        result.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+"<br>";
        if (rect.color=="red") {
          red.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+";";
        }
        else if (rect.color=="green") {
          green.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+";";
        }
        else if (rect.color=="blue") {
          blue.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+";";
        }
        else if (rect.color=="magenta") {
          magenta.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+";";
        }
        else if (rect.color=="cyan") {
          cyan.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+";";
        } 
        else if (rect.color=="yellow") {
          yellow.innerHTML+= rect.color+","+rect.x+","+rect.y+","+rect.width+","+rect.height+";";
        }           
      });
    });
               
    restart.onclick = function (event) {
      fetch(location.origin+'?restart=stop');
    }    

    framesize.onclick = function (event) {
      fetch(document.location.origin+'?framesize='+this.value+';stop');
    }  

    flash.onchange = function (event) {
      fetch(location.origin+'?flash='+this.value+';stop');
    } 

    quality.onclick = function (event) {
      fetch(document.location.origin+'?quality='+this.value+';stop');
    } 

    brightness.onclick = function (event) {
      fetch(document.location.origin+'?brightness='+this.value+';stop');
    } 

    contrast.onclick = function (event) {
      fetch(document.location.origin+'?contrast='+this.value+';stop');
    }                             
    
    function getFeedback(target) {
      var data = $.ajax({
      type: "get",
      dataType: "text",
      url: target,
      success: function(response)
        {
          result.innerHTML = response;
        },
        error: function(exception)
        {
          result.innerHTML = 'fail';
        }
      });
    }      
  </script>   
)rawliteral";



void loop() {
  Feedback="";Command="";cmd="";P1="";P2="";P3="";P4="";P5="";P6="";P7="";P8="";P9="";
  ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;
  
   WiFiClient client = server.available();

  if (client) { 
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();             
        
        getCommand(c);   //將緩衝區取得的字元拆解出指令參數
                
        if (c == '\n') {
          if (currentLine.length() == 0) {    
            
            if (cmd=="getstill") {
              //回傳JPEG格式影像
              camera_fb_t * fb = NULL;
              fb = esp_camera_fb_get();  
              if(!fb) {
                Serial.println("Camera capture failed");
                delay(1000);
                ESP.restart();
              }
  
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Origin: *");              
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: image/jpeg");
              client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\""); 
              client.println("Content-Length: " + String(fb->len));             
              client.println("Connection: close");
              client.println();
              
              uint8_t *fbBuf = fb->buf;
              size_t fbLen = fb->len;
              for (size_t n=0;n<fbLen;n=n+1024) {
                if (n+1024<fbLen) {
                  client.write(fbBuf, 1024);
                  fbBuf += 1024;
                }
                else if (fbLen%1024>0) {
                  size_t remainder = fbLen%1024;
                  client.write(fbBuf, remainder);
                }
              }  
              
              esp_camera_fb_return(fb);
            
              pinMode(4, OUTPUT);
              digitalWrite(4, LOW);               
            }
            else {
              //回傳HTML首頁或Feedback
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: text/html; charset=utf-8");
              client.println("Access-Control-Allow-Origin: *");
              client.println("Connection: close");
              client.println();
              
              String Data="";
              if (cmd!="")
                Data = Feedback;
              else {
                Data = String((const char *)INDEX_HTML);
              }
              int Index;
              for (Index = 0; Index < Data.length(); Index = Index+1000) {
                client.print(Data.substring(Index, Index+1000));
              }           
              
              client.println();
            }
                        
            Feedback="";
            break;
          } else {
            currentLine = "";
          }
        } 
        else if (c != '\r') {
          currentLine += c;
        }

        if ((currentLine.indexOf("/?")!=-1)&&(currentLine.indexOf(" HTTP")!=-1)) {
          if (Command.indexOf("stop")!=-1) {  //若指令中含關鍵字stop立即斷線 -> http://192.168.xxx.xxx/?cmd=aaa;bbb;ccc;stop
            client.println();
            client.println();
            client.stop();
          }
          currentLine="";
          Feedback="";
          ExecuteCommand();
        }
      }
    }
    delay(1);
    client.stop();
  }
}

//拆解命令字串置入變數
void getCommand(char c)
{
  if (c=='?') ReceiveState=1;
  if ((c==' ')||(c=='\r')||(c=='\n')) ReceiveState=0;
  
  if (ReceiveState==1)
  {
    Command=Command+String(c);
    
    if (c=='=') cmdState=0;
    if (c==';') strState++;
  
    if ((cmdState==1)&&((c!='?')||(questionstate==1))) cmd=cmd+String(c);
    if ((cmdState==0)&&(strState==1)&&((c!='=')||(equalstate==1))) P1=P1+String(c);
    if ((cmdState==0)&&(strState==2)&&(c!=';')) P2=P2+String(c);
    if ((cmdState==0)&&(strState==3)&&(c!=';')) P3=P3+String(c);
    if ((cmdState==0)&&(strState==4)&&(c!=';')) P4=P4+String(c);
    if ((cmdState==0)&&(strState==5)&&(c!=';')) P5=P5+String(c);
    if ((cmdState==0)&&(strState==6)&&(c!=';')) P6=P6+String(c);
    if ((cmdState==0)&&(strState==7)&&(c!=';')) P7=P7+String(c);
    if ((cmdState==0)&&(strState==8)&&(c!=';')) P8=P8+String(c);
    if ((cmdState==0)&&(strState>=9)&&((c!=';')||(semicolonstate==1))) P9=P9+String(c);
    
    if (c=='?') questionstate=1;
    if (c=='=') equalstate=1;
    if ((strState>=9)&&(c==';')) semicolonstate=1;
  }
}
