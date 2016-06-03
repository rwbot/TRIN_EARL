/* 
 */

#include <ros.h>
#include <std_msgs/Int8.h>

const int switchPinL = 12;
int stateL = LOW;

const int switchPinR = 8;
int stateR = LOW;

//Port5 is left
//port 3 is right

ros::NodeHandle  nh;

void messageCb(const std_msgs::Int8& msg){
  
  if(msg.data == 0){ //stop
    analogWrite(3, 0.0);
    analogWrite(5, 0.0);
    stateL = HIGH;
    digitalWrite(switchPinL, stateL);
    stateR = HIGH;
    digitalWrite(8, stateR);
    
    
  } else if(msg.data == 6.0){ //right
    analogWrite(3, 100);
    analogWrite(5,80);
    stateL = HIGH;
    digitalWrite(switchPinL, stateL);
    stateR = LOW;
    digitalWrite(8, stateR);
    
  }else if(msg.data == 4){ //left
    analogWrite(3, 80);
    analogWrite(5,100);
    stateL = LOW;
    digitalWrite(switchPinL, stateL);
    stateR = HIGH;
    digitalWrite(8, stateR);

  }else if(msg.data >=76){
    analogWrite(3, msg.data+40);
    analogWrite(5, (msg.data)+40);
    stateL = HIGH;
    digitalWrite(switchPinL, stateL);
    stateR = HIGH;
    digitalWrite(8, stateR);

 
  }else if(msg.data < -10){
    analogWrite(3, -(msg.data)+10);
    analogWrite(5, -(msg.data)+10);
    stateL = LOW;
    digitalWrite(switchPinL, stateL);
    stateR = LOW;
    digitalWrite(8, stateR);
    } 
  
}


ros::Subscriber<std_msgs::Int8> sub("Direction", &messageCb );



void setup()
{ 
   nh.getHardware()->setBaud(9600);
  nh.initNode();
  nh.subscribe(sub);
pinMode(5, OUTPUT);
pinMode(3, OUTPUT);
  pinMode(switchPinL, OUTPUT);
    pinMode(switchPinR, OUTPUT);
}

void loop()
{  
  nh.spinOnce();
  delay(250);
}

