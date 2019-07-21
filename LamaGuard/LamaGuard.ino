#include "WiFi.h"
#include "esp_camera.h"
#include "ESPAsyncWebServer.h"
#include "index.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Replace with your network credentials
const char* ssid     = "LamaGuard";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer controlServer(80);

String readDHTTemperature() {
  return "23";
}
String readDHTHumidity() {
  return "79";
}

//Camera code start
#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
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

#else
#error "Camera model not selected"
#endif

void startCameraServer();



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Montserrat;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    .vertical-space{
      margin-top: 1.5em;
      margin-bottom: 1.5em;
    }
    .vertical-space-xl{
      margin-top: 4em;
      margin-bottom: 4em;
    }
    .vertical-space-l{
      margin-top: 3em;
      margin-bottom: 3em;
    }
    .control-button{
    width:2em;
    height:2em;
    border-radius:0.4em;
    background-color:#0058C5;
    color:white;
    font-size:2em;
    text-align:center;
    box-shadow: none;
    border: none;
    margin-bottom:0.1em;
    }
    .control-button-link{
    padding-left: 0.7em;
    padding-right: 0.7em;
    padding-top: 0.4em;
    padding-bottom: 0.4em;
    border-radius:0.4em;
    background-color:#0058C5;
    color:white;
    font-size:2em;
    text-align:center;
    text-decoration:none;
    font-weight:bold;
    line-height:2.16em
    }
  </style>
</head>
<body>
  <div style="">

    <h2 style="line-height:1.0em; margin-bottom:15; margin-top:20">TANKBOT</h2>
    <h3 style="line-height:0.0em; margin:0">ROBO.COM.CY</h3>
    
    <!--
    <div class="vertical-space-xl"></div>  
    <a id="BtnF" class="control-button-link">▲</a>  
    <div></div>
    <a class="control-button-link" href="/left">◀<a>  
    <a class="control-button-link" href="/forward">o</a>  
    <a class="control-button-link" href="/right">▶</a>  
    <div></div>
    <a class="control-button-link" href="/back">▼</a>  
  -->

    <div class="vertical-space-xl"></div>
    <button class="control-button" id="BtnF"></button>  
    <div></div>
    <button class="control-button" id="BtnL"></button>
    <button class="control-button" id="BtnLED" style="font-weight:bold"></button>
    <button class="control-button" id="BtnR"></button>
    <div></div>
    <button class="control-button" id="BtnB"></button>  
    
  </div>
  
</body>
<script>
  forward = function() {
    window.location.href = "/forward";
  }
  back = function() {
    window.location.href = "/back";
  }
  left = function() {
    window.location.href = "/left";
  }
  right = function() {
    window.location.href = "/right";
  }
  stopBot = function() {
    window.location.href = "/stop";
  }
  
  document.getElementById('BtnF').addEventListener('touchstart', forward);
  document.getElementById('BtnB').addEventListener('touchstart', back);
  document.getElementById('BtnL').addEventListener('touchstart', left);
  document.getElementById('BtnR').addEventListener('touchstart', right);
  
  document.getElementById('BtnF').addEventListener('touchend', stopBot);
  document.getElementById('BtnR').addEventListener('touchend', stopBot);
  document.getElementById('BtnL').addEventListener('touchend', stopBot);
  document.getElementById('BtnB').addEventListener('touchend', stopBot);
</script>
</html>
)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void displayData(String id){
  Serial.println(id);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  //Camera code start
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
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  //Camera code end
  
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  controlServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", MAIN_page);
  });
  
  controlServer.on("/readADC", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "13");
    });
    
  controlServer.on("/back", HTTP_GET, [](AsyncWebServerRequest *request){
    displayData("back");
    request->send(204);
    });

  // Start server
  controlServer.begin();
  //startCameraServer();
}
 
void loop(){
  delay(10000);
}
