#define buttonPin 2
#define potPin A0
#define connectedLedPin 12

String inputString = "";
int potValue = -1;
int targetPotValue = -1;
bool haveHandshake = false;

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(3000); // TODO: this is only for the human typing this stuff

    inputString.reserve(200);
    potValue = analogRead(potPin); // Initialize the pot tracker

    pinMode(connectedLedPin, OUTPUT);
    pinMode(buttonPin, INPUT);
}

void loop() {
    // Poll the sensors and inputs
    pollPotentiometer();
    pollButton();
}

void serialEvent() {
    int input = 0;
    while (Serial.available()) {
        if (!haveHandshake) {
            waitForHandshake();  // send a byte to establish contact until receiver responds
        }

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

void waitForHandshake() {
    while (Serial.available() <= 0 || Serial.read() != ',') {
        Serial.print('.');
        delay(300);
    }
    digitalWrite(connectedLedPin, HIGH);
    haveHandshake = true;
    Serial.print(':');
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
