/**
 * Program BMP Web Server
 * Diadaptasi dari:
 * https://randomnerdtutorials.com/esp32-web-server-with-bme280-mini-weather-station/
 * Oleh: Afif Fahreza / 18219058
 */

// Import Library yang dibutuhkan
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// Variable sensor
Adafruit_BMP280 bmp; 

// Macro untuk menyimpan credentials wifi
#define SSID "04FS_d364b8"
#define PASSWORD "wlan2c9b47"

// Mengeset nomor port web server ke 80
WiFiServer server(80);

// Variable untuk menyimpan request HTTP
String header;

// Variable penyimpan current time
unsigned long currentTime = millis();
// Variable penyimpan previous time
unsigned long previousTime = 0; 
// Macro penyimpan waktu timeout, 2 detik
#define TIMEOUTTIME 2000

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
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address dan menyalakan web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// FUNGSI LOOP
void loop(){
  // Menunggu komunikasi dari client
  WiFiClient client = server.available();

  // Jika ada client yang connect
  if (client) {
    // Mencatat waktu
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    // Membuat string untuk menyimpan data dari client
    String currentLine = "";
    // Looping selama client connect
    while (client.connected() && currentTime - previousTime <= TIMEOUTTIME) {
      // Mencatat waktu
      currentTime = millis();
      // Jika ada byte yang bisa dibaca dari client
      // Membaca byte tersebut, print ke serial monitor
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        // Jika character enter
        if (c == '\n') {
          // Jika character enter 2x, HTTP request berakhir, kirim respon
          if (currentLine.length() == 0) {
            // Format kode respon dari respon HTTP, tipe, dan new line
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Style CSS
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #09b1db; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
            // Web Page Heading
            client.println("</style></head><body><h1>ESP32 dengan BME280</h1>");
            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
            client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.println(bmp.readTemperature());
            client.println(" *C</span></td></tr>");  
            client.println("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
            client.println(1.8 * bmp.readTemperature() + 32);
            client.println(" *F</span></td></tr>");       
            client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
            client.println(bmp.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Approx. Altitude</td><td><span class=\"sensor\">");
            client.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>"); 
            // Template dari sensor BME, pada BMP tidak terdapat humidity sensor
            /*
            client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
            client.println(bmp.readHumidity());
            client.println(" %</span></td></tr>");
            */
            client.println("</body></html>");
            
            // Respon akhir HTTP, line kosong
            client.println();
            // Keluar dari loop
            break;
          }
          // Jika ada newline, reset currentLine
          else {
            currentLine = "";
          }
        }
        // Jika mendapatkan selain carriage, simpan ke currentLine
        else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Mereset variable header
    header = "";
    // Menutup connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
