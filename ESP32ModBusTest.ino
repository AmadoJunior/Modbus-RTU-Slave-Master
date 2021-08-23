//Dependencies
#include <ModbusRTU.h>

//Global
#define SLAVE_ID 1
#define FIRST_REG 0
#define REG_COUNT 1
#define RXTX_PIN 5

//Init
ModbusRTU modBus;
uint16_t myValue = 1;

//Methods
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  } else {
    //Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());  
  }
  return true;
}

//Setup
void setup() {
  //Serial Begin
  Serial.begin(9600);
  Serial.println("Initiating");
  Serial2.begin(115200, SERIAL_8N1);
  modBus.begin(&Serial2, RXTX_PIN);
  modBus.setBaudrate(115200);

  //Set
  modBus.master();
}

//Loop
void loop() {
  uint16_t res[REG_COUNT];
  if (!modBus.slave()) {    // Check if no transaction in progress
    //modBus.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cb); // Send Read Hreg from Modbus Server
    modBus.readWriteHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, FIRST_REG, &myValue, REG_COUNT, cb);
    myValue += 1;
//    while(modBus.slave()) { // Check if transaction is active
//      modBus.task();
//      delay(10);
//    }
    Serial.println(res[0]);
  }
  modBus.task();
  yield();
  //delay(500);
}
