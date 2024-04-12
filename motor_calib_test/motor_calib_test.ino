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
//                                0 S   1 uL  2 LL
std::vector<uint8_t> motor_ids = {0x7F, 0x7E, 0x7D};
std::vector<float> positions = {0.0f, 0.0f, 0.0f};
std::vector<MotorStatus> motor_stats;
int current_index = 1;

CybergearController controller = CybergearController(0x00);
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
  controller.init(motor_ids, MODE_POSITION, &interface);
  controller.enable_motor(motor_ids[current_index], MODE_POSITION);
  controller.set_speed_limit(motor_ids[current_index], 0.5f);
  M5.Lcd.println("done");

  M5.Lcd.print("Setting pos: 0f @ 0.5rad ...");
  controller.send_position_command(motor_ids[current_index], 0.0f);
  delay(4000);
  M5.Lcd.println("done")

  M5.Lcd.print("Setting pos: 0.9f @ 0.5rad ...");
  controller.send_position_command(motor_ids[current_index], 0.9f);
  delay(4000);
  M5.Lcd.println("done");

  controller.set_mech_position_to_zero(motor_ids[current_index]);
  M5.Lcd.println("Set mech pos to zero");

  controller.set_speed_limit(motor_ids[current_index, 1f);
  M5.Lcd.println("Set speed limit back to 1rad");
}

void loop() {
  M5.update();

  (M5.BtnA.wasPressed()) {
    positions[current_index] -= 20.0f / 180.0f * M_PI;
    controller.send_position_command(motor_ids[current_index], positions[current_index]);  
  } else if (M5.BtnB.wasPressed()) {
    controller.set_speed_limit(motor_ids[current_index], 0.5f);
    controller.send_position_command(motor_ids[current_index], 0f)]
    delay(4000);
    controller.set_speed_limit(motor_ids[current_index], 1f);
  } else  if (M5.BtnC.wasPressed()) {
    positions[current_index] += 20.0f / 180.0f * M_PI;
    controller.send_position_command(motor_ids[current_index], positions[current_index]);  
  }

  delay(200);
}
