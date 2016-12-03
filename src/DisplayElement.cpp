#include "DisplayElement.h"

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
