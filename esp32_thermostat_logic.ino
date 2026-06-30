//new logic: 
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
//float lastRawTemp = 0.0; //removed and replaced with smoothing function

//Smoothing/moving average implementation
const int FILTER_SAMPLES = 10;     // Avg last 10 readings (100 seconds of data)
float tempSamples[FILTER_SAMPLES];
int sampleIndex = 0;
bool listFull = false;
float lastSmoothedTemp = 21.0;

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
 
float getSmoothedTemperature(float newReading) {
  tempSamples[sampleIndex] = newReading;
  sampleIndex++;
  
  if (sampleIndex >= FILTER_SAMPLES) {
    sampleIndex = 0;
    listFull = true;
  }

  int currentSampleCount;

  if (listFull) {
    currentSampleCount = FILTER_SAMPLES;
  }
  else {
    currentSampleCount = sampleIndex;
  }
  
  float sum = 0;
  for (int i = 0; i < currentSampleCount; i++) {
    sum += tempSamples[i];
  }
  
  return sum / currentSampleCount; //average reading
}

void setup(){
  Serial.begin(115200);
  irsend.begin();
  dht.begin();

  float firstReading = dht.readTemperature();
  if (!isnan(firstReading)){
    lastSmoothedTemp = firstReading;
  }
}

void loop() {
  unsigned long currentTime = millis();

  static unsigned long lastTempCheck = 0;
  if (currentTime - lastTempCheck >= CHECK_TEMP_INTERVAL) {
    lastTempCheck = currentTime;

    float rawTemp = dht.readTemperature();
    if (isnan(rawTemp)) {
      Serial.println("Skipping cycle, temperature reading is not a valid number");
      return; 
    }

    float currentSmoothedTemp = getSmoothedTemperature(rawTemp);

    Serial.print("Raw: " + String(rawTemp) + "°C | Smoothed: " + String(currentSmoothedTemp) + "°C");
    
    if (currentTime - lastToggleTime < COOLDOWN_PERIOD) { 
      //makes system stay the way it is for length of COOLDOWN_PERIOD before trying to change states again
      return; 
    }

    // Turn fan on
    if (currentSmoothedTemp >= TEMP_TURN_ON && lastSmoothedTemp < TEMP_TURN_ON) {
      Serial.println("Hot limit crossed. Toggling Power.");
      sendPower();
      lastToggleTime = currentTime; 
    } 
    // Turn fan off
    else if (currentSmoothedTemp <= TEMP_TURN_OFF && lastSmoothedTemp > TEMP_TURN_OFF) {
      Serial.println("Cold limit crossed. Toggling Power.");
      sendPower();
      lastToggleTime = currentTime;
    }
    lastSmoothedTemp = currentSmoothedTemp;
  }
}
