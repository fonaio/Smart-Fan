#include <Arduino.h> 
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <DHT.h>

#define IR_LED_PIN 1
#define DHT_PIN 4

const int CHECK_TEMP_INTERVAL = 30000;
const int CARRIER_FREQUENCY = 38;

//variables for temperature bounds
const float TEMP_TURN_OFF = 20.0; 
const float TEMP_TURN_ON = 22.0;

//variables for cooldown time before checking temperature again
unsigned long lastToggleTime = 0;
const unsigned long COOLDOWN_PERIOD = 900000;
float lastRawTemp = 21.0;
//[idea: add smoothing? to make sure the temperature doesn't just flicker on/off?? ]

const uint16_t POWER_RAW[] = {
  1250, 450,
  1250, 400, 400, 1250, 1250, 450, 1200, 450,
  400, 1200, 450, 1250, 450, 1200, 450, 1200,
  450, 1200, 450, 1200, 1300, 7400, 1250, 450,
  1200, 450, 400, 1250, 1250, 400, 1250, 450,
  350, 1250, 450, 1200, 450, 1250, 450, 1200,
  450, 1200, 450, 1200, 1250, 7450, 1250, 400,
  1250, 450, 400, 1200, 1250, 450, 1250, 400,
  400, 1250, 400, 1250, 450, 1200, 450, 1200,
  500, 1150, 500, 1200, 1250, 7400, 1250, 450,
  1250, 400, 400, 1250, 1250, 450, 1200, 450,
  400, 1200, 450, 1250, 450, 1200, 450, 1200,
  450, 1200, 450, 1200, 1250, 7450, 1250, 450,
  1200, 450, 400, 1250, 1250, 400, 1250, 400,
  400, 1250, 450, 1200, 450, 1200, 500, 1200,
  450, 1200, 450, 1200, 1250
};

const uint16_t POWER_RAW_LEN = sizeof(POWER_RAW) / sizeof(POWER_RAW[0]);

IRsend irsend(IR_LED_PIN);
DHT dht(DHT_PIN, DHT11);

void sendPower() {
  //sends singal to change the power state
  Serial.println("IR is sending POWER");
  irsend.sendRaw(POWER_RAW, POWER_RAW_LEN, CARRIER_FREQUENCY);
  delay(200);
}
 
void setup(){
  Serial.begin(115200);
  irsend.begin();
  dht.begin();

  float firstReading = dht.readTemperature()
  if (!isnan(firstReading)){
    lastRawTemp = firstReading
  }
}

void loop(){
  //if we recieve an IR signal from the physical remote
  if (irrev.decode(&results)) {
    //Speed up signal recieved from physical remote
    if (resultToSourceCode(&results).indexOf("SPEED_UP_RAW") != 1) {
      if (currentSpeed == 9){
        currentSpeed = 1;
        Serial.println("Physical remote detected. Speed is now: " + currentSpeed);
      } 
      else {
        currentSpeed ++;
        Serial.println("Physical remote detected. Speed is now: " + currentSpeed);
      }
  }
}
