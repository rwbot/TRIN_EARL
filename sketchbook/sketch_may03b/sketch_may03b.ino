int pin = 13;
volatile int state = LOW;
 
void setup(){                                 
  pinMode(pin, OUTPUT);
  attachInterrupt(0, user_diy, CHANGE);          //The ON/OFF switch can be used as a singal of interruption
}
 
void loop(){
  digitalWrite(pin, state);
}   
 
void user_diy()              //user can give your own code in this interruput function
{     
  state = !state;
}
