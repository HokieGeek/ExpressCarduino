#define buttonPin 2
#define potPin A0

#define photocellCount 2
#define photocell1Pin A1
#define photocell2Pin A2

int potValue = -1;
int targetPotValue = -1;
String inputString = "";

int photocellValues[3] = {0,0,0};

void sensorsSetup() {
    potValue = analogRead(potPin); // Initialize the pot tracker
    pinMode(buttonPin, INPUT);

    inputString.reserve(200);
}

void sensorsLoop() {
    pollPotentiometer();

    pollAmbientLight();

    if (haveHandshake) {
        writePotentiometer();
        writeButton();

        writeAmbientLight();
    }
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
    }
}

void writePotentiometer() {
    if (haveHandshake) {
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

void writeButton() {
    if (digitalRead(buttonPin) == HIGH) {
        Serial.println("Press!!");
    }
}

void pollAmbientLight() {
    photocellValues[1] = analogRead(photocell1Pin);
    photocellValues[2] = analogRead(photocell2Pin);

    photocellValues[0] = (photocellValues[1] + photocellValues[2]) / photocellCount;
}

void writeAmbientLight() {
    // Serial.write(photocellValues, sizeof(int)*(photocellCount+1));
    Serial.print(photocellValues[0]);
    Serial.print(",");
    Serial.print(photocellValues[1]);
    Serial.print(",");
    Serial.print(photocellValues[2]);
    Serial.println("");
}
