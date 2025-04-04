#ifndef SEGMENT_H
#define SEGMENT_H

#include "minunit.h"

//Pins used for the display
#define SEG_DISP_E A12
#define SEG_DISP_D A13
#define SEG_DISP_DP A14
#define SEG_DISP_C A15
#define SEG_DISP_G P8_2
#define SEG_DISP_DIG4 P4_4
#define SEG_DISP_B P8_3
#define SEG_DISP_DIG3 P2_5
#define SEG_DISP_DIG2 P4_2
#define SEG_DISP_F P8_0
#define SEG_DISP_DIG1 P4_1
#define SEG_DISP_A P4_7

//Macros for creating the binary value
#define zero 0b1111110
#define one 0b0110000
#define two 0b1101101
#define three 0b1111001
#define four 0b0110011
#define five 0b1011011
#define six 0b1011111
#define seven 0b1110000
#define eight 0b1111111
#define nine 0b1110011

#define off 0b1111
#define digit1 0b0111
#define digit2 0b1011
#define digit3 0b1101
#define digit4 0b1110

class SegmentDisplay {
  public:
    //Constructor sets up all pins
    SegmentDisplay() {
      pinMode(SEG_DISP_E, OUTPUT);
      pinMode(SEG_DISP_D, OUTPUT);
      pinMode(SEG_DISP_DP, OUTPUT);
      pinMode(SEG_DISP_C, OUTPUT);
      pinMode(SEG_DISP_G, OUTPUT);
      pinMode(SEG_DISP_DIG4, OUTPUT);
      pinMode(SEG_DISP_B, OUTPUT);
      pinMode(SEG_DISP_DIG3, OUTPUT);
      pinMode(SEG_DISP_DIG2, OUTPUT);
      pinMode(SEG_DISP_F, OUTPUT);
      pinMode(SEG_DISP_DIG1, OUTPUT);
      pinMode(SEG_DISP_A, OUTPUT);
    }

    //Turn the display completely off
    void turnOff() {
      segmentHelper(0 | (off << 4));
    }
    
    //Translate a int value into its segment shiftable equivalent
    uint16_t shiftValue(uint8_t value) {
      switch (value) {
        case 0:
          return(zero << 9);
          break;
        case 1:
          return(one << 9);
          break;
        case 2:
          return(two << 9);
          break;
        case 3:
          return(three << 9);
          break;
        case 4:
          return(four << 9);
          break;
        case 5:
          return(five << 9);
          break;
        case 6:
          return(six << 9);
          break;
        case 7:
          return(seven << 9);
          break;
        case 8:
          return(eight << 9);
          break;
        case 9:
          return(nine << 9);
          break;
         default:
          return(0);
      }
    }
    
    //Function to shift value into binary value into display
    void segmentHelper(uint16_t value) {
      digitalWrite(SEG_DISP_A, (value >> 15) & 1);
      digitalWrite(SEG_DISP_B, (value >> 14) & 1);
      digitalWrite(SEG_DISP_C, (value >> 13) & 1);
      digitalWrite(SEG_DISP_D, (value >> 12) & 1);
      digitalWrite(SEG_DISP_E, (value >> 11) & 1);
      digitalWrite(SEG_DISP_F, (value >> 10) & 1);
      digitalWrite(SEG_DISP_G, (value >> 9) & 1);
      digitalWrite(SEG_DISP_DP, (value >> 8) & 1);
      digitalWrite(SEG_DISP_DIG1, (value >> 7) & 1);
      digitalWrite(SEG_DISP_DIG2, (value >> 6) & 1);
      digitalWrite(SEG_DISP_DIG3, (value >> 5) & 1);
      digitalWrite(SEG_DISP_DIG4, (value >> 4) & 1);
    }
    
    //Meat function, pass values into display and show them
    void displayTime(uint8_t hour, uint8_t minute) {
      uint16_t value = 0;
      
      segmentHelper(value | shiftValue(hour / 10) | (digit1 << 4));
      delay(1);
      segmentHelper(value | (off << 4));
      segmentHelper(value | shiftValue(hour % 10) | (1 << 8) | (digit2 << 4));
      delay(1);
      segmentHelper(value | (off << 4));
      segmentHelper(value | shiftValue(minute / 10) | (digit3 << 4));
      delay(1);
      segmentHelper(value | (off << 4));
      segmentHelper(value | shiftValue(minute % 10) | (digit4 << 4));
      delay(1);
      segmentHelper(value | (off << 4));
    }

    //Test function to check that our time parse logic works out
    static char* testTimeLogic() {
      uint8_t hour = 12;
      uint8_t minute = 59;

      mu_assert("hour divided by 10 should give us the 10s place", hour / 10 == 1);
      mu_assert("hour mod 10 should give us 1s place", hour % 10 == 2);
      mu_assert("minute divided by 10 should give us the 10s place", minute / 10 == 5);
      mu_assert("minute mod 10 should give us the 1s place", minute % 10 == 9);
      return NULL;
    }

    //Test function to check that the concatenation of the binary strings works
    static char* testBinaryCat() {
      mu_assert("this binary string should create a 1 on digit 1", (0 | (one << 9) | (digit1 << 4)) == 0b0110000001110000);
      mu_assert("this binary string should create an 8 on digit 4 with decimal point" , (0 | (eight << 9) | (1 << 8) | (digit4 << 4)) == 0b1111111111100000);
      return NULL;
    }

    //Test function to check that shifting a value into the display actually sets the pins right
    static char* testShiftToDisplay() {
      SegmentDisplay dummyClass;
      
      dummyClass.segmentHelper(0b1010101111100000);

      mu_assert("A pin should be HIGH", digitalRead(SEG_DISP_A) == HIGH);
      mu_assert("B pin should be LOW", digitalRead(SEG_DISP_B) == LOW);
      mu_assert("C pin should be HIGH", digitalRead(SEG_DISP_C) == HIGH);
      mu_assert("D pin should be LOW", digitalRead(SEG_DISP_D) == LOW);
      mu_assert("E pin should be HIGH", digitalRead(SEG_DISP_E) == HIGH);
      mu_assert("F pin should be LOW", digitalRead(SEG_DISP_F) == LOW);
      mu_assert("G pin should be HIGH", digitalRead(SEG_DISP_G) == HIGH);
      mu_assert("DP pin should be HIGH", digitalRead(SEG_DISP_DP) == HIGH);
      mu_assert("Digit1 pin should be HIGH", digitalRead(SEG_DISP_DIG1) == HIGH);
      mu_assert("Digit2 pin should be HIGH", digitalRead(SEG_DISP_DIG2) == HIGH);
      mu_assert("Digit3 pin should be HIGH", digitalRead(SEG_DISP_DIG3) == HIGH);
      mu_assert("Digit4 pin should be LOW", digitalRead(SEG_DISP_DIG4) == LOW);
      return NULL;
    }

    static char* allSegmentTests() {
      mu_run_test(testTimeLogic);
      mu_run_test(testBinaryCat);
      mu_run_test(testShiftToDisplay);
      return NULL;
    }
};

#endif
