#define connectedLedPin 12
#define ACK_CHAR ':'

bool haveHandshake = false;

void setup() {
    Serial.begin(9600);

    pinMode(connectedLedPin, OUTPUT);

    sensorsSetup();
}

void loop() {
    sensorsLoop();
}

void serialEvent() {
    int input = 0;
    while (Serial.available()) {
        // if (!haveHandshake) {
        //     waitForHandshake();  // send a byte to establish contact until receiver responds
        // }

        input = Serial.read();
        switch (input) {
        case ',':
            digitalWrite(connectedLedPin, HIGH);
            haveHandshake = true;
            Serial.write(ACK_CHAR);
            break;
        default:
            if (haveHandshake) {
                sensorsSerialEvent(input);
            }
            break;
        }
    }
}

void waitForHandshake() {
    while (Serial.available() <= 0 || Serial.read() != ',') { // TODO: this should be readBytes, I think
        Serial.write('.');
        delay(300);
    }
    digitalWrite(connectedLedPin, HIGH);
    haveHandshake = true;
    Serial.write(ACK_CHAR);
}
