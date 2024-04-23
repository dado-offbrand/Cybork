#include <Arduino.h>
#include <math.h>
#include <M5Stack.h>
#include "cybergear_driver.hh"
#ifdef USE_ESP32_CAN
#include "cybergear_can_interface_esp32.hh"
#else
#include "cybergear_can_interface_mcp.hh"
#endif

#define CALIBRATION_SPEED 2.0
#define NORMAL_SPEED 30.0
#define MAX_ROTATION 2.9 //165deg-ish
#define MIN_ROTATION -2 // -120deg-ish
#define LL_REDUCTION 1.1464 // 1.14635639927
// 32.66 and 37.44, big pulley is for motor

float target_pos = 0.0f;
MotorStatus motor_status;

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
  driver.set_limit_speed(CALIBRATION_SPEED); // for motor calibration
  driver.enable_motor();
  M5.Lcd.println("done");

  M5.Lcd.print("Calibrate motor ... ");
  calibrate_motor(0.3f, -90.0f, false);
  M5.Lcd.println("done");

  delay(2000);
  driver.set_limit_speed(NORMAL_SPEED); // normal motor speed
}

void calibrate_motor(float endstop_effort_max, int degrees_from_endstop, bool reduced) {
  // move to endstop
  while (motor_status.effort <= endstop_effort_max) {
    M5.update();
    driver.set_speed_ref(0.7f);
    update_status();
  }

  // calibrate at endstop
  driver.set_speed_ref(0.0f);
  driver.set_mech_position_to_zero();
  delay(100);
  driver.init_motor(MODE_POSITION);
  driver.enable_motor();
  
  // move to center
  float target = degrees_from_endstop / 180.0f * M_PI;
  if (reduced) { target/=LL_REDUCTION; }
  while (std::fabs(motor_status.position - target) > (10.0f / 180.0f * M_PI)) {
    M5.update();
    driver.set_position_ref(target);
    update_status();
  }

  // zero at center
  delay(1000);
  driver.set_mech_position_to_zero();
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

  sprite.pushSprite(0, 0);
}

void update_status() {
  if (driver.process_packet()) {
    motor_status = driver.get_motor_status();
    draw_stats();
  }  
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    target_pos -= 20.0f / 180.0f * M_PI;
  } else if (M5.BtnB.wasPressed()) {
    target_pos = 0.0f;
  } else if (M5.BtnC.wasPressed()) {
    target_pos += 20.0f / 180.0f * M_PI;
  }

  driver.set_position_ref(target_pos);
  
  update_status();

  delay(200);
}
