const int ledPin = 13; // ledPin is set to hold the "address" of pin 13
const int buttonPin = 12; // buttonPin is set to hold the "address" of pin 12

// Boolean values can be only either true or false:
bool ledState = false; // The variable ledState is initialized as a boolean false value
bool buttonVal = false; // buttonVal is used to hold the digitalRead() from buttonPin
bool prevButtonVal = false; // Holds the previous buttonVal to keep track of changes


void setup() {
  pinMode(ledPin, OUTPUT); // The LED's I/O pin is an output
  pinMode(buttonPin, INPUT); // The button's I/O pin is an input
}

void loop() {
  buttonVal = digitalRead(buttonPin); // Read the value on buttonPin into buttonVal
  
  // If buttonVal's value is equal to HIGH/1/true/ (on), and it was previously off:
  if (buttonVal == HIGH && prevButtonVal == LOW){ // An "and" condition can be written with both && and "and"
    
    // The !-operator is used to represent the logical inverse (1 or 0, true or false), of a value
    ledState = !ledState; // We change ledState to the opposite of what is was
    digitalWrite(ledPin, ledState); //ledPin's output value is set to ledState's value (booleans also work as HIGH or LOW)
    
    // delay() waits for the amount of milliseconds, passed as argument:
    delay(50); // 200 milliseconds delay ensures that mechanical bounce is eliminated (can go as low as ~25-50)
  }
  else if(buttonVal == LOW && prevButtonVal == HIGH){
    delay(50); // Debouncing is necessary on both transistions
  }
  // Update prevButtonVal:
  prevButtonVal = buttonVal;
}
