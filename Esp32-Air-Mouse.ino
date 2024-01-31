#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <BleMouse.h>
Adafruit_MPU6050 mpu;
BleMouse bleMouse;

float accX, gyrZ;
int moveX, moveY;
bool state = 0;
bool flag = 0;
bool states = 0;
bool flags = 0;

void setup() {
  Serial.begin(115200);
  pinMode(4, INPUT);
  pinMode(15, INPUT);
  while (!Serial)
    delay(10);
  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("No MPU6050!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 OK.");
  Serial.println("BLE Mouse...");
  bleMouse.begin();
  Serial.println("BLE Mouse OK!");
}

void loop() {
  if(bleMouse.isConnected()) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    accX = a.acceleration.x;
    gyrZ = g.gyro.z;
    moveX = map(accX, -10, 10, -127, 127) * 1;
    moveY = map(gyrZ, -10, 10, -127, 127) * 1,25;
    if(moveX == 0 && moveY == 0) {
      Serial.println("No move");
    }
    else {
      bleMouse.move(moveY, moveX);
      Serial.print(moveX);
      Serial.print(", ");
      Serial.println(moveY);
    }
    //sol tık
    state = digitalRead(4);
    if (state == 1 && flag == 0) {
      bleMouse.click(MOUSE_LEFT);
      flag = 1;
    }
    else if (state == 0 && flag == 1) {
      flag = 0;
    }
    //sağ tık
    states = digitalRead(15);
    if (states == 1 && flags == 0) {
      bleMouse.click(MOUSE_RIGHT);
      flags = 1;
    }
    else if (states == 0 && flags == 1) {
      flags = 0;
    }
    delay(10);
  }
}
