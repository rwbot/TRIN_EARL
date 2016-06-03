/* 
 */

#include <ros.h>
#include <std_msgs/Int8.h>


ros::NodeHandle  nh;
int dir, spd;

void messageCb(const std_msgs::Int8& msg){
  
  if(dir == 0){ //stop
    analogWrite(3, 0.0);
    analogWrite(8, 0.0);
    
  } else if(dir == 6.0){ //right
    analogWrite(3, msg.data);
    analogWrite(8,0);
    
  }else if(dir == 4){ //left
    analogWrite(3, 0);
    analogWrite(8, msg.data);

}else if(dir == 8){ //left
    analogWrite(3, 8);
    analogWrite(8, msg.data);
   
    
  }else{
    analogWrite(3, 0);
    analogWrite(8, 0);
  }
  
}


void messageCb2(const std_msgs::Int8& msg2){
  
  if(msg2.data == 0){ //stop
    dir = 0;
  } else if(msg2.data == 6.0){ //right
        dir = 6;

    
  }else if(msg2.data == 4){ //left
       dir = 4;

    
  }else if(msg2.data == 8){ //left
    dir = 8;
  }else{
    dir = 0;
  }
  
}


ros::Subscriber<std_msgs::Int8> sub2("Direction", &messageCb2 );
ros::Subscriber<std_msgs::Int8> sub("Speed", &messageCb );



void setup()
{ 
   nh.getHardware()->setBaud(4800);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(sub2);

}

void loop()
{  
  nh.spinOnce();
  delay(250);
}

