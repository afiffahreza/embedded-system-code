/**
 * Program Database ESP32
 * Diadaptasi dari:
 * https://randomnerdtutorials.com/
 * Oleh: Afif Fahreza / 18219058
 */

// Import Library yang dibutuhkan
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// Variable sensor
Adafruit_BMP280 bmp; 

// Macro untuk menyimpan credentials wifi
#define SSID "04FS_d364b8"
#define PASSWORD "wlan2c9b47"

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://esp32-database-afif.000webhostapp.com/post-esp-data.php";

// API key dari php file
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BMP280";
String sensorLocation = "Rumah";

// FUNGSI SETUP
void setup() {
  Serial.begin(115200);
  bool status;

  // Memulai komunikasi dengan sensor pada address 0x76
  // Memberikan alert jika sensor tidak terdeteksi
  if (!bmp.begin(0x76)) {
    Serial.println(F("Tidak menemukan sensor BMP280, cek kembali kabel!"));
    while (1);
  }

  // Setting BMP280 default dari datasheet
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Mode operasi
                  Adafruit_BMP280::SAMPLING_X2,     // Oversampling temperatur
                  Adafruit_BMP280::SAMPLING_X16,    // Oversampling tekanan
                  Adafruit_BMP280::FILTER_X16,      // Filter
                  Adafruit_BMP280::STANDBY_MS_500); // Waktu standby

  // Connect ke WIFI dengan SSID dan password
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

// FUNGSI LOOP
void loop(){
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue
                          + "&sensor=" + sensorName
                          + "&location=" + sensorLocation
                          + "&value1=" + String(bmp.readTemperature())
                          + "&value2=" + String(bmp.readPressure() / 100.0F)
                          + "&value3=" + String(bmp.readAltitude(SEALEVELPRESSURE_HPA)) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}
