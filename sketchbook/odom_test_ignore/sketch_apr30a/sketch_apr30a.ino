#include <ros.h>
#include <std_msgs/Int16.h>

#include <NewPing.h>
     
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
#define THRESH_DISTANCE 6
#define GROUND_COVERED 14.3
#define HIGH_THRESH 9
#define LOW_THRESH 5
   
NewPing sonarL(12, 11, 200);

bool current_hit_state = false;
bool prev_hit_state = false;

ros::NodeHandle  nh;
std_msgs::Int16 vLeft, vRight;
ros::Publisher ard_odomR("ard_odomR", &vRight);
ros::Publisher ard_odomL("ard_odomL", &vLeft);
int temp;
     
void setup() {
  //nh.getHardware()->setBaud(4800);
  nh.initNode();  
  nh.advertise(ard_odomL);
  pinMode(13, OUTPUT);  
}
     
void loop() {
  delay(10);
  unsigned long int time_start;
  unsigned long int time_end;
  int distanceL = sonarL.ping_cm();
  if(distanceL >=5 && distanceL<=9)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13,LOW);
  
  Serial.print("Ping: ");
  Serial.print(distanceL);
  Serial.println("cm");

  if(distanceL>=5 && distanceL<=9){
    digitalWrite(13, HIGH);
    current_hit_state = true;
  }else{
    digitalWrite(13, LOW);
    current_hit_state = false;
  }

  if(current_hit_state == true){
    if(prev_hit_state ==false){
      time_start = millis();
      prev_hit_state = true;
    }else{
      prev_hit_state = true;
    }
  }else{
     if(prev_hit_state ==true){
      prev_hit_state = false;
      time_end = (millis()-time_start);
      if(time_end/1000.0 > 0.05){
        temp = (int)(((float)(0.143)/(time_end/1000.0))/1);
        vLeft.data = temp;
        ard_odomL.publish(&vLeft);
        nh.spinOnce();
      }
    }else{
      prev_hit_state = false;
    }
  }


  
  
}

