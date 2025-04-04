#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#define MILLI_IN_SEC 1000
#define SEC_IN_MIN 60
#define MIN_IN_HOUR 60
#define HOURS_IN_DAY 24

class Clock {
  public: 
    //Constructors to setup stuff
    Clock() {
      hour = 0;
      minute = 0;
      lastMillis = 0;
    }

    Clock(uint8_t minuteIn, uint8_t hourIn) {
      hour = hourIn;
      minute = minuteIn;
      lastMillis = 0;
    }

    //Setters and getters
    uint8_t getHour() {
      return(hour);
    }

    uint8_t getMinute() {
      return(minute);
    }

    void setHour(uint8_t hourIn) {
      hour = hourIn;
    }

    void setMinute(uint8_t minuteIn) {
      minute = minuteIn;
    }

    void updateTime() {
      //Calculate the seconds since last time we incremented
      unsigned long timeSince = (millis() - lastMillis) / MILLI_IN_SEC;

      //Increment the minutes if it's been a minute
      if (timeSince / SEC_IN_MIN >= 1) {
        lastMillis = millis();
        minute++;
      }

      //Increment hours if it has been 60 minutes
      if (minute / MIN_IN_HOUR >= 1) {
        hour++;
        minute = 0;
      }

      //Reset hours if it has been a day
      if (hour >= HOURS_IN_DAY) {
        hour = 0;
      }
    }
    
  private:
    unsigned long lastMillis;
    uint8_t hour;
    uint8_t minute;
};

#endif
