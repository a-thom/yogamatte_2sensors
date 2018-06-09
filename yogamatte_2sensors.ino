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
bool bool_sensor1 = false;
bool bool_sensor2 = false;

int mapping[8][8] = { \
    {27, 25, 13, 11, 21, 24, 0, 0},  \
    {22, 7, 2, 6, 18, 15, 0, 0}, \
    {16, 5, 00, 3, 9, 20, 0, 0}, \
    {26, 10, 1, 4, 12, 0, 0, 0}, \
    {19, 14, 8, 17, 0, 0, 0, 0}, \
    {23, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 29, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 30} \
};

//values read from the mapping matrix for new counter
int newRow;
int newCol;

int state[8][8] = { \
    {0, 0, 0, 0, 0, 0, 0, 0},  \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 1, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0} \
};

//states for the Sensor 
int  bin [] = {0b000, 0b001}; //list of binary values
/**********************************************/

void setup() {
  //pins for the sensor multiplexer
  pinMode(2, OUTPUT);    // r0
  pinMode(3, OUTPUT);    // r1
  pinMode(4, OUTPUT);    // r2
  //pins for the LED 5V multiplexer
  pinMode(5, OUTPUT);    // r0
  pinMode(6, OUTPUT);    // r1
  pinMode(7, OUTPUT);    // r2
  //pins for the LED ground multiplexer
  pinMode(8, OUTPUT);    // r0
  pinMode(9, OUTPUT);    // r1
  pinMode(10, OUTPUT);    // r2
  
  Serial.begin(9600);
}

/**********************************************/
void loop() {
  //Serial.println(mapping[2][3]);
  for (int mode=0; mode < 2; mode++) { //switches between left and right sensor
    
   int state = bin[mode]; //gets settings for s pins 
    
   r0 = bitRead(state,0); 
   r1 = bitRead(state,1); 
   r2 = bitRead(state,2); //same settings on both multiplexers
 
   digitalWrite(2, r0); // send the bits to the digital pins 
   digitalWrite(3, r1);
   digitalWrite(4, r2);
       
   // read analog pin:
   sensorValue = analogRead(A0);
   // convert analog value to voltage:
   voltage = sensorValue * (5.0 / 1023.0);

  if(voltage == 5.00){
    if(mode == 0){
      if(bool_sensor1 == false){
        bool_sensor1 = true;
        countUp();
      } else {
        bool_sensor1 = false;
      }
    } else {
      if(bool_sensor1 == false){
        bool_sensor1 = true;
        countUp();
      } else {
        bool_sensor1 = false;
      }  
    }  
  }  
  // print voltage and counter:
  if(mode == 0){
    //Serial.print("A0");
  } else {
    //Serial.print("A1");
  }
  
  /*Serial.print(": ");
  Serial.print(voltage);
  Serial.print(" - ");
  Serial.println(counter); */
  delay(2000);
  countUp();
    
 }
}

void countUp() {
  ++counter;
  Serial.print("counter: ");
  Serial.println(counter);
  getNewMapping();
  
  state[newRow][newCol]=1;
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      Serial.print(state[i][j]);
    }
    Serial.println();
  }
}

void getNewMapping(){
  for(unsigned row = 0; row < 8; ++row) {
   for(unsigned col = 0; col < 8; ++col) {
      if(mapping[row][col] == counter) {
         newRow = row;
         newCol = col;
         return;
      }
   }
}
}

