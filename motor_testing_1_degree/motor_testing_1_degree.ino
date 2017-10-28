char user_input;
int x;
int y;
int state;


void setup() {                
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  Serial.begin(9600); //Open Serial connection for debugging

}

void loop() {
  
    while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      if (user_input =='9')
      {
        for (int i=1;i<16;i++)
        {
         test123();
        }
      }
}
}

void test123()
{
    digitalWrite(9, HIGH);
    delay(1);          
    digitalWrite(9, LOW); 
    delay(1);    
}

