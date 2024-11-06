#include <Servo.h>
#include <Keypad.h>

const int servoPin = 11;
const int buzzerPin = 13;
const String correctPassword = "1234";  // Set your desired password here

Servo doorServo;
String inputPassword = "";

// Define the rows and columns for the 4x4 keypad
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'D', 'C', 'B', 'A'},  // Row 1
  {'#', '9', '6', '3'},  // Row 2
  {'0', '8', '5', '2'},  // Row 3
  {'*', '7', '4', '1'}   // Row 4
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  doorServo.attach(servoPin);
  pinMode(buzzerPin, OUTPUT);
  
  lockDoor();  // Start with the door locked
}

void loop() {
  char key = keypad.getKey();  // Get the key pressed on the keypad
  
  if (key) {
    Serial.print("Key Pressed: ");  // Display a prompt before printing key
    Serial.println(key);  // Print the key to the serial monitor
    tone(buzzerPin, 1000, 5);  // Make the buzzer buzz for 5 milliseconds
    if (key == '#') {
      checkPassword();
      delay(200); // Debounce for the "#" key
    } else if (key == '*') {
      clearPassword();
    } else if (key == 'D') {
      lockDoor();  // Lock the door when "D" is pressed
      tone(buzzerPin, 1000, 2);  // Short buzz for 2 milliseconds
      delay(200);  // Ensure the buzz completes
    } else {
      inputPassword += key;  // Append the key to the password input
    }
  }
}

// ************************ Password Check ****************************************
void checkPassword() {
  if (inputPassword == correctPassword) {
    unlockDoor();  // Unlock the door
  } else {
    incorrectPasswordAlert();  // Trigger the buzzer for incorrect password
  }
  clearPassword();  // Clear the entered password
}

// ************************ UnLock Door ****************************************
void unlockDoor() {
  doorServo.write(90);  // Rotate servo to 90 degrees (unlock position)
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 2000, 300);  // Buzz at a higher frequency (2000 Hz) for 300 milliseconds
    delay(400);  // 400ms delay between buzzes
  }
  noTone(buzzerPin);  // Stop the buzzer
}


// ************************ Lock Door ****************************************
void lockDoor() {
  doorServo.write(0);  // Rotate servo back to 0 degrees (lock position)
   for (int i = 0; i < 2; i++) {
    tone(buzzerPin, 1000, 100);  // Buzz at a higher frequency (2000 Hz) for 300 milliseconds
    delay(400);  // 400ms delay between buzzes
  }
  noTone(buzzerPin);  // Stop the buzzer
}


void incorrectPasswordAlert() {
  tone(buzzerPin, 1000);  // Buzz at 1000 Hz
  delay(1000);             // Buzz for 2 seconds
  noTone(buzzerPin);      // Stop the buzzer
}

void clearPassword() {
  inputPassword = "";  // Clear password input
}
