#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <DHT.h>

#define IR_LED_PIN      1
#define IR_RECIEVE_PIN 5
#define DHT_PIN         4

const int CHECK_TEMP_INTERVAL = 30000;
const int TARGET_SPEED = 4;
const float TEMP_LOWER_BOUND = 20.0;
const float TEMP_UPPER_BOUND = 22.0;
const int CARRIER_FREQUENCY = 38;

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
DHT    dht(DHT_PIN, DHT11);

void sendPower() {
  //sends singal to change the power state
  Serial.println("IR is sending POWER");
  irsend.sendRaw(POWER_RAW, POWER_RAW_LEN, CARRIER_FREQUENCY);
  delay(200);
}

void sendSpeedUp(){
  //Sends signal to increase the speed of the fan. Speed increases by 1 only if the current speed is less than 4.
  Serial.println("IR is sending SPEED UP");
  irsend.sendRaw(SPEED_UP_RAW, SPEED_UP_RAW_LEN, 38);
  if (currentSpeed < 4){
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

void setFanSpeed(){
  //sets the speed of the fan to TARGET_SPEED
  Serial.println("Setting the fan speed to " + String(TARGET_SPEED));
  while (currentSpeed < TARGET_SPEED){
    sendSpeedUp();
    delay(500);
  }
  while (currentSpeed > TARGET_SPEED){
    sendSpeedDown();
    delay(500);
  }
}

void turnfanOn(){
  Serial.println("Turning the fan ON");
  sendPower();
  delay(1500);
  setFanSpeed();
  fanOn = true;
}

void turnfanOff(){
  Serial.println("Turning the fan OFF");
  sendPower();
  delay(1500);
  fanOn = false;
}
void setup() {

}

void loop() {
  // put your main code here, to run repeatedly:

}
