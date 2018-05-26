#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>

// pins
const byte motorLeft = 2;
const byte motorRight = 3;

// reset
int reset_at = 100;

// counters
long countLeft = 0;
long countRight = 0;

// time diffs
long start = 0;

// number of counts of holes per second
float cpsLeft = 0;
float cpsRight = 0;

// ros related
std_msgs::Float32 vLeft, vRight;
ros::Publisher ard_cpsR("ard_cpsR", &vRight);
ros::Publisher ard_cpsL("ard_cpsL", &vLeft);
ros::NodeHandle nh;


void setup() {
  // set pins
  pinMode(motorLeft, INPUT_PULLUP);
  pinMode(motorRight, INPUT_PULLUP);
  // attach interrupt handler to digital pints
  attachInterrupt(digitalPinToInterrupt(motorLeft), counterLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motorRight), counterRight, CHANGE);
  start = millis();

  nh.initNode();    
  nh.advertise(ard_cpsR);
  nh.advertise(ard_cpsL);
}

void loop() {
  // reset left count every 200 millis
  if (millis() - start > reset_at) {
    cpsLeft = (countLeft/double(millis()-start) * 1000);
    cpsRight = (countRight/double(millis()-start) * 1000);
    
    start = millis();
    
    countLeft = 0;
    countRight = 0;
    
    vLeft.data = cpsLeft;
    ard_cpsL.publish(&vLeft);

    vRight.data = cpsRight;
    ard_cpsR.publish(&vRight);
  }
  nh.spinOnce();
  delay(50);
}

void counterLeft() {
  countLeft = countLeft + 1;
}

void counterRight() {
  countRight = countRight + 1;
}
