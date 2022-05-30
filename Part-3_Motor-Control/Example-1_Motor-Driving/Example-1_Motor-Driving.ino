#define EN  11 // Enable pin
#define FWD 7 // Forward control pin
#define BCK 6 // Backward control pin

#define BTN 12 // Button pin
#define AIN A0 // Analog input

#define FORWARD 1
#define BACK 0

boolean direction = 1; // Start off going forward
int speed = 0;
unsigned long timer = 0;

void setup() {
	// BTN is input
	pinMode(BTN, INPUT);
	
  // All other pins are output:
  pinMode(FWD, OUTPUT);
  pinMode(BCK, OUTPUT);
  pinMode(EN, OUTPUT);

	// Set timer:
	timer = millis();
}

void loop() {

	if(digitalRead(BTN) == HIGH && (millis() > timer + 200)){
    timer = millis();
		direction = !direction;
	}
	speed = analogRead(AIN)/4;
	turnMotor(direction, speed);

}


// Our own function for controlling the motor is defined:
int turnMotor (boolean dir, int spd){ // Takes two arguments: dir and spd.
  switch (dir){ // Depending on dir, FWD og BCK toggle HIGH or LOW:
    case FORWARD:
      digitalWrite(BCK, LOW);
      digitalWrite(FWD, HIGH);
      analogWrite(EN, spd);
      break;
    case BACK:
      digitalWrite(FWD, LOW);
      digitalWrite(BCK, HIGH);
      analogWrite(EN, spd);
      break;
    default:
      digitalWrite(FWD, LOW);
      digitalWrite(BCK, LOW);
      analogWrite(EN, 0);
      return -1;
      break;
  }
  return 0;
}
