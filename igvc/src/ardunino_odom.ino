#include <ros.h>
#include <std_msgs/Float.h>

// pins
const byte motorLeft = 2;
const byte motorRight = 3;

// reset
int reset_at = 1000;

// counters
long countLeft = 0;
long countRight = 0;

// time diffs
long tLeft = 0;
long tRight = 0;

// number of counts of holes per second
int cpsLeft = 0;
int cpsRight = 0;

// ros related
std_msgs::Int16 vLeft, vRight;
ros::Publisher ard_cpsR("ard_cpsR", &vRight);
ros::Publisher ard_cpsL("ard_cpsL", &vLeft);
ros::NodeHandle nh;


void setup() {
  // open serial port
  Serial.begin(9600);
  // set pins
  pinMode(motorLeft, INPUT_PULLUP);
  pinMode(motorRight, INPUT_PULLUP);
  // attach interrupt handler to digital pints
  attachInterrupt(digitalPinToInterrupt(motorLeft), counterLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motorRight), counterRight, CHANGE);
  tLeft = millis();
  tRight = millis();
}

void loop() {
  // reset left count every 200 counts
  if (countLeft >= reset_at) {
    cpsLeft = (int) (countLeft/double(millis()-tLeft));
    tLeft = millis();
    countLeft = 0;

    ard_cpsL.publish(&cpsLeft);
  }
  // reset right count every 200 counts
  if (countRight >= reset_at) {
    cpsRight = (int) (countRight/double(millis()-tRight));
    tRight = millis();
    countRight = 0;

    ard_cpsR.publish(&cpsRight);
  }
}

void counterLeft() {
  countLeft = countLeft + 1;
}

void counterRight() {
  countRight = countRight + 1;
}