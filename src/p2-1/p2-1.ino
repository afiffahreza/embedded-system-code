/* Program Analog Output + Switch
 * Diadaptasi dari
 * https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
 * https://www.arduino.cc/en/tutorial/switch
 * Diedit Oleh : Afif Fahreza / 18219058
 */

// Mengeset Nomor Pin pada ESP32
#define BUTTON_PIN 21
#define LED_PIN 5

// Frekuensi dan Resolusi PWM
#define FREQ_PWM 5000
#define RES 8

// Debounce Pengecekan Kondisi Push Button
#define DEBOUNCE 200

// Frekuensi Efek Breathing
#define LED_TIME 10

// Variable Channel PWM
int ledChannel;

// Variable untuk menyimpan kondisi push button
int buttonState, previousButtonState = LOW;

// Variable untuk status LED, 0 merah - 1 kuning
int ledState = 0;

// Variable pengatur brightness
int bright = 0;
bool up = true;

// Variable penyimpan waktu
long int timerButton = 0, timerLED = 0;

void setup() { 
  // Inisialisasi pin push button sebagai input
  pinMode(BUTTON_PIN, INPUT);
  // Konfigurasi Fungsi PWM LED
  ledcSetup(ledChannel, FREQ_PWM, RES);
  // Menyambungkan Channel PWM dengan Pin LED
  ledcAttachPin(LED_PIN, ledChannel);
}

void loop() {
  // Membaca kondisi dari push button
  // dan disimpan ke variable buttonState
  buttonState = digitalRead(BUTTON_PIN);
  
  // Switch Push Button
  // Pengecekan input yang berubah dari LOW ke HIGH 
  // dengan waktu lebih dari DEBOUNCE untuk menangani noise
  if (buttonState==HIGH && previousButtonState == LOW &&
      millis() - timerButton >= DEBOUNCE){
    if (ledState == 0) ledState = 1;
    else ledState = 0;
    timerButton = millis();
  }

  // Efek breathing LED

  if (ledState == 1){
    // Lampu merah
    ledcWrite(ledChannel, bright);
  }
  else {
    ledcWrite(ledChannel, 0);
  }

  if (millis() - timerLED >= LED_TIME){
    if (up) bright++;
    else bright--;
    if (bright==255) up = false;
    if (bright==0) up = true;
    timerLED = millis();
  }
  
}
