/* Program Sensor BMP280
 * Diadaptasi dari https://github.com/adafruit/Adafruit_BMP280_Library 
 * Dimodifikasi oleh: Afif Fahreza / 18219058
 */

// Import Library Wire untuk komunikasi dan BMP280 dari Adafruit untuk sensor
#include <Wire.h>
#include <Adafruit_BMP280.h>

// Variable sensor
Adafruit_BMP280 bmp; 

void setup() {
      // Memulai komunikasi serial
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));

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
}

void loop() {

      // Membaca temperatur
  Serial.print(F("Temperatur = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

      // Membaca tekanan
  Serial.print(F("Tekanan = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

      // Membaca ketinggian
  Serial.print(F("Perkiraan Ketinggian = "));
  Serial.print(bmp.readAltitude(1013.25));
  Serial.println(" m");

  Serial.println();
  delay(2000);
}
