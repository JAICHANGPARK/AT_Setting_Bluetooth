#include "Arduino.h"
#include "BluetoothATCommand.h"

BluetoothATCommand::BluetoothATCommand(int8_t rst, bool lineEnding) {
  pinMode(rst, OUTPUT);

  _rst = rst;
  mySerial = 0;
  newline = lineEnding;
}

BluetoothATCommand::begin(Stream &port) {
  mySerial = &port;
}

void BluetoothATCommand::reset(uint8_t pulse, uint16_t duration) {
  if (pulse == LOW) {
    digitalWrite(_rst, HIGH);
    delay(10);
    digitalWrite(_rst, LOW);
    delay(duration);
    digitalWrite(_rst, HIGH);
  }
  else if (pulse == HIGH) {
    digitalWrite(_rst, LOW);
    delay(10);
    digitalWrite(_rst, HIGH);
    delay(duration);
    digitalWrite(_rst, LOW);
  }
}

bool BluetoothATCommand::sendBlindCommand(char *command) {
  Serial.print(F("\t---> ")); Serial.println(command);
  if (newline) mySerial->println(command);
  else mySerial->print(command);
}

bool BluetoothATCommand::sendCommand(char *command, uint16_t timeout) {
  while(mySerial->available()) mySerial->read(); // Clear input buffer

  Serial.print(F("\t---> ")); Serial.println(command);
  if (newline) mySerial->println(command);
  else mySerial->print(command);

  bool gotReply = false;
  timer = millis();

  while (!gotReply && millis() - timer < timeout) {
  	if (mySerial->available()) gotReply = true;
  }

  if (gotReply) return true;
  else return false;
}

bool BluetoothATCommand::sendCommand(char *command, char *reply, uint16_t timeout) {
  while(mySerial->available()) mySerial->read(); // Clear input buffer

  Serial.print(F("\t---> ")); Serial.println(command);
  if (newline) mySerial->println(command);
  else mySerial->print(command);

  uint8_t idx = 0;
  bool replyMatch = false;
  timer = millis();

  while (!replyMatch && millis() - timer < timeout) {
  	if (mySerial->available()) {
  	  replybuffer[idx] = mySerial->read();
  	  idx++;
  	  // if (strstr(replybuffer, reply) != NULL) replyMatch = true; // Only checks if desired reply is inside replybuffer
  	  if (strcmp(replybuffer, reply) == 0) replyMatch = true; // This means the reply must start with the desired reply to be successful
  	}
  }

  Serial.print(F("\t<--- ")); Serial.println(replybuffer);
  memset(replybuffer, 0, sizeof(replybuffer));
  
  if (replyMatch) return true;
  else return false;
}
