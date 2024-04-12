#include <Arduino.h>
#include <math.h>
#include <M5Stack.h>
#include "cybergear_driver.hh"
#include <vector>
#ifdef USE_ESP32_CAN
#include "cybergear_can_interface_esp32.hh"
#else
#include "cybergear_can_interface_mcp.hh"
#endif

#define SLOW_SPEED 0.5
#define NORMAL_SPEED 5

float position = 0.0f;
MotorStatus status;
int current_index = 1;

CybergearDriver driver = CybergearDriver(0x00, 0x7E);
#ifdef USE_ESP32_CAN
CybergearCanInterfaceEsp32 interface;
#else
CybergearCanInterfaceMcp interface;
#endif

// theory: new power cycle shifts zero by 0.9rad (approx), setting new mech
// resets this back to 0 rad?
void setup() {
  M5.begin();

  M5.Lcd.print("Init systems ... ");
  interface.init(12, 15);
  driver.init(&interface);
  driver.init_motor(MODE_POSITION);
  driver.set_limit_speed(SLOW_SPEED);
  driver.enable_motor();
  M5.Lcd.println("done");

  M5.Lcd.print("Setting pos: 0f @ 0.5rad ...");
  driver.set_position_ref(0.0f);
  delay(4000);
  M5.Lcd.println("done");

  M5.Lcd.print("Setting pos: 0.9f @ 0.5rad ...");
  driver.set_position_ref(0.9f);
  delay(4000);
  M5.Lcd.println("done");

  driver.set_mech_position_to_zero();
  M5.Lcd.println("Set mech pos to zero");

  driver.set_limit_speed(NORMAL_SPEED);
  M5.Lcd.println("Set speed limit back to 1rad");
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    position -= 20.0f / 180.0f * M_PI;
    driver.set_position_ref(position); 
  } else if (M5.BtnB.wasPressed()) {
    driver.set_limit_speed(SLOW_SPEED);
    driver.set_position_ref(0.0f); 
    delay(4000);
    driver.set_limit_speed(NORMAL_SPEED);
    position = 0.0f;
  } else if (M5.BtnC.wasPressed()) {
    position += 20.0f / 180.0f * M_PI;
    driver.set_position_ref(position); 
  }

  delay(200);
}