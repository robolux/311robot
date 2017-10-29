char user_input;
int x;
int y;
int state;


void setup() {                
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(8, LOW);
  digitalWrite(A0,0);
  digitalWrite(A1,0);
  digitalWrite(A2,0);
  Serial.begin(9600); //Open Serial connection for debugging

}

void loop() {
  
    while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      if (user_input =='9')
      {
         test123();
      }
}
}

void test123()
{
    digitalWrite(8, HIGH);
    delay(1);          
    digitalWrite(8, LOW); 
    delay(1);    
}

