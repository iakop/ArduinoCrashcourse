#include <SoftwareSerial.h> // Include the SoftwareSerial library for serial emulation.

// Virtual RX and TX pins for ESP-01 communication:
#define V_RX 13
#define V_TX 12

SoftwareSerial ESP01(V_RX,V_TX); // Virtual serial connection to the HC-05 Module.

String data; // Datastring for the ESP-01 messages.
int x, y = 0;

void setup(){
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
}
