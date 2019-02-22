#define LED_PIN 13
void setup() {
  // put your setup code here, to run once:
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(6)==LOW)
  {
    digitalWrite(LED_PIN,HIGH);
  }
  if (digitalRead(7)==LOW)
  {
    digitalWrite(LED_PIN,LOW);
  }
}
