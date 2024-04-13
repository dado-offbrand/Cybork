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

#define INC_POSITION  10.0 //     S,    UL,   LL
std::vector<uint8_t> motor_ids = {0x7F, 0x7E, 0x7D};
std::vector<float> positions = {0.0f, 0.0f, 0.0f};
std::vector<MotorStatus> motor_stats;
int current_index = 2; // default start on LL

CybergearController controller = CybergearController(0x00);
#ifdef USE_ESP32_CAN
CybergearCanInterfaceEsp32 interface;
#else
CybergearCanInterfaceMcp interface;
#endif

TFT_eSprite sprite = TFT_eSprite(&sprite);

void setup() {
  M5.begin();

  M5.Lcd.print("Initialize systems ... ");
  interface.init(12, 15);
  controller.init(motor_ids, MODE_POSITION, &interface);
  controller.enable_motors();

  sprite.setColorDepth(8);
  sprite.setTextSize(3);
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
  M5.Lcd.println("done");

  M5.Lcd.print("Send move command and wait");
  controller.send_position_command(motor_ids, positions);
  delay(2000);
  M5.Lcd.println("done");
  delay(200);
}

void loop() {
  M5.update();
  
  // DO NOT GIVE 0x7F (p[0]) OR 0x7E (p[1]) UNLIMITED ROTATION.
  if (M5.BtnA.wasPressed()) {
    positions[current_index] -= INC_POSITION / 180.0f * M_PI;
    controller.send_position_command(motor_ids[current_index], positions[current_index]);
    draw_screen();
  } else if (M5.BtnB.wasPressed()) {
    current_index++;
    if (current_index > 2) {
      current_index = 0;  
    }
    draw_screen();
  } else if (M5.BtnC.wasPressed()) {
    positions[current_index] +=  INC_POSITION / 180.0f * M_PI;
    controller.send_position_command(motor_ids[current_index], positions[current_index]);
    draw_screen();
  }

  if (controller.process_packet()) {
    draw_screen();
  }
  
  delay(200);
}

void draw_screen()
{
  sprite.fillScreen(BLACK);
  sprite.setCursor(0, 0);
  sprite.setTextColor(TFT_WHITE, BLACK);

  sprite.setTextSize(4);
  sprite.println(motor_ids[current_index]);

  sprite.setTextSize(2);
  sprite.println("");
  sprite.println("=== Target ===");
  sprite.print("Position:");
  sprite.print(positions[current_index]);
  sprite.println(" rad");
  sprite.println("");

  sprite.println("=== Current ===");
  sprite.print("Position:");
  sprite.print(motor_stats[current_index].position);
  sprite.println(" rad");
  sprite.print("Velocity:");
  sprite.print(motor_stats[current_index].velocity);
  sprite.println(" rad/s");
  sprite.print("Effort : ");
  sprite.print(motor_stats[current_index].effort);
  sprite.println(" Nm");

  sprite.pushSprite(0, 0);
}
