/* 

  Micro Processor  With Bluetooth module
  1. HC-05
  2. HC-06

  DEVICE_BAUD 8 : 115200

  Made by Dreamwalker
  Date : 2019-02-23


*/

#include "BluetoothATCommand.h"
#include <SoftwareSerial.h>

#define ARDUINO_UNO_USER
#define SET_BAUD_DEFAULT

#define SERIAL_BAUDRATE 115200


// 사용할 모듈에 따라 주석을 꼭 제거하세요 
// 사용하지 않을 모듈은 모두 주석처리해야 문제없이 컴파일 됩니다.
#define HC_05
#define HC_06

#ifdef ARDUINO_UNO_USER
#define MCU_RX 2 // Remember MCU RX connects to module TX and vice versa
#define MCU_TX 3
#define RST 4 // MCU pin to control module reset
#define DEVICE_BAUD 8

SoftwareSerial moduleSS = SoftwareSerial(MCU_RX, MCU_TX); // MCU RX, TX
SoftwareSerial *moduleSerial = &moduleSS;

#elif ESP32_USER

#define MCU_RX 16
#define MCU_TX 17
// Hardware serial is also possible!
// HardwareSerial *moduleSerial = &Serial1;
#endif

#ifdef HC_05
#define BLUETOOTH_BAUDRATE 38400
BluetoothATCommand module = BluetoothATCommand(RST, true);
#endif

#ifdef HC_06
#define BLUETOOTH_BAUDRATE 115200
BluetoothATCommand module = BluetoothATCommand(RST, false);
#endif


static int deviceNum = 10000;

void setup()
{
  String setNameCommand = "AT+NAME" + String("AS") + String(deviceNum);
  Serial.begin(SERIAL_BAUDRATE);
  moduleSerial->begin(BLUETOOTH_BAUDRATE);
  module.begin(*moduleSerial);

  module.reset(LOW, 10);

  module.sendBlindCommand("AT");
  delay(1000);

  if (!module.sendCommand("AT", 1000)) Serial.println(F("Command failed!"));
  delay(1000);

  if (!module.sendCommand("AT")) Serial.println(F("Command failed!"));
  delay(1000);

#ifdef HC_05
 if (!module.sendCommand("AT\r\n", "OK", 2000)) Serial.println(F("Command failed!"));
 if (!module.sendCommand("AT+VERSION?\r\n", "OK", 2000)) Serial.println(F("Command failed!"));
 if (!module.sendCommand("AT+ADDR?\r\n", "OK", 2000)) Serial.println(F("Command failed!"));
 if (!module.sendCommand("AT+NAME=setNameCommand.c_str()\r\n", "OK", 2000)) Serial.println(F("Command failed!"));
 if (!module.sendCommand("AT+UART=115200,0,0\r\n", "OK", 2000)) Serial.println(F("Command failed!"));

#elif HC_06
  if (!module.sendCommand("AT", "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+VERSION", "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand(setNameCommand.c_str(), "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+BAUD8", "OK", 2000)) Serial.println(F("Command failed!"));
  #endif
}

void loop()
{
  // Nothing here
}
