#ifndef TTDISPLAY_H
#define TTDISPLAY_H

#include <LiquidCrystal.h>

#include "TrainTimer.h"
#include "DisplayElement.h"

#define   CONTRAST_PIN   9
#define   BACKLIGHT_PIN  7
#define   CONTRAST       110


#define ELEMENTCOUNT 8


class TTDisplay {

 public:
    TTDisplay();
    // Timing methods
    void startTimer() { startTime = millis(); endTime = 0; tv->setDirty(true);} // records the start millis
    void stopTimer()  { endTime = millis(); tv->setDirty(true);}   // records the stop millis
    boolean isTiming() { return (startTime > 1 && endTime == 0); }   // true if startTimer is recorded, but no stopTimer yet
    long getElapsed() { if (endTime != 0) { return endTime - startTime; } else { return (millis() - startTime); } }// current time minus start if still timing, else stop - start.

    // Let's paint the display
    void updateDisplay();  // updates from all internal values
    void setup();

    void input (InputEvent ie); // Process a new input

    void resetTimer() { startTime = 0; endTime = 0; updateDisplay(); }

    // Mark all elements to dirtyVal
    void setDirty(boolean dirtyVal) { for(int i = 0; i < ELEMENTCOUNT; ++i) elements[i]->setDirty(dirtyVal); }

 private:
    DisplayElement *elements[ELEMENTCOUNT];
    TimeValue *tv;
    SpeedValue *sv;

    int selectedDisplayElement;
    bool displayElementInputMode;

    long startTime;
    long endTime;

    void nextElement() { selectedDisplayElement++; if (selectedDisplayElement >= ELEMENTCOUNT) selectedDisplayElement = 0; }
    void prevElement() { selectedDisplayElement--; if (selectedDisplayElement <= -1 ) selectedDisplayElement = ELEMENTCOUNT - 1; }
    void selectElement() { displayElementInputMode = true; }
    void deselectElement() { displayElementInputMode = false; }

    void lcdPrintLine(int line, const char *msg);

    static LiquidCrystal lcd;
};

#endif
