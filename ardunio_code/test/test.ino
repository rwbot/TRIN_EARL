/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <std_msgs/String.h>


void setup()
{ 
  analogWrite(3, 0);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop()
{  
  
  delay(1);
}

