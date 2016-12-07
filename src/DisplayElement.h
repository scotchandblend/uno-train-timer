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

    DisplayElement(int c, int r, boolean f = false) { col=c; row=r; focusable = f; }

    virtual void updateDisplay (LiquidCrystal lcd) = 0;  // Update ourself on the display
    virtual boolean input (InputEvent ie); // pass an input event to this element... if it returns false, then this element is done with input

    boolean isDirty() { return dirty; }
    void setDirty(boolean a_dirty) { dirty = a_dirty; }
    void setFocus(LiquidCrystal lcd) { lcd.setCursor(col, row); }

    boolean acceptsFocus() { return focusable;} 
 private:
    int row, col;
    boolean dirty = true;  // indicates it needs to be redrawn
    boolean focusable;
};

class DistanceLabel: public DisplayElement {
 public:
    DistanceLabel(int c, int r)
        : DisplayElement(c,r,true){;}
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
        : DisplayElement(c, r, true)
    { distance = 12.0; }

    boolean input (InputEvent ie );
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
    
 private:
    double distance;
    const double stepValue = 0.5;
    void incrementDistance() { distance += stepValue; }
    void decrementDistance() { distance -= stepValue; }
};

class ScaleLabel: public DisplayElement {
public:
    ScaleLabel(int c, int r)
        : DisplayElement(c,r, false) // can't handle a inputs
    { ; }

private:
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
};


class ScaleValue: public DisplayElement {

public:
    ScaleValue(int c, int r)
        : DisplayElement(c,r, true)
    { scale = RailroadScale::RAILROADSCALE_O; }

private:
    RailroadScale scale;
    boolean input (InputEvent ie );
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
};

class TimeLabel: public DisplayElement {
public:
    TimeLabel(int c, int r)
        : DisplayElement(c,r, false) // can't handle a inputs
    { ; }

private:
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
};


class TimeValue: public DisplayElement {

public:
    TimeValue(int c, int r)
        : DisplayElement(c,r, false)
    { ; }

    void setElapsed(long e) { elapsed = e; setDirty(true);}
    long getElapsed() { return elapsed; }

private:
    long elapsed = 0;
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
};


class SpeedLabel: public DisplayElement {
public:
    SpeedLabel(int c, int r)
        : DisplayElement(c,r, false) // can't handle a inputs
    { ; }

private:
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
};

class SpeedValue: public DisplayElement {
public:
    SpeedValue(int c, int r)
        : DisplayElement(c,r, false)
    { ; }

    void setSpeed(double spd) { speed = spd; setDirty(true);}
    double getSpeed() { return speed; }

private:
    double speed=0.0;
    void updateDisplay (LiquidCrystal lcd);  // Update ourself on the display
};



#endif
