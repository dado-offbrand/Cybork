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
int current_index = 2; // default start on LL

CybergearController controller = CybergearController(0x00);
#ifdef USE_ESP32_CAN
CybergearCanInterfaceEsp32 interface;
#else
CybergearCanInterfaceMcp interface;
#endif

void setup() {
  M5.begin();

  M5.Lcd.print("Setting up stuff ... ");
  interface.init(12, 15);
  controller.init(motor_ids, MODE_POSITION, &interface);
  controller.enable_motors();

  controller.send_position_command(motor_ids, positions);

  delay(200);
  M5.Lcd.println("done");
}

void loop() {
  M5.update();
  
  // DO NOT GIVE 0x7F (p[0]) OR 0x7E (p[1]) UNLIMITED ROTATION.
  if (M5.BtnA.wasPressed()) {
    positions[current_index] -= 10.0f / 180.0f * M_PI;
  } else if (M5.BtnB.wasPressed()) {
    current_index++;
    if (current_index > 2) {
      current_index = 0;  
    }
  } else if (M5.BtnC.wasPressed()) {
      positions[current_index] +=  10.0f / 180.0f * M_PI;
  }

  //controller.send_position_command(motor_ids, positions);
  controller.send_position_command(motor_ids[current_index], positions[current_index]);

  draw_screen();
  delay(200);
}

void draw_screen() {
  M5.Lcd.print("Current motor: ");
  M5.Lcd.println(current_index);

  M5.Lcd.print("Current position: ");
  M5.Lcd.println(positions[current_index]);
}
