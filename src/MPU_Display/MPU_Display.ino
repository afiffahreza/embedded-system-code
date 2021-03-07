/* Program OLED Sensor MPU6050
 *  Diadaptasi dari:
 *  https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
 *  Dimodifikasi oleh: Afif Fahreza / 18219058
 */


// Import Library yang dibutuhkan
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

// Variable sensor MPU6050
Adafruit_MPU6050 mpu;
// Variable Display OLED
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// Fungsi setup
void setup() {
  // Memulai komunikasi serial
  Serial.begin(115200);

  // Memulai komunikasi dengan MPU6050
  // memberikan pesan jika koneksi gagal
  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  // Memulai komunikasi dengan OLED pada address 0x3C
  // memberikan pesan jika koneksi gagal
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Setup display
  display.display();
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
}

void loop() {
  // Variable sensor
  sensors_event_t a, g, temp;
  // Membaca nilai sensor
  mpu.getEvent(&a, &g, &temp);

  // Reset display
  display.clearDisplay();
  display.setCursor(0, 0);

  // Mendisplay bacaan accelerometer
  display.println("Accelerometer - m/s^2");
  display.print(a.acceleration.x, 1);
  display.print(", ");
  display.print(a.acceleration.y, 1);
  display.print(", ");
  display.print(a.acceleration.z, 1);
  display.println("");

  // Mendisplay bacaan gyroscope
  display.println("Gyroscope - rps");
  display.print(g.gyro.x, 1);
  display.print(", ");
  display.print(g.gyro.y, 1);
  display.print(", ");
  display.print(g.gyro.z, 1);
  display.println("");

  display.display();
  delay(100);
}
