// Array begins top right to bottom left
// 0 = off
// 1 = green
// 2 = red
// 3 = blink
int ArrayForActiveLEDs[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int CurrentLED = 8;
int PinButtonLedSwitch = 2;
int PinButtonLedConfirm = 3;

int LatchPinRED = 8;
int ClockPinRED = 13;
int DataPinRED = 11;

int LatchPinGREEN = 7;
int ClockPinGREEN = 12;
int DataPinGREEN = 10;

bool Led9 = false;
bool IsGreen = true;
int ByteOnGreen;
int ByteOffGreen;
int ByteOnRed;
int ByteOffRed;

void setup()
{
    Serial.begin(9600);
    // shift red
    pinMode(LatchPinRED, OUTPUT);
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
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), ButtonLedSwitch, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), ButtonLedConfirm, FALLING);
}

void loop()
{
    Blink();
    for (int i = 0; i < 9; i++)
    {
        Serial.print(ArrayForActiveLEDs[i]);
    }
    Serial.println("");
}

void Blink()
{
    if (IsGreen)
    {
        digitalWrite(LatchPinGREEN, LOW);
        shiftOut(DataPinGREEN, ClockPinGREEN, ByteOffGreen);
        digitalWrite(LatchPinGREEN, HIGH);

        if (ArrayForActiveLEDs[0] == 1)
        {
            digitalWrite(6, 1);
        }
        else
        {
            digitalWrite(6, 0);
        }
        delay(1000);
        digitalWrite(LatchPinGREEN, LOW);
        shiftOut(DataPinGREEN, ClockPinGREEN, ByteOnGreen);
        digitalWrite(LatchPinGREEN, HIGH);

        if (ArrayForActiveLEDs[0] == 0 || ArrayForActiveLEDs[0] == 2)
        {
            digitalWrite(6, 0);
        }
        else
        {
            digitalWrite(6, 1);
        }
        delay(1000);
    }
    else
    {
        digitalWrite(LatchPinRED, LOW);
        shiftOut(DataPinRED, ClockPinRED, ByteOffRed);
        digitalWrite(LatchPinRED, HIGH);

        if (ArrayForActiveLEDs[0] == 2)
        {
            digitalWrite(5, 1);
        }
        else
        {
            digitalWrite(5, 0);
        }
        delay(1000);
        digitalWrite(LatchPinRED, LOW);
        shiftOut(DataPinRED, ClockPinRED, ByteOnRed);
        digitalWrite(LatchPinRED, HIGH);

        if (ArrayForActiveLEDs[0] == 0 || ArrayForActiveLEDs[0] == 1)
        {
            digitalWrite(5, 0);
        }
        else
        {
            digitalWrite(5, 1);
        }
        delay(1000);
    }
}

void ButtonLedSwitch()
{
    AvoidMultipleInput();
    int nextLED = FindNextLED();
    if (nextLED != 0)
    {
        ArrayForActiveLEDs[nextLED] = 3;
    }
    else
    {
        ArrayForActiveLEDs[nextLED] = 3;
    }

    ParseArrayToString(IsGreen);
    Serial.println("parse");
}

int FindNextLED()
{
    int led = CurrentLED;
    for (int i = 0; i < 9; i++)
    {
        if (ArrayForActiveLEDs[led] != 0)
        {
            if (led >= 0)
            {
                led--;
            }
            if (led < 0)
            {
                led = 8;
            }
        }
    }
    if (ArrayForActiveLEDs[CurrentLED] == 3)
    {
        ArrayForActiveLEDs[CurrentLED] = 0;
    }
    CurrentLED = led;
    return led;
}

void AvoidMultipleInput()
{
    detachInterrupt(digitalPinToInterrupt(2));
    detachInterrupt(digitalPinToInterrupt(3));
    delay(500);
    attachInterrupt(digitalPinToInterrupt(2), ButtonLedSwitch, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), ButtonLedConfirm, FALLING);
}

