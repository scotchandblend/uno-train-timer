#ifndef DISPLAYELEMENT_H
#define DISPLAYELEMENT_H


#include "TrainTimer.h"

#include <LiquidCrystal.h>


class InputEvent {
 public:
    enum EventType {IE_NOP, IE_CW, IE_CCW, IE_BUTTON};
    InputEvent(EventType a_et=IE_NOP) { this->et = a_et; }

    EventType getEventType() {return et;}
    
 private:
    EventType et;
};

class DisplayElement {
 public:

    DisplayElement(int c, int r) { col=c; row=r; }

    virtual void updateDisplay (LiquidCrystal lcd) = 0;  // Update ourself on the display
    virtual boolean input (InputEvent ie) = 0; // pass an input event to this element... if it returns false, then this element is done with input

    boolean isDirty() { return dirty; }
    void setDirty(boolean a_dirty) { dirty = a_dirty; }
    void setFocus(LiquidCrystal lcd) { lcd.setCursor(col, row); }

 private:
    int row, col;
    boolean dirty = true;  // indicates it needs to be redrawn

};

class DistanceLabel: public DisplayElement {
 public:
    DistanceLabel(int c, int r)
        : DisplayElement(c,r){;}
    boolean input ( InputEvent ie );
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display

    static const char *ABBREV_meter;
    static const char *ABBREV_foot;
 private:
    DistanceUnits du = IMPERIAL;
    void invertDu() { du = (du == METRIC) ? IMPERIAL : METRIC; }

};

class DistanceValue: public DisplayElement {

 public:
    DistanceValue(int c, int r)
        : DisplayElement(c, r)
    { distance = 12.0; }

    boolean input (InputEvent ie );
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
    
 private:
    double distance;
    const double stepValue = 0.5;
    void incrementDistance() { distance += stepValue; }
    void decrementDistance() { distance -= stepValue; }
};

#endif
