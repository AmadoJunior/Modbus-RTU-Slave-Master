//Dependencies
#include <ModbusRTU.h>
#include "BluetoothSerial.h"

//Bluetooh Enable
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


//Global
#define SLAVE_ID 1
#define FIRST_REG 1
#define REG_COUNT 1
#define RXTX_PIN 15
#define COMMON_PIN 23
#define LED_PIN 2

//Init
ModbusRTU modBus;
BluetoothSerial SerialBT;
uint16_t myValue = 0;
uint16_t firstReg = 1;
String message = "";

//Methods
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Failed Request Result: 0x");
    Serial.print(event, HEX);
  } else {
    digitalWrite(LED_PIN, HIGH);
    //Serial.printf_P("Request Result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());  
  }
  return true;
}

//Setup
void setup() {
  //Serial Begin
  Serial.begin(9600);
  Serial.println("Initiating");
  SerialBT.begin("ESP32");
  Serial.println("BT Active");
  Serial2.begin(115200, SERIAL_8N1);
  modBus.begin(&Serial2, RXTX_PIN);
  modBus.setBaudrate(115200);

  //Common Pin
  pinMode(OUTPUT, COMMON_PIN);
  digitalWrite(COMMON_PIN, LOW);
  pinMode(OUTPUT, LED_PIN);
  
  //Set
  modBus.master();
}

//Loop
void loop() {
  uint16_t res[REG_COUNT];
   if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    
    if (incomingChar != '\n'){
      message += String(incomingChar);
    } else {
      message = "";
    }
    
    if(message.length() == 2){
      Serial.println(message);
      if(message[0] == 'r'){
        firstReg = ((int)message[1] - 48);
      } else if(message[0] == 'v'){
        myValue = ((int)message[1] - 48);
      }
    }
  }
  if (!modBus.slave()) {    // Check if no transaction in progress
    //modBus.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cb); // Send Read Hreg from Modbus Server
    modBus.readWriteHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, firstReg, &myValue, REG_COUNT, cb);
    Serial.println(res[0]);
  }
  modBus.task();
  yield();
}
