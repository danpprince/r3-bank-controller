int currentBank = 0;
int bankArray[16];

#define DEBOUNCE_DELAY 50

#define BUTTON_BANK_LEFT 2
#define BUTTON_BANK_RIGHT 3
#define LED_PIN 4

enum Direction {LEFT, RIGHT};

class ButtonHandler
{
public:
	unsigned long lastReadingTime;
	int reading, prevReading, buttonState, prevButtonState, buttonPin, advanceDirection;
	int* bankNumber;
        Direction dir;

	ButtonHandler (Direction d, int* b) { 
		prevButtonState = buttonState = LOW;
		dir = d; 
		bankNumber = b;
		if (dir == LEFT)
		{
			buttonPin = BUTTON_BANK_LEFT;
			advanceDirection = -1;
		}
		else if (dir == RIGHT)
		{
			buttonPin = BUTTON_BANK_RIGHT;
			advanceDirection = 1;
		}
	}

	void updateState()
	{
		reading = digitalRead(buttonPin);

		// Reset the timer if the state changes
		if (reading != prevReading)
			// HEY! ints are 16 bit!
			lastReadingTime = millis();

		// If this state has lasted longer than the debounce delay parameter,
		// actually count it as a reading
		if ((millis() - lastReadingTime) > DEBOUNCE_DELAY)
		{
			if (reading != buttonState) 
				buttonState = reading;

			if (buttonState == HIGH && prevButtonState == LOW)
				changeBank();

			prevButtonState = buttonState;
		}

		prevReading = reading;
	}

private:
	void changeBank()
	{
		*bankNumber += advanceDirection;
		
		// Range checking, make array loop around
		if (*bankNumber < 0)
			*bankNumber = 15;
		if (*bankNumber > 15)
			*bankNumber = 0;

		Serial.write(0xC0);
		Serial.write(bankArray[*bankNumber]);

		digitalWrite(LED_PIN, HIGH);
		delay(50);
		digitalWrite(LED_PIN, LOW);
	}

};

ButtonHandler leftButtonHandler(LEFT, &currentBank);
ButtonHandler rightButtonHandler(RIGHT, &currentBank);

void setup() 
{
	//  Set MIDI baud rate:
	Serial.begin(31250);

	// Set pin modes
	pinMode(BUTTON_BANK_LEFT, INPUT);
	pinMode(BUTTON_BANK_RIGHT, INPUT);
	pinMode(LED_PIN, OUTPUT);

	// Populate array
	for(int i = 0; i < 16; i++)
		bankArray[i] = i*8;
}

void loop() 
{
	leftButtonHandler.updateState();
	rightButtonHandler.updateState();
}
