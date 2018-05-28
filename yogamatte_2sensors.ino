// the six digital pins for s pins
// code for 2 Multiplexers to handle 2 pressure sensors

/**********************************************/
int r0 = 0;      
int r1 = 0;       
int r2 = 0;

/**********************************************/
int sensorValue = 0;
float voltage = 0;
int counter = 0;
bool bool_sensor = false;

int  bin [] = {000, 001}; //list of binary values
int selectIn = 0;
int selectOut = 0;

/**********************************************/

void setup() {
  pinMode(2, OUTPUT);    // r0
  pinMode(3, OUTPUT);    // r1
  pinMode(4, OUTPUT);    // r2
  pinMode(5, OUTPUT);    // r3
  pinMode(6, OUTPUT);    // r4
  pinMode(7, OUTPUT);    // r5
  
  Serial.begin(9600);
}

/**********************************************/
void loop() {
  for (int mode=0; mode < 2; mode++) { //switches between left and right sensor
    
   int state = bin[mode]; //gets settings for s pins 
    
   r0 = bitRead(state,0); 
   r1 = bitRead(state,1); 
   r2 = bitRead(state,2); //same settings on both multiplexers
 
   digitalWrite(2, r0); // send the bits to the digital pins 
   digitalWrite(3, r1);
   digitalWrite(4, r2);
   digitalWrite(5, r0);  
   digitalWrite(6, r1);
   digitalWrite(7, r2);
       
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
  if(mode == 0){
    Serial.print("left");
  } else {
    Serial.print("right");
  }
  
  Serial.print(": ");
  Serial.print(voltage);
  Serial.print(" - ");
  Serial.println(counter);
  delay(1000);
    
 }
}
