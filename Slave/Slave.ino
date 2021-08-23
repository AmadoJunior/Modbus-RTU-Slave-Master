#include <ModbusRTU.h>

#define SLAVE_ID 1
#define RXTX_PIN 5

ModbusRTU modBus;

void setup() {
  //Begin
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1);
  modBus.begin(&Serial2, RXTX_PIN);
  modBus.setBaudrate(115200);
  
  //Set
  modBus.slave(SLAVE_ID);
  modBus.addHreg(0);
  modBus.Hreg(0, 100);
}

void loop() {
  modBus.task();
  yield();
}
