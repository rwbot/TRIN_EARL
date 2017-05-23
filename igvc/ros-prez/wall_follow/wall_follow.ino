
#include <ros.h>
#include <std_msgs/Int16.h>

#define trigPin 12
#define echoPin 13
#define MOTOR_FORWARD 9      // INA of motor driver board
#define MOTOR_REVERSE 6      // INB of motor 

// 9 and 6 are chosen as the ~ indicates that the 

int MOTOR_SPEED;
int MOTOR_MIN_SPEED = 0;
int MOTOR_MAX_SPEED = 150; //can adjust to 255 for full motor speed. 
int POT_VAL;

long duration = 0;
int distance = 0;

ros::NodeHandle nh;

std_msgs::Int16 int_msg;

ros::Publisher arduino("distance", &int_msg);

#include <ros.h>
#include <std_msgs/Int16.h>
//int intensity = 0;
int way = 0;

void subscriber_node(const std_msgs::Int16& power_msg) {
  //intensity = power_msg.data;
  way = power_msg.data;
  if (way == 1) {
    digitalWrite(MOTOR_FORWARD, LOW);
    analogWrite(MOTOR_REVERSE, 50);
  } else if (way == 2){
    digitalWrite(MOTOR_REVERSE, LOW);
    analogWrite(MOTOR_FORWARD, 100);
  } else if (way == 3){
    digitalWrite(MOTOR_REVERSE, LOW);
    analogWrite(MOTOR_FORWARD, 150);
  } else {
    digitalWrite(MOTOR_REVERSE, LOW);
    analogWrite(MOTOR_FORWARD, 254);
  }
}

ros::Subscriber<std_msgs::Int16> s("dir", &subscriber_node);


void setup() {
  digitalWrite(13,HIGH);
  
  Serial.begin(9600);
  Serial.println("SETUP");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MOTOR_FORWARD, OUTPUT);
  pinMode(MOTOR_REVERSE, OUTPUT);
  
  nh.initNode();
  nh.advertise(arduino);
  nh.subscribe(s);
  
}

int rotation = 0;

void loop() {
  Serial.println("LOOP");
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //digitalWrite(MOTOR_REVERSE, LOW);
  //analogWrite(MOTOR_FORWARD, 20); // the direction is visible at 20
  

  //digitalWrite(MOTOR_FORWARD, LOW);
  //analogWrite(MOTOR_REVERSE, 10);
  

  // if you want the fan to turn other way

  duration = pulseIn(echoPin, HIGH);
  
  distance = (duration/2) / 29.1;
  int_msg.data = distance;

//  digitalWrite(MOTOR_FORWARD, LOW);
//  analogWrite(MOTOR_REVERSE, distance);
    arduino.publish(&int_msg);
    
    nh.spinOnce();
    

  delay(250);
}

