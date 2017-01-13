#include <NewPing.h>
     
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
#define THRESH_DISTANCE 6
#define GROUND_COVERED 14.3
#define HIGH_THRESH 9
#define LOW_THRESH 5
     
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
bool current_hit_state = false;
bool prev_hit_state = false;
unsigned long int time_start;
unsigned long int time_end;
     
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  
}
     
void loop() {
  delay(1);
  int distance = sonar.ping_cm();
  if(distance >=LOW_THRESH && distance<=HIGH_THRESH)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13,LOW);
  
  /*Serial.print("Ping: ");
  Serial.print(distance);
  Serial.println("cm");
*/
  if(distance>=LOW_THRESH && distance<=HIGH_THRESH){
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
        Serial.print("Hit Durration: ");
        Serial.print(time_end/1000.00000);
        Serial.println(" seconds");
        Serial.println("");
        Serial.print((int)((float)(14.3)/(time_end/1000.0)));
        Serial.println(" cm/s");
        Serial.println("====================");
      }
    }else{
      prev_hit_state = false;
    }
  }

  
  
}

