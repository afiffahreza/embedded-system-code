/*
 * Program Project 3 II2260 Sistem Embedded
 * Internal ESP32 Touch Sensor Combination Lock
 * Afif Fahreza / 18219058
 */

// Menentukan touch sensor mana yang digunakan sebagai lock
#define LOCK1 t2
#define LOCK2 t3

// Menentukan pin touch sensor dan LED
#define TOUCH1 27
#define TOUCH2 14
#define TOUCH3 12
#define TOUCH4 13
#define LEDCORRECT 4
#define LEDWRONG 5

// Menentukan lama waktu (ms) touch sensor harus disentuh
#define TOUCHPERIOD 1500
#define TOUCHTHRESHOLD 30
#define TOUCHINPUT 500

#define TR(i) touchRead(i->tpin)

long int tlast=0;
int touchCount = 0;

// Struktur Data TouchSensor
typedef struct{
    int tstate;
    int tfirston;
    int tpin;
    unsigned long int ttime;
} touchSensor;

// Setup touch sensor
touchSensor touchSensorSetup(int p){
    touchSensor tS;
    tS.tpin = p;
    tS.tstate = 0;
    tS.tfirston = 0;
    tS.ttime = 0;
    return tS;
}

void touchSensorON(touchSensor *t){
    if (t->tstate == 0) {
        if(t->tfirston == 0){
            t->tfirston = 1;
            t->ttime = millis();
        }
        else{
            if(millis()-t->ttime >= TOUCHPERIOD){
                t->tstate = 1;
                touchCount++;
            }
        }
    }
}
void touchSensorOFF(touchSensor *t){
    if (t->tstate == 1){
        touchCount--;
    }
    t->tfirston = 0;
    t->tstate = 0;
}

void touchSensorLoop(touchSensor *t){
    Serial.print(String(TR(t))+" ");
    if (touchRead(t->tpin)<TOUCHTHRESHOLD){
        touchSensorON(t);
    }
    else{
        touchSensorOFF(t);
    }
}

touchSensor t1, t2, t3, t4;

void setup(){

    // Memulai komunikasi serial
    Serial.begin(115200);

    // Setup touch sensor
    t1 = touchSensorSetup(TOUCH1);
    t2 = touchSensorSetup(TOUCH2);
    t3 = touchSensorSetup(TOUCH3);
    t4 = touchSensorSetup(TOUCH4);

    // Mengeset pin LED menjadi output
    pinMode(LEDCORRECT,OUTPUT);
    pinMode(LEDWRONG,OUTPUT);

    // Menunggu komunikasi serial selesai
    delay(1000);
    Serial.println("Setup Selesai!");
}

void loop(){
    
    if (millis()-tlast>=TOUCHINPUT){
        touchSensorLoop(&t1);
        touchSensorLoop(&t2);
        touchSensorLoop(&t3);
        touchSensorLoop(&t4);
        Serial.println();
        tlast = millis();
    }

    if (touchCount == 2){
        if (LOCK1.tstate == true and LOCK2.tstate == true){
            Serial.println("Kombinasi Benar!");
            digitalWrite(LEDCORRECT,HIGH);
        }
        else {
            Serial.println("Kombinasi Salah!");
            digitalWrite(LEDWRONG,HIGH);
        }
    }
    else {
        digitalWrite(LEDCORRECT,LOW);
        digitalWrite(LEDWRONG,LOW);
    }

}
