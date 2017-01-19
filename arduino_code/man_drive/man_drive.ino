#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>

//const int switchPinL = 12;
//const int left = 5;
//const int right = 3;
//int stateL = LOW;

//const int switchPinR = 8;
//int stateR = LOW;

//Port5 is left
//port 3 is right

int led = 13;
boolean blink = false;
int count = 0;

ros::NodeHandle  nh;

void leftCB(const std_msgs::Int16& msg){
  if(msg.data >= 0){
    //stateL = HIGH;
    analogWrite(3, msg.data);
    digitalWrite(8, HIGH);

  }else if(msg.data < -10){
    //stateL = LOW;
    analogWrite(3, -(msg.data));
    digitalWrite(8, LOW);

  }
}

void rightCB(const std_msgs::Int16& msg){
    if(msg.data >= 0){
    //stateR = HIGH;
    analogWrite(5, msg.data);
    digitalWrite(12, HIGH);

  }else if(msg.data < -10){
    //stateR = LOW;
    analogWrite(5, -(msg.data));
    digitalWrite(12, LOW);

  }
}

void blinkCB(const std_msgs::Bool& msg) {
  blink = msg.data;
}

ros::Subscriber<std_msgs::Int16> subL("Left", &leftCB );
ros::Subscriber<std_msgs::Int16> subR("Right", &rightCB );
ros::Subscriber<std_msgs::Bool> subB("blink", &blinkCB );

void setup()
{ 
  nh.getHardware()->setBaud(9600);
  nh.initNode();
  nh.subscribe(subL);
  nh.subscribe(subR);
  nh.subscribe(subB);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{  
  if (blink && count >= 1000) {
    count = 0;
    if (digitalRead(13) == HIGH) {
      digitalWrite(led, HIGH);
    }
    else {
      digitalWrite(led, LOW);
    }
  }
  count += 10;
  
  nh.spinOnce();
  delay(10);
}

