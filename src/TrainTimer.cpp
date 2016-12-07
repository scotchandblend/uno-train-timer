
/*
 * PIO libs required
 *
 * pio lib install "TaskScheduler"
 * pio lib install "LiquidCrystal"
 * pio lib install "Encoder"
 * pio lib install "Bounce2"
 */



/*
 * Code has baked into it a table of conversion factors for the different gauges:
 *   - O, HO, N
 * 
 * EEPROM settings:
 *   - distance between measuring points ( initialize to 12" originally )
 *   - default scale readout (initialize to HO originally)
 *   - english / metric units
 * 
 * LCD display is mapped out like this:
 * 
 *  01234567890123456789   [ this is the 20 chars width... 4 lines ]
 *
 *  Distance(ft)   Scale   [ line 0 ]
 *  0001.0            HO   [ line 1 ]
 *  Time(s)   ScaleSpeed   [ line 2 ]
 *  0012.3     0013.2mph   [ line 3 ]
 * 
 * 
 * Scale speed may be displayed as:
 *   - mph (miles per hour), fps (feet per second)
 *   - kph (kilometers per hour), mps (meters per second)
 * 
 * 
 */


/*
 * configuration setttings
 */



#include <EEPROM.h>
#include <Wire.h>
#include <TaskScheduler.h>
#define ENCODER_USE_INTERRUPTS
#include <Encoder.h>
#include <Bounce2.h>


#include "TrainTimer.h"
#include "TTDisplay.h"
#include "DisplayElement.h"


Scheduler runner;   // Task Scheduler to manage all processing callbacks.


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached


#define ENCODER_BUTTON_PIN 10
Bounce debouncer_encoder_button = Bounce();

#define TIMER_BUTTON_PIN 9
Bounce debouncer_timer_button = Bounce();

// Scale Table
double inchesToScaleFeet( char *scale ) {
    if (strcmp(scale, "HO") == 0) {
        return 1.111;
    } else {
        return 0.0;
    }
}

// circular increment/decrement operators for RailroadScale
RailroadScale nextScale (RailroadScale s) {
    switch(s) {
    case RAILROADSCALE_O:
        return(RAILROADSCALE_HO);
    case RAILROADSCALE_HO:
        return(RAILROADSCALE_N);
    case RAILROADSCALE_N:
        return(RAILROADSCALE_O);
    default:
        return RAILROADSCALE_O;
    }
}
RailroadScale prevScale (RailroadScale s) {
    switch(s) {
    case RAILROADSCALE_O:
        return(RAILROADSCALE_N);
    case RAILROADSCALE_HO:
        return(RAILROADSCALE_O);
    case RAILROADSCALE_N:
        return(RAILROADSCALE_HO);
    default:
        return RAILROADSCALE_O;
    }
}

const char *railroadScaleEnumToStr(RailroadScale s) {
    switch(s) {
    case RAILROADSCALE_O:
        return "O";
    case RAILROADSCALE_HO:
        return "HO";
    case RAILROADSCALE_N:
        return "N";
    default:
        return "??";
    }
}


TTDisplay myDisplay;

void setup()
{
  Serial.begin ( 115200 );

  // Configure the encoder button
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
  debouncer_encoder_button.attach(ENCODER_BUTTON_PIN);
  debouncer_encoder_button.interval(5); // internal in milliseconds

  // Configure the timer button
  pinMode(TIMER_BUTTON_PIN, INPUT_PULLUP);
  debouncer_timer_button.attach(ENCODER_BUTTON_PIN);
  debouncer_timer_button.interval(5); // internal in milliseconds

  myDisplay.setup();
}

long oldPosition; // needed to determine direction of the encoder

void loop()
{
    int inputsCount = 0;
    InputEvent inputs[5];

    if (debouncer_timer_button.fell()) {
        if (!myDisplay.isTiming()) {
            Serial.println("Timer Button:  startTimer");
            myDisplay.startTimer();
        } else {
            Serial.println("Timer Button:  stopTimer");
            myDisplay.stopTimer();
        }
    }

    debouncer_encoder_button.update();
    long newPosition = myEnc.read();

    if ((newPosition % 4 == 0) && (newPosition != oldPosition)) {
        if (newPosition > oldPosition) {
            InputEvent ie(InputEvent::IE_CCW);
            inputs[inputsCount] = ie;
            Serial.print("Encoder val:");
            Serial.print(newPosition);
            Serial.println("generating event: IE_CCW");
        } else {
            InputEvent ie(InputEvent::IE_CW);
            inputs[inputsCount] = ie;
            Serial.print("Encoder val:");
            Serial.print(newPosition);
            Serial.println("generating event: IE_CW");
        }
        ++inputsCount;
        oldPosition = newPosition;
    }
    // int buttonVal = debouncer_encoder_button.read();
    // if (buttonVal == LOW ) {

    if (debouncer_encoder_button.fell()) {
        // Button is pushed
        InputEvent ie(InputEvent::IE_BUTTON);
        inputs[inputsCount] = ie;
        ++ inputsCount;
            Serial.println("generating event: IE_BUTTON");
    }

    // process any events in the queue
    for (int i = 0; i < inputsCount; i++) {
        Serial.print("Dispatching event #");
        Serial.println(i);
        myDisplay.input(inputs[i]);
    }

    myDisplay.updateDisplay();
}

void TrainTimerEEPROM::initDefaults() {

    this->maxTime = 300000; // 5 minutes, in milliseconds
    this->scale = RAILROADSCALE_O;
    this->units = IMPERIAL;

    this->distance = 12;  // 12 inches
}
