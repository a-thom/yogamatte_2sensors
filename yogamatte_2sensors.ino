// the six digital pins for s pins
// code for 2 Multiplexers to handle 2 pressure sensors

/**********************************************/
int r0 = 0;      
int r1 = 0;       
int r2 = 0;
int r3 = 0;      
int r4 = 0;     
int r5 = 0;

/**********************************************/
int sensorValue = 0;
float voltage = 0;
int counter = 0;
bool bool_sensor = false;

/**********************************************/

void setup() {
  Serial.begin(9600);
}

/**********************************************/
void loop() {
  // read analog pin:
  sensorValue = analogRead(A0);
  // convert analog value to voltage:
  voltage = sensorValue * (5.0 / 1023.0);

  if(voltage == 5){
    if(bool_sensor == false){
      bool_sensor = true;
      counter = counter + 1;
    }
  } else {
    bool_sensor = false;
  }
  // print voltage and counter:
  Serial.print(voltage);
  Serial.print(" - ");
  Serial.println(counter);
  delay(1000);
}
