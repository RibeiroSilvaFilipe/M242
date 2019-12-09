// Provisorisches Konzept aus Stichwörtern für Programmcode

/* 
CurrentLEDSet int = ???;
CurrentLED int = ???;
LEDColor = ???;

start func(){
    Initialize variables(
        ArrayForActiveLEDs
        PinsForIO
    )
}

loop(){
    blink CurrentLEDAndColor(CurrentLED, LEDColor)
    switch(ArrayForActiveLEDs{
        case: 12 Cases--> winnerFunc(LED1, LED2, LED3) 
        default
    }
}

externerInterrupt ButtonLedSwitch(){
    CurrentLED++
}

externerInterrupt ButtonLedConfirm(){
    CurrentLEDAndColor(CurrentLED, LEDColor) Save in ArrayForActiveLEDs
    Remove CurrentLEDAndColor(CurrentLED, LEDColor) from LEDSwitchPool
    Make CurrentLEDAndColor(CurrentLED, LEDColor) Glow constantly
    Change LEDColor
}

winnerFunc(array ArrayForActiveLEDs){
    blink LED1
    blink LED2
    blink LED3
}
*/

/*
currentled-- --> nächste led, 
wenn currentled == 8 --> letzte led blinkt
for(i =0, i<9, i++) 
    if array[currentled] != 0
        currentled--
if array[currentled] != 0
    return '9'
else
{
        generatestring()
}
*/
