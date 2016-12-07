
#include "TrainTimer.h"
#include "DisplayElement.h"
#include "TTDisplay.h"


// Constructor
TTDisplay::TTDisplay () {

    startTime = 0;
    endTime = 0;

    selectedDisplayElement = 0; // index of the current display element
    displayElementInputMode = false;

    

    elements[0] = new DistanceLabel(0,0);
    elements[1] = new DistanceValue(0,1);
    elements[2] = new ScaleLabel(14,0);
    elements[3] = new ScaleValue(18,1);
    elements[4] = new TimeLabel(0,2);
    elements[5] = tv = new TimeValue(0,3);
    elements[6] = new SpeedLabel(10,2);
    elements[7] = sv = new SpeedValue(10,3);
}

void TTDisplay::lcdPrintLine(int line, const char *msg) {
    lcd.setCursor(0,1);        // go to the next line
    lcd.print(msg);
}

// Initialize our LCD static member
LiquidCrystal TTDisplay::lcd(12, 11, 7, 6, 5, 4, BACKLIGHT_PIN, POSITIVE );

void TTDisplay::setup() {


    // Switch on the backlight and LCD contrast levels
    pinMode(CONTRAST_PIN, OUTPUT);
    analogWrite ( CONTRAST_PIN, CONTRAST );

    //lcd.setBacklightPin ( BACKLIGHT_PIN, POSITIVE );
    //lcd.setBacklight ( HIGH );
    lcd.backlight();
    lcd.begin(20,4);               // initialize the lcd 
    lcd.clear();
    lcd.home();
    lcd.print("<<Starting>>");
    delay(1000);
    lcd.home();
    lcd.clear();

    lcd.cursor();
    lcd.noBlink();
}

void TTDisplay::input( InputEvent ie ) {
    if (displayElementInputMode) {
        Serial.print("Routing event to element:");
        Serial.println(selectedDisplayElement);
        // events route to the selected element
        boolean done = elements[selectedDisplayElement]->input(ie);
        if (done) {
            displayElementInputMode = false;
            lcd.noBlink();
        }
    } else {
        // event is used to select the display element
        switch ( ie.getEventType() ) {
        case InputEvent::IE_CW:
            do { nextElement(); } while (! elements[selectedDisplayElement]->acceptsFocus());
            elements[selectedDisplayElement]->setFocus(lcd);
            break;
        case InputEvent::IE_CCW:
            do { prevElement(); } while (! elements[selectedDisplayElement]->acceptsFocus());
            elements[selectedDisplayElement]->setFocus(lcd);
            break;
        case InputEvent::IE_BUTTON:
            selectElement();
            lcd.blink();
            break;
        default:
            // assert failure
            break;
        }
    }
}

long lastUpdateMillis = 0;

const long timeRefreshInterval = 50;

void TTDisplay::updateDisplay() {

    long curTime = millis();
    // long elapsedSinceRefresh = curTime - lastUpdateMillis;

    // String s("curTime: ");
    // s += curTime;
    // s += ", lastUpdateMillis: ";
    // s += lastUpdateMillis;
    // s += ", elapsedSinceRefres: ";
    
    // Serial.println(s);


    if (isTiming() && ((curTime - lastUpdateMillis) > timeRefreshInterval) ) {
        tv->setElapsed( getElapsed());
        // Serial.print(" Refresh elapsed: ");
        // Serial.println(tv->getElapsed());
        lastUpdateMillis = curTime;
    }
    
    for ( int i = 0; i < ELEMENTCOUNT; ++i) {
        elements[i]->updateDisplay(lcd);
    }
}
