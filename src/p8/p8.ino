/*
 * ESP32 Web Server dengan LED Control
 * Diadaptasi dari:
 * https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
 * Oleh: Afif Fahreza / 18219058
 */

// Import library wifi
#include <WiFi.h>

// Macro untuk menyimpan credentials wifi
#define SSID "04FS_d364b8"
#define PASSWORD "wlan2c9b47"

// Macro untuk menyimpan pin LED pada ESP32
#define LED1 4
#define LED2 5

// Mengeset nomor port web server ke 80
WiFiServer server(80);

// Variable untuk menyimpan request HTTP
String header;

// Variable status LED
String LED1State = "off";
String LED2State = "off";

// Variable penyimpan current time
unsigned long currentTime = millis();
// Variable penyimpan previous time
unsigned long previousTime = 0; 
// Macro penyimpan waktu timeout, 2 detik
#define TIMEOUTTIME 2000

// Fungsi setup
void setup() {
  // Memulai komunikasi serial
  Serial.begin(115200);
  // Inisialisasi pin LED menjadi output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  // Mengeset output LED low / 0V
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

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

// Fungsi loop
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
            
            // Mengatur pin LED on/off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              LED1State = "on";
              digitalWrite(LED1, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              LED1State = "off";
              digitalWrite(LED1, LOW);
            } else if (header.indexOf("GET /6/on") >= 0) {
              Serial.println("GPIO 6 on");
              LED2State = "on";
              digitalWrite(LED2, HIGH);
            } else if (header.indexOf("GET /6/off") >= 0) {
              Serial.println("GPIO 6 off");
              LED2State = "off";
              digitalWrite(LED2, LOW);
            }
            
            // HTML Web Page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Style CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Heading Web Page
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Mendisplay status saat ini untuk LED1
            client.println("<p>GPIO 5 - State " + LED1State + "</p>");
            // Jika LED1 off, display ON button dan sebaliknya     
            if (LED1State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Mendisplay status saat ini untuk LED1
            client.println("<p>GPIO 6 - State " + LED2State + "</p>");
            // Jika LED1 off, display ON button dan sebaliknya       
            if (LED2State=="off") {
              client.println("<p><a href=\"/6/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/6/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
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
