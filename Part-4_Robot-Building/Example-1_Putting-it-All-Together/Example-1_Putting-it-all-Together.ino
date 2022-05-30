#include <SoftwareSerial.h> // Include the SoftwareSerial library for serial emulation.

// Left motor's Enable, Forward and Back pins: 
#define L_EN 10
#define L_FWD 4
#define L_BCK 5

// Right motor's Enable, Forward and Back pins:
#define R_EN 11
#define R_FWD 7
#define R_BCK 6

// Virtual RX and TX pins for ESP-01 communication:
#define V_RX 13
#define V_TX 12

// Define the deadzone and threshhold where X and Y are considered unchanged
#define DEADZONE 100
#define THRESHHOLD 10

SoftwareSerial ESP01(V_RX,V_TX); // Virtual serial connection to the HC-05 Module.

String data; // Datastring for the HC-05 messages.
int * dir; // Holds the integer representations of the data.
int x, y = 0;

void setup(){
  motorInitL(); // Initialization function for the left motor.
  motorInitR(); // Initialization function for the right motor.
  
  Serial.begin(9600);
  ESP01.begin(9600);
  ESP01.println("ip:123.123.123.123");
  ESP01.println("ssid:hejsa123");
  ESP01.println("pass:hejsa123");
}

void loop(){

  // Check for new messages from HC-05 module:
  if(ESP01.available() > 0){
    data = ESP01.readStringUntil('\n');
    Serial.println(data);
    int xIdx = data.indexOf("X:");
    int yIdx = data.indexOf("Y:");
    if(!(xIdx == -1) && !(yIdx == -1)){
      String xStr = data.substring(xIdx+2, yIdx);
      String yStr = data.substring(yIdx+2);
      x = xStr.toInt();
      y = yStr.toInt();
      Serial.print("X is "); Serial.print(x); Serial.print("Y is "); Serial.println(y);
    }
  }

  // If y is 50 bigger than the upper threshhold:
  if(y > DEADZONE+THRESHHOLD){
    motorSpeedL(255);
    motorSpeedR(255);
  }
  // If the down button is pressed on joypad:
  else if(y < -DEADZONE-THRESHHOLD){
    motorSpeedL(-255);
    motorSpeedR(-255);
  }
  // If the left button is pressed on joypad:
  else if(x < -DEADZONE-THRESHHOLD){
    motorSpeedL(0);
    motorSpeedR(255);
  }
  // If the right button is pressed on joypad:
  else if(x > DEADZONE+THRESHHOLD){
    motorSpeedL(255);
    motorSpeedR(0);
  }
  // If no defined button is pressed on joypad:
  else{
    motorSpeedL(0);
    motorSpeedR(0);
  }
}

// Initializes the left motor's pins:
void motorInitL(){
  // Set all the pins to output, and write LOW to them:
  pinMode(L_EN, OUTPUT);
  digitalWrite(L_EN, LOW);
  pinMode(L_FWD, OUTPUT);
  digitalWrite(L_FWD, LOW);
  pinMode(L_BCK, OUTPUT);
  digitalWrite(L_BCK, LOW);
}

// Initializes the right motor's pins:
void motorInitR(){
  // Set all the pins to output, and write LOW to them:
  pinMode(R_EN, OUTPUT);
  digitalWrite(R_EN, LOW);
  pinMode(R_FWD, OUTPUT);
  digitalWrite(R_FWD, LOW);
  pinMode(R_BCK, OUTPUT);
  digitalWrite(R_BCK, LOW);
}

// Sets the left motor speed from the input. (From -255 to 255)
void motorSpeedL(int spd){
  
  //If the speed is over 0, Set backward low, forward high:
  if(spd > 0){
    digitalWrite(L_BCK, LOW);
    digitalWrite(L_FWD, HIGH);
    analogWrite(L_EN, spd);
  }
  //If the speed is exactly 0, Set everything low:
  else if(spd == 0){
    digitalWrite(L_BCK, LOW);
    digitalWrite(L_FWD, LOW);
    analogWrite(L_EN, spd);
  }
  //If the speed is under 0, Set forward low, backward high:
  else{
    digitalWrite(L_FWD, LOW);
    digitalWrite(L_BCK, HIGH);
    analogWrite(L_EN, -spd);
  }
}

// Sets the right motor speed from the input. (From -255 to 255)
void motorSpeedR(int spd){
  //If the speed is over 0, Set backward low, forward high:
  if(spd > 0){
    digitalWrite(R_BCK, LOW);
    digitalWrite(R_FWD, HIGH);
    analogWrite(R_EN, spd);
  }
  //If the speed is exactly 0, Set everything low:
  else if(spd == 0){
    digitalWrite(R_BCK, LOW);
    digitalWrite(R_FWD, LOW);
    analogWrite(R_EN, spd);
  }
  //If the speed is under 0, Set forward low, backward high:
  else{
    digitalWrite(R_FWD, LOW);
    digitalWrite(R_BCK, HIGH);
    analogWrite(R_EN, -spd);
  }
}