void ButtonLedConfirm()
{
    AvoidMultipleInput();
    bool win = false;

    if (IsGreen)
    {
        IsGreen = false;
        ArrayForActiveLEDs[CurrentLED] = 1;
        win = (CheckForWin(ArrayForActiveLEDs, 1));
    }
    else
    {
        IsGreen = true;
        ArrayForActiveLEDs[CurrentLED] = 2;
        win = (CheckForWin(ArrayForActiveLEDs, 2));
    }
    CurrentLED--;
    ParseArrayToString(IsGreen);

    if (win)
    {
        detachInterrupt(digitalPinToInterrupt(2));
        detachInterrupt(digitalPinToInterrupt(3));

        while (true)
        {
            if (!IsGreen)
            {
                digitalWrite(LatchPinGREEN, LOW);
                shiftOut(DataPinGREEN, ClockPinGREEN, ByteOffGreen);
                digitalWrite(LatchPinGREEN, HIGH);
                if (ArrayForActiveLEDs[0] == 0 || ArrayForActiveLEDs[0] == 2)
                {
                    digitalWrite(5, 0);
                }
                delay(1000);

                digitalWrite(LatchPinGREEN, LOW);
                shiftOut(DataPinGREEN, ClockPinGREEN, ByteOnGreen);
                digitalWrite(LatchPinGREEN, HIGH);
                if (ArrayForActiveLEDs[0] == 2)
                {
                    digitalWrite(5, 1);
                }
            
            }
            else
            {
                digitalWrite(LatchPinRED, LOW);
                shiftOut(DataPinRED, ClockPinRED, ByteOffRed);
                digitalWrite(LatchPinRED, HIGH);
                if (ArrayForActiveLEDs[0] == 0 || ArrayForActiveLEDs[0] == 1)
                {
                    digitalWrite(6, 0);
                }
                delay(1000);

                digitalWrite(LatchPinRED, LOW);
                shiftOut(DataPinRED, ClockPinRED, ByteOnRed);
                digitalWrite(LatchPinRED, HIGH);
                if (ArrayForActiveLEDs[0] == 1)
                {
                    digitalWrite(6, 1);
                }
            }
            delay(1000);
        }
    }
}

void ParseArrayToString(bool IsGreen)
{
    int byteStringOnGreen = 0;
    int byteStringOffGreen = 0;
    int byteStringOnRed = 0;
    int byteStringOffRed = 0;
    int power = 0;
    if (IsGreen)
    {
        for (int i = 8; i > 0; i--)
        {
            if (ArrayForActiveLEDs[i] == 0)
            {
                byteStringOnGreen += 0;
                byteStringOffGreen += 0;
            }
            if (ArrayForActiveLEDs[i] == 1)
            {
                byteStringOnGreen += pow_int(2, power);
                byteStringOffGreen += pow_int(2, power);
            }
            if (ArrayForActiveLEDs[i] == 2)
            {
                byteStringOnGreen += 0;
                byteStringOffGreen += 0;
            }
            if (ArrayForActiveLEDs[i] == 3)
            {
                byteStringOnGreen += pow_int(2, power);
                byteStringOffGreen += 0;
            }
            power++;
        }

        ByteOffGreen = byteStringOffGreen;
        ByteOnGreen = byteStringOnGreen;
    }
    else
    {
        for (int i = 8; i > 0; i--)
        {
            if (ArrayForActiveLEDs[i] == 0)
            {
                byteStringOnRed += 0;
                byteStringOffRed += 0;
            }
            if (ArrayForActiveLEDs[i] == 1)
            {
                byteStringOnRed += 0;
                byteStringOffRed += 0;
            }
            if (ArrayForActiveLEDs[i] == 2)
            {
                byteStringOnRed += pow_int(2, power);
                byteStringOffRed += pow_int(2, power);
            }
            if (ArrayForActiveLEDs[i] == 3)
            {
                byteStringOnRed += pow_int(2, power);
                byteStringOffRed += 0;
            }
            power++;
        }
        ByteOffRed = byteStringOffRed;
        ByteOnRed = byteStringOnRed;
    }
}

int pow_int(int x, unsigned int n)
{
    int res = 1;

    for (unsigned int i = 0; i != n; ++i)
    {
        res *= x;
    }
    return res;
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
{
    if (array[8] == color && array[7] == color && array[6] == color)
    {
        return true; // UUU
    }
    else if (array[2] == color && array[5] == color && array[8] == color)
    {
        return true; // LLL
    }
    else if (array[0] == color && array[1] == color && array[2] == color)
    {
        return true; // OOO
    }
    else if (array[0] == color && array[3] == color && array[6] == color)
    {
        return true; // RRR
    }
    else if (array[1] == color && array[4] == color && array[7] == color)
    {
        return true; // MMM ver
    }
    else if (array[3] == color && array[4] == color && array[5] == color)
    {
        return true; // MMM hor
    }
    else if (array[0] == color && array[4] == color && array[8] == color)
    {
        return true; // OR-UL diagonal
    }
    else if (array[2] == color && array[4] == color && array[6] == color)
    {
        return true; // OL-UR diagonal
    }
    else
    {
        return false;
    }
}
