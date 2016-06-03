int photo=0;

int count=0;

int Cur;
int Past;

int Distance;

void setup() {
  Serial.begin(9600);
  pinMode(photo,INPUT);
}

void loop() 
{
  Past = Cur;
  Cur = analogRead(photo);

  if((Past-Cur)>123)
    {
      count++;
    }
   

    Distance = count* 3;

    Serial.println(analogRead(photo));

    delay(200);
    
}




