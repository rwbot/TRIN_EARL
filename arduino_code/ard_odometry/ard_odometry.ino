#include <ros.h>
#include <std_msgs/Int16.h>

#include <NewPing.h>
/*     
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
#define THRESH_DISTANCE 6
#define GROUND_COVERED 14.3
#define HIGH_THRESH 9
#define LOW_THRESH 5
*/   
NewPing sonarL(12, 11, 200);
NewPing sonarR(7, 6, 200);

bool current_hit_stateL = false;
bool prev_hit_stateL = false;
unsigned long int time_startL;
unsigned long int time_endL;

bool current_hit_stateR = false;
bool prev_hit_stateR = false;
unsigned long int time_startR;
unsigned long int time_endR;

std_msgs::Int16 vLeft, vRight;
ros::Publisher ard_odomR("ard_odomR", &vRight);
ros::Publisher ard_odomL("ard_odomL", &vLeft);
ros::NodeHandle nh;
int tempL, tempR;
     
void setup() {
  nh.getHardware()->setBaud(4800);
  nh.initNode();    
  nh.advertise(ard_odomR);
  nh.advertise(ard_odomL);
  Serial.begin(4800);
  pinMode(13, OUTPUT);  
}
     
void loop() {
  delay(1);

  int distanceR = sonarR.ping_cm();
  //if(distanceR < 12){
//    Serial.print("PING AT: ");
//    Serial.println(distanceR);
//    Serial.println("==========================");
//    Serial.println("");
  //}
  if(distanceR>=4 && distanceR<=9){
    digitalWrite(13, HIGH);
    current_hit_stateR = true;
  }else{
    digitalWrite(13, LOW);
    current_hit_stateR = false;
  }

  if(current_hit_stateR == true){
    if(prev_hit_stateR ==false){
      time_startR = millis();
      prev_hit_stateR = true;
    }else{
      prev_hit_stateR = true;
    }
  }else{
     if(prev_hit_stateR ==true){
      prev_hit_stateR = false;
      time_endR = (millis()-time_startR);
      if(time_endR/1000.0 > 0.0799){
//        Serial.print("Hit Durration Right: ");
//        Serial.print(time_endR/1000.00000);
//        Serial.println(" seconds");
//        Serial.println("");
//        Serial.print((int)((float)(14.1)/(time_endR/1000.0)));
//        Serial.println(" cm/s");
//        Serial.println("====================");
        tempR = (int)((float)(14.1)/(time_endR/1000.0));
        vRight.data = tempR;
        ard_odomR.publish(&vRight);
        //nh.spinOnce();
      }
    }
    else{
      prev_hit_stateR = false;
    }
  }

  int distanceL = sonarL.ping_cm();
//      Serial.print("PING AT: ");
//    Serial.println(distanceL);
//    Serial.println("==========================");
//    Serial.println("");
  if(distanceL>=4 && distanceL<=7){
    digitalWrite(13, HIGH);
    current_hit_stateL = true;
  }else{
    digitalWrite(13, LOW);
    current_hit_stateL = false;
  }

  if(current_hit_stateL == true){
    if(prev_hit_stateL ==false){
      time_startL = millis();
      prev_hit_stateL = true;
    }else{
      prev_hit_stateL = true;
    }
  }else{
     if(prev_hit_stateL ==true){
      prev_hit_stateL = false;
      time_endL = (millis()-time_startL);
      if(time_endL/1000.0 > 0.0799){
//        Serial.print("Hit Durration Left: ");
//        Serial.print(time_endL/1000.00000);
//        Serial.println(" seconds");
//        Serial.println("");
//        Serial.print((int)((float)(14.1)/(time_endL/1000.0)));
//        Serial.println(" cm/s");
//        Serial.println("====================");
        tempL = (int)((float)(14.1)/(time_endL/1000.0));
        vLeft.data = tempL;
        ard_odomL.publish(&vLeft);
      }
    }
    else{
      prev_hit_stateL = false;
    }
  }

  
  //A
 nh.spinOnce();

}

