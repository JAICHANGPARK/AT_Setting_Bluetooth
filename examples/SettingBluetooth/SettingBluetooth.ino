/*

  Micro Processor  With Bluetooth module
  Made by Dreamwalker
  Date : 2019-02-23

  1. HC-05
  2. HC-06

  DEVICE_BAUD
  1---------1200
  2---------2400
  3---------4800
  4---------9600
  5---------19200
  6---------38400
  7---------57600
  8---------115200

*/

/*---------------------------------------------------------------/
    Mode Setting  (사용하는 보드 및 모드 빼고 전부 주석처리)
    1. 프로세서 선택
    2. 블루투스 모듈 선택
    3. Baudrate Defalt 여부 선택

    Note :
    사용할 모듈에 따라 주석을 꼭 제거하세요
    사용하지 않을 모듈은 모두 주석처리해야 문제없이 컴파일 됩니다.
  ---------------------------------------------------------------*/


#define ARDUINO_UNO_USER
//#define ESP32_USER

//#define HC_05
#define HC_06

//#define SET_BAUD_DEFAULT
#define DEVICE_ID                 10000
/*----------------------------------------------------------------*/


#ifdef ARDUINO_UNO_USER
#include <SoftwareSerial.h>
#include "BluetoothATCommand.h"


#define MCU_RX                    2               // RX
#define MCU_TX                    3               // TX
#define RST                       4               // 블루투스 모듈 리셋을 위한핀.
#define DEVICE_BAUD               8

SoftwareSerial moduleSS = SoftwareSerial(MCU_RX, MCU_TX); // SoftwareSerial(RX, TX)
SoftwareSerial *moduleSerial = &moduleSS;

#ifdef HC_05
#ifdef SET_BAUD_DEFAULT
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          38400
#else
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          38400
BluetoothATCommand module = BluetoothATCommand(RST, true);
#endif
#endif

#ifdef HC_06
#ifdef SET_BAUD_DEFAULT
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          9600
#else
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          115200
BluetoothATCommand module = BluetoothATCommand(RST, false);
#endif
#endif

#endif


#ifdef ESP32_USER

#define MCU_RX2                     16
#define MCU_TX2                     17
#define RST                         4               // 블루투스 모듈 리셋을 위한핀.
#define DEVICE_BAUD                 8
// Hardware serial is also possible!
// HardwareSerial *moduleSerial = &Serial1;
HardwareSerial bluetoothSerial(2);
//HardwareSerial *moduleSerial = &Serial2;

#ifdef HC_05
#ifdef SET_BAUD_DEFAULT
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          38400
#else
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          38400
#endif
#endif

#ifdef HC_06
#ifdef SET_BAUD_DEFAULT
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          9600
#else
#define SERIAL_BAUDRATE             115200
#define BLUETOOTH_BAUDRATE          115200
#endif
#endif

#endif


static int deviceNum = 10000;
int deviceBaudrate = 115200;

#ifdef ESP32_USER
void sendCommandForEsp32(String command, long timeout) {

  while (bluetoothSerial.available()) bluetoothSerial.read(); //clear buffer
  uint8_t idx = 0;
  bool replyMatch = false;
  long timer = millis();
  char replybuffer[255] = {0,};
  bluetoothSerial.print(command);
  Serial.print(F("\t-->")); Serial.println(command);

  while (!replyMatch && (millis() - timer < 1000)) {
    if (bluetoothSerial.available()) {
      replybuffer[idx] = bluetoothSerial.read();
      idx++;
    }
  }
  Serial.print(F("\t <--- ")); Serial.println(replybuffer);
  memset(replybuffer, 0, sizeof(replybuffer));

}

#endif


