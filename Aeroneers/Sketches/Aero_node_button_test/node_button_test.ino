void setup() {
  // put your setup code here, to run once:
pinMode(7, INPUT);
pinMode(8,INPUT);
pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(7)==HIGH)
{
  digitalWrite(13,HIGH);
}
else if (digitalRead(7)==LOW)
{
  digitalWrite(13,LOW);
}
if (digitalRead(8)==HIGH)
{
  digitalWrite(LED_BUILTIN,HIGH);
}
else if (digitalRead(8)==LOW)
{
  digitalWrite(LED_BUILTIN,LOW);
}
}
