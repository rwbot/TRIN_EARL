/* 
 */

#include <ros.h>
#include <std_msgs/String.h>


ros::NodeHandle  nh;

int outLeft = 0;
int outRight = 0;

void messageCb( const std_msgs::String& toggle_msg){
  if(toggle_msg.data[0] == '4'){ //plus 5 left
    if(outLeft < 150)
      outLeft = outLeft+5;
    analogWrite(3, outLeft);

  }  

  if(toggle_msg.data[0] == '6'){ //plus 5 right
    if(outRight < 150)
      outRight = outRight+5;
    analogWrite(11, outRight);

  }  

  if(toggle_msg.data[0] == '1'){ //minus 5 left
    if(outLeft > 0)
      outLeft = outLeft-5;
    analogWrite(3, outLeft);
  }  

  
  if(toggle_msg.data[0] == '3'){ //minus 5 right
    if(outRight > 0)
      outRight = outRight-5;
    analogWrite(11, outRight);
  }

  if(toggle_msg.data[0] == '2'){ //stop
    outLeft = 0;
    outRight = 0;
    analogWrite(3, outLeft);
    analogWrite(11, outRight);
  }
}


ros::Subscriber<std_msgs::String> sub("Direction1", &messageCb );



void setup()
{ 
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}

