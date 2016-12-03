
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
            nextElement();
            elements[selectedDisplayElement]->setFocus(lcd);
            break;
        case InputEvent::IE_CCW:
            prevElement();
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

void TTDisplay::updateDisplay() {
    for ( int i = 0; i < ELEMENTCOUNT; ++i) {
        elements[i]->updateDisplay(lcd);
    }
}
