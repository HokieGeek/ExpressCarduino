#define buttonPin 2

#define photocellCount 2
#define photocell1Pin A1
#define photocell2Pin A2

String inputString = "";

int photocellValues[3] = {0,0,0};

void sensorsSetup() {
    pinMode(buttonPin, INPUT);

    inputString.reserve(200);
}

void sensorsLoop() {
    pollAmbientLight();

    if (haveHandshake) {
        writeButton();

        writeAmbientLight();
    }
}

void sensorsSerialEvent(int input) {
    switch (input) {
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
