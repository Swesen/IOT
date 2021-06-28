#define DOT 10

/*takes an array with 8 pins and connects them to our 7-segment display
Each pin should be an Arduino-pin number from 0 to (and including) 13
and these should be entered as such: {segment-A, segment-B, ... , segment-DP} */
void setSegmentPins(int *pins);

/*Sets all segments so that the display shows numbers from 
0-9. If numbers 10-19 are put in then the display should show
the number -10 so 10 becomes 0, 11 becomes 1 and so on, but the dot should also be lit. 
If any other number than 0-19 is entered the display should be completely turned off. */
void setDisplayNumber(int nr);