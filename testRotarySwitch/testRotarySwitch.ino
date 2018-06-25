void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  state1 = analogRead(A1);
  state2 = analogRead(A2);
  state3 = analogRead(A3);
  state4 = analogRead(A4);

  Serial.print("state1: ");
  Serial.println(state1);
  Serial.print("state2: ");
  Serial.println(state2);
  Serial.print("state3: ");
  Serial.println(state3);
  Serial.print("state4: ");
  Serial.println(state4);
  
}
