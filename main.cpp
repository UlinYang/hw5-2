#include "mbed.h"
#include "bbcar.h"

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);
BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

BusInOut qti_pin(D4,D5,D6,D7);

int main() {
   float distance = 0;
   parallax_qti qti1(qti_pin);
   int pattern;
   int original_angle;
   car.goStraight(50);
   int flag = 1;
   while(1) {
      pattern = (int)qti1;
      //printf("%d\n",pattern);
      switch (pattern) {
         case 0b1000: car.turn(60, 0.1); break;
         case 0b1100: car.turn(70, 0.45); break;
         case 0b1110: car.turn(70, 0.6); break;
         case 0b0100: car.turn(70, 0.7); break;
         case 0b0110: car.goStraight(60); break;
         case 0b0010: car.turn(70, -0.7); break;
         case 0b0111: car.turn(70, -0.6); break;
         case 0b0011: car.turn(70, -0.45); break;
         case 0b0001: car.turn(60, -0.1); break;
         case 0b1111: car.stop(); break;
         default: car.goStraight(50);
      }
      if (pattern == 0b1111) {
          if(flag == 1) {
              original_angle = car.servo0.angle;
          }
          ThisThread::sleep_for(1s);
          distance = (car.servo0.angle - original_angle +0.01)*6.5*3.14/360;
          printf("distance: %f \n", distance);
          flag++;
          if(flag <= 5){
            car.goStraight(35);
            ThisThread::sleep_for(1s);
          }
      }
      ThisThread::sleep_for(10ms);
      
   }
}