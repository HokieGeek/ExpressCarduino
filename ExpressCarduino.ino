#define connectedLedPin 12
#define ACK_CHAR ':'

bool haveConnection = false;
bool displayAscii = false;

void setup() {
    Serial.begin(9600);

    pinMode(connectedLedPin, OUTPUT);

    sensorsSetup();
}

void loop() {
    sensorsLoop();
}

void serialEvent() {
    // int input = 0;
    char cmd = 0;
    while (Serial.available()) {
        // if (!haveConnection) {
        //     waitForHandshake();  // send a byte to establish contact until receiver responds
        // }

        // input = Serial.read();
        Serial.readBytes(&cmd, 1);
        switch (cmd) {
        case ';':
            displayAscii = true;
        case ',':
            // byte baud[];
            digitalWrite(connectedLedPin, HIGH);
            haveConnection = true;
            Serial.write(ACK_CHAR);
            break;
        case '.':
            digitalWrite(connectedLedPin, LOW);
            haveConnection = false;
            Serial.write(ACK_CHAR);
            break;
        default:
            if (haveConnection) {
                sensorsSerialEvent(cmd);
            }
            break;
        }
    }
}

/*
void waitForHandshake() {
    while (Serial.available() <= 0 || Serial.read() != ',') { // TODO: this should be readBytes, I think
        Serial.write('.');
        delay(300);
    }
    digitalWrite(connectedLedPin, HIGH);
    haveConnection = true;
    Serial.write(ACK_CHAR);
}
*/
