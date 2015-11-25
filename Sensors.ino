#define ACK_CHAR '.'

#define buttonPin 2

#define photocellCount 2
#define photocell1Pin A1
#define photocell2Pin A2

boolean sendButtonState = false;
boolean sendAmbientLight = false;
String inputString = "";

int photocellValues[photocellCount+1] = {0,0,0};

void sensorsSetup() {
    pinMode(buttonPin, INPUT);

    inputString.reserve(200);
}

void sensorsLoop() {
    pollAmbientLight();

    if (haveHandshake) {
        if (sendButtonState) {
            writeButton();
        }

        if (sendAmbientLight) {
            writeAmbientLight();
        }
    }
}

void sensorsSerialEvent(int input) {
    switch (input) {
    case 'B':
        sendButtonState = true;
        Serial.write(ACK_CHAR);
        break;
    case 'L': 
        sendAmbientLight = true;
        Serial.write(ACK_CHAR);
        break;
    case '*':
        sendAmbientLight = true;
        sendButtonState = true;
        Serial.write(ACK_CHAR);
        break;
    default: // TODO: this is mostly useless, for now.
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

void writeButtonASCII() {
    if (digitalRead(buttonPin) == HIGH) {
        Serial.println("Press!!");
    }
}

void writeButton() {
    Serial.write('B');
    Serial.write(digitalRead(buttonPin));
}

void pollAmbientLight() {
    photocellValues[1] = analogRead(photocell1Pin);
    photocellValues[2] = analogRead(photocell2Pin);

    photocellValues[0] = (photocellValues[1] + photocellValues[2]) / photocellCount;
}

void writeAmbientLightASCII() {
    Serial.print("L");
    Serial.print(photocellValues[0]); // Average
    Serial.print(",");
    Serial.print(photocellValues[1]);
    Serial.print(",");
    Serial.print(photocellValues[2]);
    Serial.println("");
}

void writeAmbientLight() {
    Serial.write("L");
    Serial.write((uint8_t)(photocellCount+1));
    Serial.write((uint8_t*)photocellValues, sizeof(photocellValues));
}
