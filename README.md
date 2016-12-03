# Uno train timer

Ok, here's some information about the train timer...

## Hardware

- Arduino MCU
- 1x rotary switch ( for navigating menus )
- 1x red momentary pushbutton ( for resetting timer )
- 1x Slide switch (for power on/off)
- 1x battery pack (4xAA batteries)
- 2x infrared LEDs
- 2x infrared sensor

The main display (leading spaces for clarity only)
```
01234567890123456789 [ this is the 20 chars width... 4 lines ]

Distance(in)   Scale
0012.0            HO
Time(s)   ScaleSpeed
0012.3     0013.2mph
```

## Notes

- On this display, the input fields are:
  - Distance(in)
  - 0012.0
  - HO
  - Scale Speed
  - 0013.2mph
- Rotating the encoder will move the cursor from field to field.  It'll wrap around (either left or right)
- Pushing the encoder switch on one of these fields will cause the cursor to blink... this allows setting that value
  - When in "set value" mode, the following will happen:
    - Distance(in):  rotating left/right will change units (Values:  in, cm)
      - Pressing encoder switch will set the value.  This will update the EEPROM and also cause conversion of the distance value.
  - This will also set the Metric vs. Imperial units and change all distance / speed values
    - 0012.0:   rotating left / right will increase / decrease the value by .1
      - Pressing encoder swtich will set the value.  This will update EEPROM.  And update scale speed display.
    - HO:  rotating left / right will cycle the scale settings (O -> HO -> N -> O)
      - Pressing the encoder siwtch will set the value.  This will update EEPROM and scale speed display.
    - Scale Speed:  rotating left / right will cycle from (Scale Speed -> ActualSpeed -> Scale Speed)
      - Pressing the encoder switch will store the setting.  This will also update the displayed speed value (and units)
    - 0013.2mph:  rotating left/right will cycle the units (mph -> fps -> ips OR kph -> mps -> cms)
- Timing of speed can't happen while setting values.  Entering "set" mode will cancel any timing in progress.

### Modes

1. Initialized from power-on.  Display becomes:
  ```
  Distance(ft)   Scale
  0001.0            HO
  Time(s)   ScaleSpeed
  -             -
  ```
  - Note that the display cursor will be set to the HO field.

#### General function

1. Initial Mode
  - read EEPROM values
  - if EEPROM is not initalized, then initialize to defaults
  - Loop:
    - paint the display if needed
    - On input
      - rotary encoder
      - move cursor, update display
      - rotary encoder switch
        - enter "set" mode
      - timer reset button
      - clear timer values
      - enter "timer" mode
      ```
        Distance(ft)   Scale
        0001.0            HO
        Time(s)   ScaleSpeed
        -             -
      ```
1. Set Mode
  - loop:
    - On Input:
      - rotary encoder:  adjust value, update display
      - rotary encoder switch:  update settings & update EEPROM
      - timer reset button
      ```
        Distance(ft)   Scale
        0001.0           _HO
        Time(s)   ScaleSpeed
        -             -
      ```
1. Timer Mode
  - loop:
    - Every .1 seconds:
    - update the elapsed time display
    - On Input:
      - start timer photo transistor
        - capture the start-time in millis
      - end timer photo transistor
        - capture the end-time in millis
      - set the elapsed time
      - update the display
      - go back to Initial Mode
      - Rotary Encoder Switch
        - clear timings
      - go back to Initial Mode
      - Timer Reset Button
        - clear timings
      - go back to Initial Mode
      - On Max Elapsed Time
        - Clear Timings
        - go back to initial mode
        ```
          Distance(ft)   Scale
          0001.0           _HO
          Time(s)   ScaleSpeed
          000.1             -
        ```
        changes to:
        ```
          Distance(ft)   Scale
          0001.0           _HO
          Time(s)   ScaleSpeed
          009.1      067.3mph
        ```
1. After timing completes
