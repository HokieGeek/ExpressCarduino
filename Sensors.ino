#define buttonPin 2
#define potPin A0

int potValue = -1;
int targetPotValue = -1;
String inputString = "";

void sensorsSetup() {
    potValue = analogRead(potPin); // Initialize the pot tracker
    pinMode(buttonPin, INPUT);

    inputString.reserve(200);
}

void sensorsLoop() {
    pollPotentiometer();
    pollButton();
}

void sensorsSerialEvent(int input) {
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
