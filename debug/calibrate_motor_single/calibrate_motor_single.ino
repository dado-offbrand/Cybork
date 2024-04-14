#include <Arduino.h>
#include <math.h>
#include <M5Stack.h>
#include "cybergear_driver.hh"
#ifdef USE_ESP32_CAN
#include "cybergear_can_interface_esp32.hh"
#else
#include "cybergear_can_interface_mcp.hh"
#endif

#define UPPER_LEG_CALIB_EFFORT 0.5
#define MAX_ROTATION 2.9 //165deg-ish
#define MIN_ROTATION -2 // -120deg-ish

uint8_t MOTOR_ID = 0x7E; // STILL NEEDS UPDATED
MotorStatus motor_status;

CybergearDriver driver = CybergearDriver(0x00, MOTOR_ID);
#ifdef USE_ESP32_CAN
CybergearCanInterfaceEsp32 interface;
#else
CybergearCanInterfaceMcp interface;
#endif

void setup() {
  M5.begin();

  M5.Lcd.print("Init systems ... ");
  interface.init(12, 15);
  driver.init(&interface);
  driver.init_motor(MODE_POSITION);
  driver.set_limit_speed(0.5f);
  M5.Lcd.println("done");

  M5.Lcd.print("Finding 180deg point ... ");

  M5.Lcd.println("Done");
}

void loop() {
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    
  } else if (M5.BtnB.wasPressed()) {
    driver.reset_motor();
  else if (M5.BtnC.wasPressed()) {
    driver.enable_motor();  
  }

  if (driver.process_packet()) {
    motor_status = driver.get_motor_status();
    draw_display();
  }

  delay(200);
}

void draw_display() {
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
  sprite.println(" NM");

  sprite.pushSprite(0, 0);
}
