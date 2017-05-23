#define lightPin 13

#include <ros.h>
#include <std_msgs/Bool.h>

ros::NodeHandle nh;

int delayTime = 500;

void callback(const std_msgs::Bool& msg) {
  if (msg.data) {
    delayTime = 0;  
  }
  else {
    delayTime = 500;
  }
}

void setup() {

  pinMode(lightPin, OUTPUT);
  ros::Subscriber<std_msgs::Bool> sub("is_autonomous", &callback);

  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  digitalWrite(lightPin, HIGH);
  delay(delayTime);
  digitalWrite(lightPin, LOW);
  nh.spinOnce();
}
