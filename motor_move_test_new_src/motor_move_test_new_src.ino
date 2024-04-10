#include <Arduino.h>
#include <math.h>
#include <M5Stack.h>
#include "cybergear_controller.hh"
#include <vector>
#ifdef USE_ESP32_CAN
#include "cybergear_can_interface_esp32.hh"
#else
#include "cybergear_can_interface_mcp.hh"
#endif

#define INC_POSITION  20.0 //     S,    UL,   LL
std::vector<uint8_t> motor_ids = {0x7F, 0x7E, 0x7D};
std::vector<float> positions = {0.0f, 0.0f, 0.0f};
bool bounce = false;

CybergearController controller = CybergearController(0x00);
#ifdef USE_ESP32_CAN
CybergearCanInterfaceEsp32 interface;
#else
CybergearCanInterfaceMcp interface;
#endif

void setup() {
  M5.begin();

  M5.Lcd.print("Init motors ... ");
  interface.init(12, 15);
  controller.init(motor_ids, MODE_POSITION, &interface);
  controller.enable_motors();
  M5.Lcd.println("done");

  //M5.Lcd.print("configuring settings ... ");
  //M5.Lcd.println("done");

  M5.Lcd.print("safety wait period ... ");
  delay(200);
  M5.Lcd.println("done");
}

void loop() {
  M5.update();

  M5.Lcd.print("calculate rotation position ... ");
  // DO NOT GIVE 0x7F (p[0]) OR 0x7E (p[1]) UNLIMITED ROTATION.

  if (bounce) {
    positions[2] += INC_POSITION / 180.0f * M_PI;
  } else {
    positions[2] -= INC_POSITION / 180.0f * M_PI;
  }
  bounce != bounce;
  
  M5.Lcd.println("done");

  M5.Lcd.print("send command position ... ");
  //controller.send_position_command(motor_ids, positions);
  controller.send_position_command(motor_ids[2], positions[2]);
  M5.Lcd.println("done");
  
  delay(200);
}
