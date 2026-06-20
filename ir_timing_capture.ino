#include <IRremote.hpp>
#define IR_RECEIVE_PIN 11

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    IrReceiver.begin(IR_RECIEVE_PIN, ENABLE_LED_FEEDBACK) 
    //Starts listening for a signal on the dedicated pin. If we recieve a signal, the Arduino LED should flash
    Serial.println("Setup ready. Press a button on the remote to record IR signal")
  }
}

void loop() { //Taken from IRremote library
 if (IrReceiver.decode()) {
    Serial.println("----------");
    Serial.print("Protocol: ");
    Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));
    Serial.print("Address: 0x");
    Serial.println(IrReceiver.decodedIRData.address, HEX);
    Serial.print("Command: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    Serial.println("Raw timing data:");
    IrReceiver.printIRResultRawFormatted(&Serial, true);

    IrReceiver.resume();
}
