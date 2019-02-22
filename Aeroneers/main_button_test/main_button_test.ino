void setup() {
  // put your setup code here, to run once:
  pinMode(24, INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(24)==HIGH)
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }
  else if (digitalRead(24)==LOW)
  {
    digitalWrite(LED_BUILTIN,LOW);
  }
}
