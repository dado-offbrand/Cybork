#include <Arduino.h>
#include <math.h>
#include <M5Stack.h>
#include "cybergear_driver.hh"
#ifdef USE_ESP32_CAN
#include "cybergear_can_interface_esp32.hh"
#else
#include "cybergear_can_interface_mcp.hh"
#endif

#define MAX_ROTATION 2.9 //165deg-ish
#define MIN_ROTATION -2 // -120deg-ish

float target_pos = 0.0f;
MotorStatus motor_status;
bool caliS1 = false;
bool caliS2 = false;

CybergearDriver driver = CybergearDriver(0x00, 0x7E);
#ifdef USE_ESP32_CAN
CybergearCanInterfaceEsp32 interface;
#else
CybergearCanInterfaceMcp interface;
#endif

TFT_eSprite sprite = TFT_eSprite(&sprite);

void setup() {
  M5.begin();

  M5.Lcd.print("Init sprite ... ");
  sprite.setColorDepth(8);
  sprite.setTextSize(3);
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
  M5.Lcd.println("done");
  delay(200);

  M5.Lcd.print("Init driver ... ");
  interface.init(12, 15);
  driver.init(&interface);
  driver.init_motor(MODE_SPEED);
  driver.set_limit_speed(30.0f); // necessary?
  driver.enable_motor();
  M5.Lcd.println("done");

  draw_stats();
}

void draw_stats() {
  sprite.fillScreen(BLACK);
  sprite.setCursor(0, 0);
  sprite.setTextColor(TFT_WHITE, BLACK);

  sprite.setTextSize(4);
  sprite.println(0x7E);

  sprite.setTextSize(2);
  sprite.println("");
  sprite.println("Position:");
  sprite.print(motor_status.position);
  sprite.println(" rad");
  sprite.println("");
  sprite.println("Effort:");
  sprite.print(motor_status.effort);
  sprite.println(" Nm");
  sprite.println("");
  sprite.println("Calibrated:");
  sprite.println(caliS1);
  sprite.println(caliS2);

  sprite.pushSprite(0, 0);
}

void loop() {
  M5.update();

  if (!caliS1) { // set to !calib, just changed for debugging
    driver.set_speed_ref(0.7f); // not specific, just a random speed
    if (motor_status.effort >= 0.3f) {
      driver.set_speed_ref(0.0f);
      driver.set_mech_position_to_zero();

      delay(100);
      driver.init_motor(MODE_POSITION);
      driver.enable_motor();
      driver.set_limit_speed(2.5f);

      caliS1 = true;
    }
  } else if (caliS1 && !caliS2) {
    driver.set_position_ref(-3.14f);
    // if within two degrees, just zero
    if (std::fabs(motor_status.position - -3.14f) < (10.0f / 180.0f * M_PI)) {
      driver.set_limit_speed(1.0f);
      delay(1000);
      driver.set_mech_position_to_zero();
      caliS2 = true;
    }
  } else {
    driver.set_position_ref(target_pos);
  }

  if (M5.BtnA.wasPressed()) {
    target_pos -= 20.0f / 180.0f * M_PI;
  } else if (M5.BtnB.wasPressed()) {
    target_pos = 0.0f;
  } else if (M5.BtnC.wasPressed()) {
    target_pos += 20.0f / 180.0f * M_PI;
  }

  if (driver.process_packet()) {
    motor_status = driver.get_motor_status();
    draw_stats();
  }

  delay(200);
}