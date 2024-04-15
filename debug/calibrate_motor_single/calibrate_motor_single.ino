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

uint8_t MOTOR_ID = 0x7E; // STILL NEEDS UPDATED
MotorStatus motor_status;
float target_position = 0.0f;
float target_torque = 0.0f;
bool calib = false;

CybergearDriver driver = CybergearDriver(0x00, MOTOR_ID);
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
  M5.Lcd.println("done");
  delay(200);
  
  M5.Lcd.print("Finding 180deg point ... ");
  driver.init_motor(MODE_SPEED);
  driver.set_speed_ref(0.25f); // not specific, just a random speed
  driver.enable_motor();
  delay(100); // avoid effort spikes?

  // motor stats may not be updating?
  

  M5.Lcd.println("done");
}

void draw_stats() {
  sprite.fillScreen(BLACK);
  sprite.setCursor(0, 0);
  sprite.setTextColor(TFT_WHITE, BLACK);

  sprite.setTextSize(4);
  sprite.println(MOTOR_ID);

  sprite.setTextSize(2);
  sprite.println("");
  sprite.println("Position:");
  sprite.print(motor_status.position);
  sprite.println(" rad");
  sprite.println("");
  sprite.println("Effort:");
  sprite.print(motor_status.effort);
  sprite.println(" Nm");

  sprite.pushSprite(0, 0);
}

void loop() {
  M5.update();

  if (calib) {
    do {
      if (driver.process_packet()) {
        motor_status = driver.get_motor_status();
        draw_stats();
      }
      delay(100);
    } while (motor_status.effort <= 0.2f);
    driver.set_speed_ref(0.0f);
    driver.init_motor(MODE_POSITION);
    // set zero, move motor back
    calib = true;
  }
  
  if (M5.BtnA.wasPressed()) {
    driver.reset_motor();
  } else if (M5.BtnB.wasPressed()) {
    driver.reset_motor();
  } else if (M5.BtnC.wasPressed()) {
    driver.enable_motor();  
  }

  if (driver.process_packet()) {
    motor_status = driver.get_motor_status();
    draw_stats();
  }

  delay(200);
}
