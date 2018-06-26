void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  float state1 = analogRead(A1);
  float state2 = analogRead(A2);
  float state3 = analogRead(A3);
  float state4 = analogRead(A4);
  float state5 = analogRead(A5);
  
  // put your main code here, to run repeatedly:
  Serial.print("state1: ");
  Serial.println(state1);
  Serial.print("state2: ");
  Serial.println(state2);
  Serial.print("state3: ");
  Serial.println(state3);
  Serial.print("state4: ");
  Serial.println(state4);
  Serial.print("state5: ");
  Serial.println(state5);
  Serial.println();
  delay(2000);
  
}
