//Arduino Arm Driver Board Code
#include "U8glib.h"
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); 
int servopin1 = 9;
int servopin2 = 6;
int servopin3 = 5;
int servopin4 = 3;
int servopin5 = 11;

//Serial input from Python
int pyInput;

//Default servo angles
int servo1Angle=90;
int servo2Angle=70;
int servo3Angle=60;
int servo4Angle=90;
int servo5Angle=85;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5; 

//free motion = 0
//ball by color = 1
//init = 2
int mode = 2;
int grabbed = 1;
int bottom2 = 0;
int bottom3 = 0;
int speed = 0;
int initDone = 0;
bool redraw = true;
String grabbedStatement;
bool grabDraw = false;
int offsetStr = 0;

void setup() {
  //display init
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    u8g.setColorIndex(255);
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  //servo init
  pinMode(servopin1,OUTPUT);
  pinMode(servopin2,OUTPUT);
  pinMode(servopin3,OUTPUT);
  pinMode(servopin4,OUTPUT);
  pinMode(servopin5,OUTPUT);
  Serial.begin(9600);
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
  servo5.attach(servopin5);
  servo1.write(90);
  servo2.write(70);
  servo3.write(60);
  servo4.write(90);
  servo5.write(85);
  delay(20);
}

void loop() {
  //picture loop
  if (redraw) {
    u8g.firstPage();
    while (u8g.nextPage()){
      if (mode == 2) {
        u8g.drawStr(30, 20, "Initialized.");
        u8g.drawStr(0, 40, "Waiting for Python...");
      }
      else if (mode == 0) {
        u8g.drawStr(30, 20, "Free Motion");
        u8g.drawStr(10, 40, "Press start for BBC");
      }
      else if (mode == 1) {
        u8g.drawStr(30, 20, "Ball By Color");
        u8g.drawStr(0, 40, "Please select a color");
      }
    }
    redraw = false;
  }
  //free motion/ball by color loop
  do{
    pyInput = Serial.read();
    //free motion init
    if (pyInput == 'I') {
      mode = 0;
      redraw = true;
    }
    //ball by color init
    if (pyInput == 'J') {
      mode = 1;
      servo1Angle = servo2Angle = servo3Angle = servo4Angle = servo5Angle = 80;
      servo1.write(servo1Angle);
      servo2.write(servo2Angle);
      servo3.write(servo3Angle);
      servo4.write(servo4Angle);
      servo5.write(servo5Angle);
      redraw = true;
    }
    //free motion
    if (mode == 0) {
      if (pyInput == 'Z') {//base left
        servo1Angle++;
        if(servo1Angle>=180){servo1Angle=180;}
        servo1.write(servo1Angle);
      }
      if (pyInput == 'X'){//base right
        servo1Angle--;
        if(servo1Angle<=0){servo1Angle=0;}
        servo1.write(servo1Angle);
      }
      if (pyInput == 'S') {//shoulder down
        servo2Angle++;
        if(servo2Angle>=180){servo2Angle=180;}
        servo2.write(servo2Angle);
      }
      if (pyInput == 'U') {//shouler up
        servo2Angle--;
        if(servo2Angle<=0){servo2Angle=0;}
        servo2.write(servo2Angle);
      }
      if (pyInput == 'C') {//elbow up
        servo3Angle++;
        if(servo3Angle>=180){servo3Angle=180;}
        servo3.write(servo3Angle);
      }
      if (pyInput == 'D') {//elbow down
        servo3Angle--;
        if(servo3Angle<=0){servo3Angle=0;}
        servo3.write(servo3Angle);
      }
      if (pyInput == 'R') {//hand rotate left
        servo4Angle++;
        if(servo4Angle>=180){servo4Angle=180;}
        servo4.write(servo4Angle);
      }
      if (pyInput == 'L') {//hand rotate right
        servo4Angle--;
        if(servo4Angle<=0){servo4Angle=0;}
        servo4.write(servo4Angle);
      }
      if(pyInput == 'A') {//close claw
        servo5Angle++;
        if(servo5Angle>=85){servo5Angle=85;}
        servo5.write(servo5Angle);
      }
      if(pyInput == 'B') {//open claw
        servo5Angle--;
        if(servo5Angle<=35){servo5Angle=35;}
        servo5.write(servo5Angle);
      } 
    }
    //ball by color
    else if (mode == 1) {
      if ((pyInput == 'O') || (pyInput == 'W') || (pyInput == 'H')){//left
        if (pyInput == 'O') {grabbedStatement = "Red is in Pos. 1"; offsetStr = 20;}
        else if (pyInput == 'W') {grabbedStatement = "Blue is in Pos. 1"; offsetStr = 17;}
        else if (pyInput == 'H') {grabbedStatement = "Yellow is in Pos. 1"; offsetStr = 10;}
        grabbed = 0;
        bottom2 = 170;
        bottom3 = 55;
        servo1Angle = 104;
        servo1.write(servo1Angle);
        grabDraw = true;
      }
      else if ((pyInput == 'T') || (pyInput == 'V') || (pyInput == 'G')){//center
        if (pyInput == 'T') {grabbedStatement = "Red is in Pos. 2"; offsetStr = 20;}
        else if (pyInput == 'V') {grabbedStatement = "Blue is in Pos. 2"; offsetStr = 17;}
        else if (pyInput == 'G') {grabbedStatement = "Yellow is in Pos. 2"; offsetStr = 10;}
        grabbed = 0;
        bottom2 = 167;
        servo1Angle = 70;
        bottom3 = 50;
        servo1.write(servo1Angle);
        grabDraw = true;
      }
      else if ((pyInput == 'E') || (pyInput == 'C') || (pyInput == 'S')){//Right
        if (pyInput == 'E') {grabbedStatement = "Red is in Pos. 3"; offsetStr = 20;}
        else if (pyInput == 'C') {grabbedStatement = "Blue is in Pos. 3"; offsetStr = 17;}
        else if (pyInput == 'S') {grabbedStatement = "Yellow is in Pos. 3"; offsetStr = 10;}
        grabbed = 0;
        bottom2 = 162;
        servo1Angle = 35;
        bottom3 = 40;
        servo1.write(servo1Angle);
        grabDraw = true;
      }
      //second picture loop
      if (grabDraw) {
        u8g.firstPage();
        while (u8g.nextPage()){
          u8g.drawStr(30, 10, "Ball By Color");
          u8g.setPrintPos(offsetStr, 25);
          u8g.print(grabbedStatement);
          if (grabbed == 1) {u8g.drawStr(17, 45, "Task Completed :)");}
        }
        grabDraw = false;
      }
      if (grabbed == 0){ //ball grabbing motion
        servo5Angle = 35;
        servo5.write(servo5Angle);
        servo2Angle = slowWrite(servo2, servo2Angle, bottom2);
        servo3Angle = slowWrite(servo3, servo3Angle, bottom3);
        delay(500);
        servo5Angle = slowWrite(servo5, servo5Angle, 60);
        delay(100);
        servo2Angle = slowWrite(servo2, servo2Angle, 90);
        delay(800);
        servo2Angle = slowWrite(servo2, servo2Angle, bottom2);
        servo3Angle = slowWrite(servo3, servo3Angle, bottom3);
        delay(500);
        servo5Angle = slowWrite(servo5, servo5Angle, 35);
        delay(100);
        servo2Angle = slowWrite(servo2, servo2Angle, 90);
        servo3Angle = slowWrite(servo3, servo3Angle, 90);
        delay(500);
        grabbed = 1;
        grabDraw = true;
      }
    }
  }
  while(Serial.available()>0);
}

//slows down the rotation speed of the servo and returns the new angle
int slowWrite(Servo servo, int current, int target){
  if (current > target){
    while (current > target){
      current--;
      servo.write(current);
      delay(18);
    }
  }
  else if (current < target){
    while(current < target){
      current++;
      servo.write(current);
      delay(18);
    }    
  }
  return current;
}