void setup()
{

#ifdef HC_06
  String setNameCommand = "AT+NAME" + String("AS") + String(DEVICE_ID);
  String setBaudrateCommand = "AT+BAUD" + String(DEVICE_BAUD);
#endif

#ifdef HC_05
  String setNameCommand = "AT+NAME=" + String("AS") + String(DEVICE_ID) + "\r\n";
  String setBaudrateCommand = "AT+UART=" + String(deviceBaudrate) + ",0,0" + "\r\n";
#endif

#ifdef ARDUINO_UNO_USER
  Serial.begin(SERIAL_BAUDRATE);
  moduleSerial->begin(BLUETOOTH_BAUDRATE);
  module.begin(*moduleSerial);
  module.reset(LOW, 10);
#endif

#ifdef ESP32_USER
  Serial.begin(SERIAL_BAUDRATE);
  bluetoothSerial.begin(BLUETOOTH_BAUDRATE, SERIAL_8N1, MCU_RX2, MCU_TX2);

  //  moduleSerial->begin(BLUETOOTH_BAUDRATE, SERIAL_8N1, MCU_RX2, MCU_TX2);
  //  module.begin(*moduleSerial);
  //  module.reset(LOW, 10);
#endif


#ifdef ARDUINO_UNO_USER
  module.sendBlindCommand("AT");
  delay(1000);
  if (!module.sendCommand("AT", 1000)) Serial.println(F("Command failed!"));
  delay(1000);
  if (!module.sendCommand("AT")) Serial.println(F("Command failed!"));
  delay(1000);
#ifdef HC_05
  if (!module.sendCommand("AT\r\n", "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+VERSION?\r\n", "OK", 1000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+NAME?\r\n", "OK", 1000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+UART?\r\n", "OK", 1000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+ADDR?\r\n", "OK", 1000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand(setNameCommand.c_str(), "OK", 1000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+UART=115200,0,0\r\n", "OK", 1000)) Serial.println(F("Command failed!"));

  if (!module.sendCommand("AT+NAME?\r\n", "OK", 1000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+UART?\r\n", "OK", 1000)) Serial.println(F("Command failed!"));
#endif

#ifdef HC_06
  if (!module.sendCommand("AT", "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+VERSION", "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand(setNameCommand.c_str(), "OK", 2000)) Serial.println(F("Command failed!"));
  if (!module.sendCommand("AT+BAUD8", "OK", 2000)) Serial.println(F("Command failed!"));
#endif
#endif

#ifdef ESP32_USER
#ifdef HC_05
  sendCommandForEsp32("AT\r\n", 1000);
  sendCommandForEsp32("AT\r\n", 1000);
  sendCommandForEsp32("AT\r\n", 1000);
  sendCommandForEsp32("AT+VERSION?\r\n", 1000);
  sendCommandForEsp32("AT+NAME?\r\n", 1000);
  sendCommandForEsp32("AT+UART?\r\n", 1000);
  sendCommandForEsp32("AT+ADDR?\r\n", 1000);
  sendCommandForEsp32(setNameCommand, 1000);
  sendCommandForEsp32(setBaudrateCommand, 1000);

  sendCommandForEsp32("AT+NAME?\r\n", 1000);
  sendCommandForEsp32("AT+UART?\r\n", 1000);

#endif

#ifdef HC_06
  sendCommandForEsp32("AT\r\n", 1000);
  sendCommandForEsp32("AT\r\n", 1000);
  sendCommandForEsp32("AT+VERSION\n", 2000);
  sendCommandForEsp32(setNameCommand + "\n", 2000);
  sendCommandForEsp32(setBaudrateCommand + "\n", 2000);

#endif

#endif


}

void loop()
{
  // Nothing here
}

/* Baud-rates available: 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200, 256000, 512000, 962100

    Protocols available:
   SERIAL_5N1   5-bit No parity 1 stop bit
   SERIAL_6N1   6-bit No parity 1 stop bit
   SERIAL_7N1   7-bit No parity 1 stop bit
   SERIAL_8N1   (the default) 8-bit No parity 1 stop bit
   SERIAL_5N2   5-bit No parity 2 stop bits
   SERIAL_6N2   6-bit No parity 2 stop bits
   SERIAL_7N2   7-bit No parity 2 stop bits
   SERIAL_8N2   8-bit No parity 2 stop bits
   SERIAL_5E1   5-bit Even parity 1 stop bit
   SERIAL_6E1   6-bit Even parity 1 stop bit
   SERIAL_7E1   7-bit Even parity 1 stop bit
   SERIAL_8E1   8-bit Even parity 1 stop bit
   SERIAL_5E2   5-bit Even parity 2 stop bit
   SERIAL_6E2   6-bit Even parity 2 stop bit
   SERIAL_7E2   7-bit Even parity 2 stop bit
   SERIAL_8E2   8-bit Even parity 2 stop bit
   SERIAL_5O1   5-bit Odd  parity 1 stop bit
   SERIAL_6O1   6-bit Odd  parity 1 stop bit
   SERIAL_7O1   7-bit Odd  parity 1 stop bit
   SERIAL_8O1   8-bit Odd  parity 1 stop bit
   SERIAL_5O2   5-bit Odd  parity 2 stop bit
   SERIAL_6O2   6-bit Odd  parity 2 stop bit
   SERIAL_7O2   7-bit Odd  parity 2 stop bit
   SERIAL_8O2   8-bit Odd  parity 2 stop bit
*/
