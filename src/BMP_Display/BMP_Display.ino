/* Kode Program OLED Display + Sensor BMP280
 * Diadaptasi dari https://github.com/adafruit/Adafruit_BMP280_Library dan
 * https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/
 * Dimodifikasi oleh: Afif Fahreza / 18219058
 */

// Import library display, sensor, dan wiring
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

// Lebar dan tinggi OLED display dalam pixel
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

// Deklarasi Variable sensor
Adafruit_BMP280 bmp;

// Deklarasi variable display yang terhubung secara I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Deklarasi variable penyimpan nilai sensor
float temp, pres, alt;

// Prosedur setup, dijalankan sekali saat pertama kali menyala
void setup() {

  // Memulai komunikasi serial
  Serial.begin(115200);

  // Memulai komunikasi I2C dengan display pada address 0x3C
  // Memberikan alert jika display gagal
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Alokasi display SSD1306 gagal!"));
    while (1);
  }

  // Memulai komunikasi I2C dengan sensor pada address 0x76
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

  // Delay 2 detik untuk memastikan semua komunikasi berhasil
  delay(2000);

  // Membersihkan display dan mengeset warna putih
  display.clearDisplay();
  display.setTextColor(WHITE);

  Serial.println(F("Setup selesai!"));

}

// Prosedur loop, dijalankan berulang setelah setup
void loop() {
  
  // Membaca input sensor dan menyimpan nilai ke variable
  temp = bmp.readTemperature();
  pres = bmp.readPressure();
  alt = bmp.readAltitude(1013.25);

  // Membersihkan display
  display.clearDisplay();

  // Mengeset size text
  display.setTextSize(1);

  // Display Temperature
  display.setCursor(0,10);
  display.print("Temperature: ");
  display.print(temp);
  display.print("*C");

  // Display Pressure
  display.setCursor(0,30);
  display.print("Pressure: ");
  display.print(pres);
  display.print("Pa");

  // Display Temperature
  display.setCursor(0,50);
  display.print("Altitude: ");
  display.print(alt);
  display.print("m");
  
  // Menampilkan display
  display.display();

  // Delay 2 detik
  delay(2000);

}
