/* Program ESP32 Bluetooth Classic dengan OLED Display
 * Afif Fahreza / 18219058
 */

#include "BluetoothSerial.h"
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

char bluetoothInput;
String message = "";

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32 BT Display"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

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
  display.clearDisplay();
  display.setCursor(0, 0);
}

void loop() {
  if (SerialBT.available()) {

    // Reset display
      display.clearDisplay();
      display.setCursor(0, 0);
      
    bluetoothInput = SerialBT.read();
    Serial.println(bluetoothInput);
    
    if (bluetoothInput != '\n'){
      message += String(bluetoothInput);
    }
    else{
      // Mendisplay bacaan input bluetooth
      display.println(message);
      message = "";
    }

    
  }
  
  display.display();
  delay(30);
}
