//1 multiplexer handles switch between 2 pressure sensors (velostat)
//2 multiplexers handle "matrix" of 30 LEDs

/**********************************************/
//bits for multiplexer 1
int r0 = 0;      
int r1 = 0;       
int r2 = 0;
//bits for multiplexer 2
int r3 = 0;      
int r4 = 0;       
int r5 = 0;
//bits for multiplexer 3
int r6 = 0;      
int r7 = 0;       
int r8 = 0;

/**********************************************/
//count repetitions
int repetitions = 0;

//initial settings vor pressure sensors
int mode = 0; //timer for sensor change
int sensorValue = 0;
float voltage = 0;
int side = 0; // 0 for right, 1 for left

bool bool_sensor1 = false;
bool bool_sensor2 = false;

//maps the order of the LEDs being turned on to their position in the "matrix"
int mapping[8][8] = { \
    {27, 25, 13, 11, 21, 24,  0,  0},  \
    {22,  7,  2,  6, 18, 15,  0,  0}, \
    {16,  5,  0,  3,  9, 20,  0,  0}, \
    {26, 10,  1,  4, 12,  0,  0,  0}, \
    {19, 14,  8, 17,  0,  0,  0,  0}, \
    {23,  0,  0,  0,  0,  0,  0,  0}, \
    { 0,  0,  0,  0,  0,  0, 29,  0}, \
    { 0,  0,  0,  0,  0,  0,  0, 30} \
};

//values read from the mapping matrix for new counter
int newRow;
int newCol;

int state[8][8] = { \
    {0, 0, 0, 0, 0, 0, 0, 0},  \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0} \
};

//input states for the multiplexor address select pins 
int  bin [] = {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111}; //list of binary values
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
  // change sensor every second
  int timer = round(millis()/1000);
  if(timer % 2 == 0){
    mode = 0;
    Serial.println("left");
    //measure first sensor
  } else {
    mode = 1;
    Serial.println("right");
    //measure second sensor
  }

  // get settings for sensor select pins 
  int state = bin[mode]; 
  r0 = bitRead(state,0); 
  r1 = bitRead(state,1); 
  r2 = bitRead(state,2);
  
  // send the bits to the digital pins
  digitalWrite(2, r0);  
  digitalWrite(3, r1);
  digitalWrite(4, r2);
       
  // read analog pin and convert value to voltage
  sensorValue = analogRead(A0);
  voltage = sensorValue * (5.0 / 1023.0);
  if(voltage == 5.00){
    if(mode == 0){
      if(bool_sensor1 == false){
        bool_sensor1 = true;
      }
    } else {
      //mode == 1
      if(bool_sensor1 == false){
        bool_sensor1 = true;
      }  
    }  
  } else { //voltage < 5
    if(bool_sensor1 && bool_sensor2){ //coming out of firm stand
      countUp();
      if(mode == 0){
        bool_sensor1 = false;
        side = 0; //right foot moved first
      } else {
        bool_sensor2 = false;
        side = 1; // left foot moved first
      }
    } else { 
      // some other reason for no pressure on sensore
      if(mode == 0){
        bool_sensor1 = false;
      } else {
        bool_sensor2 = false;
      }
    }
  }
  // print voltage and counter:
  if(mode == 0){
    //Serial.print("A0");
  } else {
    //Serial.print("A1");
  }
  
  delay(1000);
    
 }


void countUp() {
  ++repetitions;
  Serial.print("repetitions: ");
  Serial.println(repetitions);
  getNewMapping();
  
  state[newRow][newCol]=1;
  //print current matrix
  /*for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      Serial.print(state[i][j]);
    }
    Serial.println();
  }*/
}

void getNewMapping(){
  for(unsigned row = 0; row < 8; ++row) {
   for(unsigned col = 0; col < 8; ++col) {
      if(mapping[row][col] == repetitions) {
         newRow = row;
         newCol = col;
         return;
      }
   }
}
}

