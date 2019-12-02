// Array begins top right to bottom left
// 0 = off
// 1 = green
// 2 = red
// 3 = blink
int ArrayForActiveLEDs[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
int CurrentLED = 8;
int pinButtonLedSwitch = 2;
int pinButtonLedConfirm = 3;

int LatchPinRED = 8;
int ClockPinRED = 13;
int DataPinRED = 11;

int LatchPinGREEN = 7;
int ClockPinGREEN = 12;
int DataPinGREEN = 10;

bool isGreen = true;

void setup()
{
    Serial.begin(9600);
    // shift red
    pinMode(LatchPinGREEN, OUTPUT);
    pinMode(DataPinRED, OUTPUT);
    pinMode(ClockPinRED, OUTPUT);
    // shift green
    pinMode(LatchPinGREEN, OUTPUT);
    pinMode(DataPinGREEN, OUTPUT);
    pinMode(ClockPinGREEN, OUTPUT);
    // single leds
    pinMode(5, OUTPUT); // red OR
    pinMode(6, OUTPUT); // green OR
    // buttons, 2 = next, 3 = confirm
    pinMode(2,INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), ButtonLedSwitch, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), ButtonLedConfirm, FALLING);
}

void loop()
{
    Blink();
}

void Blink()
{
    if (isGreen)
    {
        digitalWrite(LatchPinGREEN, LOW);
        shiftOut(DataPinGREEN, ClockPinGREEN, 0b00000000);
        digitalWrite(LatchPinGREEN, HIGH);
        digitalWrite(6, 0);
        delay(1000);
        digitalWrite(LatchPinGREEN, LOW);
        shiftOut(DataPinGREEN, ClockPinGREEN, 0b111111111);
        digitalWrite(LatchPinGREEN, HIGH);
        digitalWrite(6, ArrayForActiveLEDs[0]);
        delay(1000);
    }
    else
    {
        digitalWrite(LatchPinRED, LOW);
        shiftOut(DataPinRED, ClockPinRED, 0b00000000);
        digitalWrite(LatchPinRED, HIGH);
        digitalWrite(5, 0);
        delay(1000);
        digitalWrite(LatchPinRED, LOW);
        shiftOut(DataPinRED, ClockPinRED, 0b111111111);
        digitalWrite(LatchPinRED, HIGH);
        digitalWrite(5, ArrayForActiveLEDs[0]);
        delay(1000);
    }
}
void ButtonLedSwitch()
{
    // if currentled > < 0,8, --> set up, down else --
}

void ButtonLedConfirm()
{
    bool win = false;

    if (isGreen)
    {
        isGreen = false;
        win = (CheckForWin(ArrayForActiveLEDs, 1));
    }
    else
    {
        isGreen = true;
        win = (CheckForWin(ArrayForActiveLEDs, 2));
    }
    Serial.print('a');
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
    // This shifts 8 bits out MSB first,
    //on the rising edge of the clock,
    //clock idles low

    //internal function setup
    int i = 0;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);

    //clear everything out just in case to
    //prepare shift register for bit shifting
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);

    //for each bit in the byte myDataOut�
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights.
    for (i = 7; i >= 0; i--)
    {
        digitalWrite(myClockPin, 0);

        //if the value passed to myDataOut and a bitmask result
        // true then... so if we are at i=6 and our value is
        // %11010100 it would the code compares it to %01000000
        // and proceeds to set pinState to 1.
        if (myDataOut & (1 << i))
        {
            pinState = 1;
        }
        else
        {
            pinState = 0;
        }

        //Sets the pin to HIGH or LOW depending on pinState
        digitalWrite(myDataPin, pinState);
        //register shifts bits on upstroke of clock pin
        digitalWrite(myClockPin, 1);
        //zero the data pin after shift to prevent bleed through
        digitalWrite(myDataPin, 0);
    }

    //stop shifting
    digitalWrite(myClockPin, 0);
}

bool CheckForWin(int array[9], int color)
{ /*
    if (array[8] == color) && array[7] == color) && array[6] == color))
        {
            return true; // UUU
        }
    else if (array[2] == color) && array[5] == color) && array[8] == color))
        {
            return true; // LLL
        }
    else if (array[0] == color) && array[1] == color) && array[2] == color))
        {
            return true; // OOO
        }
    else if (array[0] == color) && array[3] == color) && array[6] == color))
        {
            return true; // RRR
        }
    else if (array[1] == color) && array[4] == color) && array[7] == color))
        {
            return true; // MMM ver
        }
    else if (array[3] == color) && array[4] == color) && array[5] == color))
        {
            return true; // MMM hor
        }
    else if (array[0] == color) && array[4] == color) && array[8] == color))
        {
            return true; // OR-UL diagonal
        }
    else if (array[2] == color) && array[4] == color) && array[6] == color))
        {
            return true; // OL-UR diagonal
        }*/
}
