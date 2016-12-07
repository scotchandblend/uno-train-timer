#include "DisplayElement.h"


boolean DisplayElement::input( InputEvent ie) {
    boolean retval = false;

    switch ( ie.getEventType()) {
    case InputEvent::IE_CW:
    case InputEvent::IE_CCW:
    case InputEvent::IE_BUTTON:
        retval = true;
    default:
        // Assert error here
        break;
    }
    setDirty(true);
    return retval;
}


boolean DistanceLabel::input( InputEvent ie) {
    boolean retval = false;

    // handle rotating through the values..
    // only two values, so just flip flop them
    switch ( ie.getEventType()) {
    case InputEvent::IE_CW:
        invertDu();
        break;
    case InputEvent::IE_CCW:
        invertDu();
        break;
    case InputEvent::IE_BUTTON:
        // set value... need to return true
        retval = true;
    default:
        // Assert error here
        break;
    }
    setDirty(true);
    return retval;
}

const char *DistanceLabel::ABBREV_meter = "m";
const char *DistanceLabel::ABBREV_foot = "ft";

void DistanceLabel::updateDisplay( LiquidCrystal lcd) {

    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(0,0);

    char labelBuf[13];
    snprintf(labelBuf, sizeof(labelBuf), "Distance(%2s)", (du == IMPERIAL) ? ABBREV_foot : ABBREV_meter);

    // Serial.print("DistanceLabel::updateDisplay - ");
    // Serial.println(labelBuf);

    lcd.print(labelBuf);

    setDirty(false);
}


boolean DistanceValue::input( InputEvent ie) {
    boolean retval = false;

    // handle incrementing / decrementing value
    switch ( ie.getEventType()) {
    case InputEvent::IE_CW:
        incrementDistance();
        break;
    case InputEvent::IE_CCW:
        decrementDistance();
        break;
    case InputEvent::IE_BUTTON:
        // set value... need to return true
        retval = true;
    default:
        // Assert error here
        break;
    }
    setDirty(true);
    return retval;
}

void DistanceValue::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(0,1);

    char distanceBuf[9];

    dtostrf(distance, 5, 2, distanceBuf);

    // String s = "DistanceValue::updateDisplay - ";
    // s += distanceBuf;
    // Serial.println(s);

    lcd.print(distanceBuf);
    setDirty(false);
}



void ScaleLabel::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(14,0);

    lcd.print("Scale");
    setDirty(false);
}



boolean ScaleValue::input( InputEvent ie) {
    boolean retval = false;

    // handle incrementing / decrementing value
    switch ( ie.getEventType()) {
    case InputEvent::IE_CW:
        scale = nextScale(scale);
        break;
    case InputEvent::IE_CCW:
        scale = prevScale(scale);
        break;
    case InputEvent::IE_BUTTON:
        // set value... need to return true
        retval = true;
    default:
        // Assert error here
        break;
    }
    setDirty(true);
    return retval;
}

void ScaleValue::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(17,1);

    char scaleBuf[4];

    sprintf(scaleBuf, "%2s", railroadScaleEnumToStr(scale));
    lcd.print(scaleBuf);
    setDirty(false);
}


void TimeLabel::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(0,2);

    lcd.print("Time(s)");
    setDirty(false);
}

void TimeValue::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;


    long seconds = elapsed / 1000;
    long hundredths = (elapsed % 1000) / 10;
    // Position cursor 
    // Draw the label
    lcd.setCursor(0,3);

    char timeBuf[10];

    sprintf(timeBuf, "%4ld.%02ld", seconds, hundredths);

    // String s = "DistanceValue::updateDisplay - ";
    // s += distanceBuf;
    // Serial.println(s);

    lcd.print(timeBuf);

    setDirty(false);
}

void SpeedLabel::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(10,2);

    lcd.print("ScaleSpeed");
    setDirty(false);
}


void SpeedValue::updateDisplay( LiquidCrystal lcd) {
    if (! isDirty())
        return;
    // Position cursor 
    // Draw the label
    lcd.setCursor(10,3);

    char speedBuf[10];
    dtostrf(speed, 5, 2, speedBuf);

    // String s = "DistanceValue::updateDisplay - ";
    // s += distanceBuf;
    // Serial.println(s);
    lcd.print(speedBuf);

    setDirty(false);
}

