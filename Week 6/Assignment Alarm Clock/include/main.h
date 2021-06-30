#ifndef __MAIN_H__
#define __MAIN_H__

int readButtons();
void displayNumber(char digit, char number);
void display(char hour, char minute, bool alarmEnabled);
char changeOnInput(char buttons, char value, char min, char max);
void input();
void handleButtonPresses(char buttonStatus);
void alarm();
void makeSound(int lengthMS);
void alternatingDisplay(char hour, char altHour, char minute, char altMinute, bool alarmDot, bool altAlarmDot);
void clockDisplay();
void keepTime();
void incrementMinute();
void incrementHour();

#endif // __MAIN_H__