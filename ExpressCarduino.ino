#define buttonPin 2
#define potPin A0
#define connectedLedPin 13

String inputString = "";
int potValue = -1;
int targetPotValue = -1;

void setup() {
    Serial.begin(9600);

    waitForHandshake();  // send a byte to establish contact until receiver responds
    Serial.setTimeout(3000); // TODO: this is only for the human typing this stuff

    inputString.reserve(200);
    potValue = analogRead(potPin); // Initialize the pot tracker

    pinMode(buttonPin, INPUT);
}

void loop() {
    // Poll the sensors and inputs
    pollPotentiometer();
    pollButton();

    // Send out status?
    // TODO: sendStatus();
}

void waitForHandshake() {
    while (Serial.available() <= 0 || Serial.read() != ',') {
      Serial.print('.');
      delay(300);
    }
    Serial.println("hello");
    digitalWrite(connectedLedPin, HIGH);
}

void serialEvent() {
    int input = 0;
    while (Serial.available()) {
        input = Serial.read();
        switch (input) {
        case '$':
            targetPotValue = Serial.parseInt();
            Serial.print("#");
            Serial.println(targetPotValue);
            break;
        default:
            char inChar = (char)input;
            if (inChar == '\n' || inChar == '\r') {
                Serial.println(inputString);
                inputString = "";
            } else {
                inputString += inChar;
            }
            break;
        }
    }
}

void pollPotentiometer() {
    int currentPotValue = analogRead(potPin);
    if (currentPotValue != potValue && abs(currentPotValue - potValue) > 1) {
        potValue = currentPotValue;

        if (targetPotValue >= 0) {
            if (potValue == targetPotValue) {
                Serial.print('!');
            } else if (potValue < targetPotValue) {
                Serial.print('+');
            } else {
                Serial.print('-');
            }
        }
    }
}

void pollButton() {
    if (digitalRead(buttonPin) == HIGH) {
      Serial.println("Press!!");
    }
}
