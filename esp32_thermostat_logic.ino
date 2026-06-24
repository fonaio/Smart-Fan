//rework logic to see if the temperature still has not reached the desired amt we increase to max speed??
//i can override max_speed with the remote (ex go higher), but the LED cannot
//think of alternative architecture to go for
#include <Arduino.h> 
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <DHT.h>

#define IR_LED_PIN 1
#define IR_RECIEVE_PIN 5
#define DHT_PIN 4

const int CHECK_TEMP_INTERVAL = 30000;
const float TEMP_LOWER_BOUND = 20.0;
const float TEMP_UPPER_BOUND = 22.0;
const int CARRIER_FREQUENCY = 38;
const int MAX_SPEED = 6; 

bool fanOn = false;
int currentSpeed = 1;

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

const uint16_t SPEED_UP_RAW[] = {
  1250, 450,
  1200, 450, 400, 1200, 1300, 400, 1250, 400,
  400, 1250, 450, 1200, 450, 1200, 500, 1200,
  1250, 400, 400, 1250, 450
};

const uint16_t SPEED_DOWN_RAW[] = {
  1250, 400,
  1250, 450, 400, 1200, 1250, 450, 1250, 400,
  400, 1250, 400, 1250, 450, 1200, 450, 1200,
  450, 1250, 1250, 400, 400
};

const uint16_t POWER_RAW_LEN = sizeof(POWER_RAW) / sizeof(POWER_RAW[0]);
const uint16_t SPEED_UP_RAW_LEN = sizeof(SPEED_UP_RAW) / sizeof(SPEED_UP_RAW[0]);
const uint16_t SPEED_DOWN_RAW_LEN = sizeof(SPEED_DOWN_RAW) / sizeof(SPEED_DOWN_RAW[0]);

IRsend irsend(IR_LED_PIN);
IRrecv irrecv(IR_RECIEVE_PIN);
decode_results results;
DHT dht(DHT_PIN, DHT11);

void sendPower() {
  //sends signal to change the power state
  Serial.println("IR is sending POWER");
  irsend.sendRaw(POWER_RAW, POWER_RAW_LEN, CARRIER_FREQUENCY);
  delay(200);
}

void sendSpeedUp(){
  //Sends signal to increase the speed of the fan. Speed increases by 1 only if the current speed is less than TARGET_SPEED.
  Serial.println("IR is sending SPEED UP");
  irsend.sendRaw(SPEED_UP_RAW, SPEED_UP_RAW_LEN, 38);
  if (currentSpeed < MAX_SPEED){
    currentSpeed ++;
  }
  delay(200);
}

void sendSpeedDown(){
  //Sends signal to decrease the speed of the fan
  Serial.println("IR is sending SPEED DOWN");
  irsend.sendRaw(SPEED_DOWN_RAW, SPEED_DOWN_RAW_LEN, 38);
  if (currentSpeed > 1){
    currentSpeed --;
  }
  delay(200);
}

void turnfanOn(){
  Serial.println("Turning the fan ON");
  sendPower();
  delay(1500);
  fanOn = true;
}

void turnfanOff(){
  Serial.println("Turning the fan OFF");
  sendPower();
  delay(1500);
  fanOn = false;
}
 
void setup(){
  Serial.begin(115200);
  irsend.begin();
  irrecv.enableIRIn();
  dht.begin();
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
