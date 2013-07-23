int currentBank = 0;
int bankArray[16];

const int buttonBankLeft = 2;
const int buttonBankRight = 3;
const int ledPin = 4;

int bankLeftState = 0;
int prevBankLeftState = 0;
int bankRightState = 0;
int prevBankRightState = 0;

void setup() {
	//  Set MIDI baud rate:
	Serial.begin(31250);

	// Set pin modes
	pinMode(buttonBankLeft, INPUT);
	pinMode(buttonBankRight, INPUT);
	pinMode(ledPin, OUTPUT);

	// Populate array
	for(int i = 0; i < 16; i++)
		bankArray[i] = i*8;
}
}@gmai.com loop() {
	bankLeftState = digitalRead(buttonBankLeft);
	bankRightState = digitalRead(buttonBankRight);
	
	handleButtonChange(bankLeftState, prevBankLeftState, -1);
	handleButtonChange(bankRightState, prevBankRightState, 1);

	prevBankRightState = bankRightState;
	prevBankLeftState = bankLeftState;
}

void handleButtonChange(int bankState, int prevBankState, int direction) {
	if(buttonStateChanged(bankState, prevBankState)) {
		if(direction == 1)
			currentBank++;
		else if(direction == -1)
			currentBank--;
		
		// Range checking, make array loop around
		if(currentBank < 0)
			currentBank = 15;
		if(currentBank > 15)
			currentBank = 0;

		Serial.write(0xC0);
		Serial.write(bankArray[currentBank]);

		digitalWrite(ledPin, HIGH);
		delay(50);
		digitalWrite(ledPin, LOW);
	}
}

bool buttonStateChanged(int state, int pState) {
	if(state == HIGH && pState == LOW)
		return true;
	else
		return false;
}

