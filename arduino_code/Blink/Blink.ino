#include <ros.h>
#include <std_msgs/Bool.h>

ros::NodeHandle nh;
boolean blink;
int LED = 13;

void blinkCB(const std_msgs::Bool& msg) {
  if (msg.data == true) blink = true;
  else blink = false;
}

void setup() {
  pinMode(LED, OUTPUT);
  ros::Subscriber<std_msgs::Bool> sub("blink", &blinkCB);
  nh.initNode();
  
}
void loop() {
  if (blink) {
    digitalWrite(LED, LOW);
    delay(500);
  }
  digitalWrite(LED, HIGH);
  nh.spinOnce();
}   
