// Hunter Phillips

int green = 11;
int red = 10;
int blue = 9;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  analogWrite(red, 0);
  analogWrite(blue, 0);
  analogWrite(green, 0);
  digitalWrite(red, HIGH);
}

void loop() {
  delay(1000);
  
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
    analogWrite(green, fadeValue);
    delay(30);
  }

  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 1) {
    analogWrite(red, fadeValue);
    delay(30);
  }
digitalWrite(green, LOW);
digitalWrite(red, HIGH);

 
}
