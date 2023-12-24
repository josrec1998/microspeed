#include "mbed.h"
#include "PwmOut.h"
#include <C12832.h>


Serial PC(USBTX, USBRX);
enum direction{Forward,Backward,Stopped};
DigitalOut Enable();


C12832 LCD(D11, D13, D12, D7, D10); //uses the C12832 library to declare and LCD object.


//------------ Joystick ------------//


class Joystick{ // class used to define an object joystick.
   private:
       DigitalIn up, down, left, right, fire;
   public:
       //constructor that takes in the digital pins names and assigns them as inputs
       Joystick(PinName u, PinName d, PinName l, PinName r, PinName f) : up(u), down(d), left(l), right(r), fire(f){}
       bool upPressed();   // definition of all the functions used in this program
       bool downPressed();
       bool leftPressed();
       bool rightPressed();
       bool firePressed();
};


bool Joystick::upPressed(){ // all functions are the same, used to test for the joystick position.
   if(up){         // Joystick pins are active high so it reads the pin value and if its logic high,
       return true;// returns bool value true, else it returns ta value false.
   }
   else{
       return false;
   }
}


bool Joystick::downPressed(){
   if(down){
       return true;
   }
   else{
       return false;
   }
}


bool Joystick::leftPressed(){
   if(left){
       return true;
   }
   else{
       return false;
   }
}


bool Joystick::rightPressed(){
   if(right){
       return true;
   }
   else{
       return false;
   }
}


bool Joystick::firePressed(){
   if(fire){
       return true;
   }
   else{
       return false;
   }
}


class Motor{
   private:
       DigitalOut Bipolar, MotorDirection;
       PwmOut speed;
       InterruptIn Encoder;
       int count, prevcount=0;
       Ticker calulator;
       float time = 0.01, ETR, WV;
   public:
       Motor(PinName a, PinName b, PinName c, PinName d):Bipolar(a), MotorDirection(b), speed(c), Encoder(d){
           count = 0;
           MotorDirection.write(1);
           Bipolar.write(0);
           speed.write(0);
           speed.period_us(100);
           Encoder.rise(callback(this,&Motor::Counter));
           calulator.attach(callback(this,&Motor::SpeedCalculator), time);
       }
       void Direction(direction dir);
       void Speed(float S){speed.write(S);}
       void Counter(){count++;}
       void ResetCounter(){count=0;}
       int GetCount(){return count;}
       float distance();
       void SpeedCalculator();
       float WheelSpeed(){return WV;}
       float EncoderTickRate(){return ETR;}
};


void Motor::Direction(direction dir){
   if(dir == Forward){
       MotorDirection.write(1);
   }
   else if(dir == Backward){
       MotorDirection.write(0);
   }
   else{
       MotorDirection.write(1);
       speed.write(0);
   }
}


float Motor::distance(){
   int CPR=256;
   float pi=3.14159, WD=0.0777;
   return ((WD*pi)/CPR);
}


void Motor::SpeedCalculator(){
   ETR=(count-prevcount)/time;
   prevcount=count;
   WV=ETR*distance();
}


int main(){
   Motor Left(, , , );
   Motor Right(, , , );
   Left.Direction(Forward);
   Left.Speed(0);
   Right.Direction(Forward);
   Right.Speed(0);
   Enable.write(1);
   Joystick Joy (A2, A3, A4, A5, D4); //creates 2 objects, joystick names joy and LED called Led.
   LCD.cls();
   LCD.locate(3,3);
   LCD.printf("Right encoder, Left Encoder");
   float speedR = 0;
   float speedL = 0.9;
   while(true){
       if(Joy.upPressed()==true){
           Left.Direction(Forward);
           Right.Direction(Forward);
           Right.Speed(0.8);
           Left.Speed(0.4);
       }
       else if(Joy.downPressed()==true){
           Left.Direction(Backward);
           Right.Direction(Backward);
           Left.Speed(0.8);
           Right.Speed(0.4);
       }
       else {
           Left.Speed(0);
           Right.Speed(0);
           speedR = 0;
           speedL = 0.9;
       }
       LCD.locate(3,12);
       LCD.printf("%d      %d     ",Right.GetCount(),Left.GetCount());
       if(Joy.firePressed()==true){
           Right.ResetCounter();
           Left.ResetCounter();
       }
   }


}
