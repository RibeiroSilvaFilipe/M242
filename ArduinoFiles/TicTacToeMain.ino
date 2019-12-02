// Array begins top right to bottom left
int ArrayForActiveLEDs[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};

int latchPinRED = 8;
int clockPinRED = 13;
int dataPinRED = 11;

int latchPinGREEN = 7;
int clockPinGREEN = 12;
int dataPinGREEN = 10;

void setup()
{
    Serial.begin(9600);
    // shift red
    pinMode(latchPinRED, OUTPUT);
    pinMode(dataPinRED, OUTPUT);
    pinMode(clockPinRED, OUTPUT);
    // shift green
    pinMode(latchPinGREEN, OUTPUT);
    pinMode(dataPinGREEN, OUTPUT);
    pinMode(clockPinGREEN, OUTPUT);
    // single leds
    pinMode(5, OUTPUT); // red OR
    pinMode(6, OUTPUT); // green OR
    // buttons, 3 = next, 4 = confirm
    pinMode(3, INPUT);
    pinMode(4, INPUT);
}

void loop()
{
    digitalWrite(latchPinGREEN, LOW);
    shiftOut(dataPinGREEN, clockPinGREEN, 0b111111111);
    digitalWrite(latchPinGREEN, HIGH);
    digitalWrite(6, ArrayForActiveLEDs[0]);
    delay(1000);
    digitalWrite(latchPinGREEN, LOW);
    shiftOut(dataPinGREEN, clockPinGREEN, 0b00000000);
    digitalWrite(latchPinGREEN, HIGH);
    digitalWrite(6, 0);
    delay(1000);
    if (CheckForWin(ArrayForActiveLEDs))
    {
        // allahu akbar
    }
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

bool CheckForWin(int array[9])
{
    switch (1)
    {
        //green
        if (array[8] == 1 && array[7] == 1 && array[7] == 1)
            return true; // UUU
    /*case array{0,0,1,0,0,1,0,0,1}: return true; // LLL
    case array{1,1,1,0,0,0,0,0,0}: return true; // OOO
    case array{1,0,0,1,0,0,1,0,0}: return true; // RRR
    case array{0,1,0,0,1,0,0,1,0}: return true; // MMM vertikal
    case array{0,0,0,1,1,1,0,0,0}: return true; // MMM horizontal
    case array{1,0,0,0,1,0,0,0,1}: return true; // OR-UL diagonal
    case array{0,0,1,0,1,0,1,0,0}: return true; // OL-UR diagonal
    //Red
    case array{0,0,0,0,0,0,2,2,2}: return true; // UUU
    case array{0,0,2,0,0,2,0,0,2}: return true; // LLL
    case array{2,2,2,0,0,0,0,0,0}: return true; // OOO
    case array{2,0,0,2,0,0,2,0,0}: return true; // RRR
    case array{0,2,0,0,2,0,0,2,0}: return true; // MMM vertikal
    case array{0,0,0,2,2,2,0,0,0}: return true; // MMM horizontal
    case array{2,0,0,0,2,0,0,0,2}: return true; // OR-UL diagonal
    case array{0,0,2,0,2,0,2,0,0}: return true; // OL-UR diagonal*/
    default:
        return false;
    }
}
