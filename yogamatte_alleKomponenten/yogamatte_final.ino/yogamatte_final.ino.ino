#include <TimerOne.h>

//1 multiplexer handles switch between 2 pressure sensors (velostat)
//2 multiplexers handle "matrix" of 30 LEDs

/**********************************************/
//bits for multiplexer 1
int r0 = 0;      
//bits for multiplexer 2
int r3 = 0;      
int r4 = 0;       
int r5 = 0;
//bits for multiplexer 3
int r6 = 0;      
int r7 = 0;       
int r8 = 0;

/**********************************************/

//initial settings vor pressure sensors
double threshold_sensor1 = 5;
double threshold_sensor2 = 5;
bool calib_sensor1 = false;
bool calib_sensor2 = false;
double dist = 0.3;

int frequency = 25;
int mode = 0; //timer for sensor change
int sensorValue = 0;
float voltage = 0;
bool bool_sensor1 = false;
bool bool_sensor2 = false;
bool bool_sensor1_m = false; //memory for last read
bool bool_sensor2_m = false;
bool start = true;

//count repetitions
int repetitions = 0;
int target = 0;

//maps the order of the LEDs being turned on to their position in the "matrix"
int mapping[5][6] = { \
    {30, 24, 15, 13, 22, 27}, \
    {21, 10,  2,  7, 19, 16}, \
    {17,  5,  1,  4,  9, 23}, \
    {25, 12,  3,  6, 11, 29}, \
    {20, 14,  8, 18, 28, 26}  \
};

int state[5][6] = { \
    {0, 0, 0, 0, 0, 0},  \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 1, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 1}, \
    {0, 0, 0, 0, 0, 0} \
};

//input states for the multiplexor address select pins 
int  bin [] = {0b000, 0b001, 0b010, 0b011, 0b100, 0b101}; //list of binary values
/**********************************************/

void setup() {
  // set a timer in microseconds
  Timer1.initialize(1); 
  // attach the service routine
  Timer1.attachInterrupt( timerIsr ); 
  
  //pins for the sensor multiplexer
  pinMode(2, OUTPUT);    // r0
  pinMode(3, OUTPUT);    // r1
  pinMode(4, OUTPUT);    // r2
  
  //pins for the LED ground multiplexer
  pinMode(5, OUTPUT);    // r0
  pinMode(6, OUTPUT);    // r1
  pinMode(7, OUTPUT);    // r2
  //pins for the LED 5V multiplexer
  pinMode(8, OUTPUT);    // r0
  pinMode(9, OUTPUT);    // r1
  pinMode(10, OUTPUT);   // r2
  //on/off pin for LED 5V-multiplexer
  pinMode(12, OUTPUT);  

  //pins for the rotary switch
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  
  Serial.begin(9600);
}

/**********************************************/
void loop() {
  delay(frequency);
   //switch between left and right
  if(mode == 0){
    mode=1;
  } else {
    mode=0;
  }

  //gets settings for sensor switch pins 
  int setting = bin[mode]; 
  r0 = bitRead(setting, 0); 
  
  // send the bits to the digital pins
  digitalWrite(2, r0);  
  digitalWrite(3, 0);
  digitalWrite(4, 0);
  
  // read analog pin:
  sensorValue = analogRead(A0);
  // convert analog value to voltage:
  voltage = sensorValue * (5.0 / 1023.0);

  //calibrate sensors on start
  if(mode == 0 && calib_sensor1 == false){
    if(voltage < 4.6){
      threshold_sensor1 = voltage + dist;
    } else {
      threshold_sensor1 = 4.9;
    }
    calib_sensor1 = true;
    Serial.print("threshold A0: ");
    Serial.println(threshold_sensor1);
  } else if(mode == 1 && calib_sensor2 == false){
    if(voltage < 4.6){
      threshold_sensor2 = voltage + dist;
    } else {
      threshold_sensor2 = 4.9;
    }
    calib_sensor2 = true;
    Serial.print("threshold A1: ");
    Serial.println(threshold_sensor2);
  }
   
  //read state of the rotary switch
  if(analogRead(A2)==1023){
    target = 10;
    //Serial.println("target 10");
  } else if(analogRead(A3)==1023){
    target = 27;
  } else if(analogRead(A4)==1023){
    target = 54;
  } else if(analogRead(A5)==1023){
    target = 108;
  } else if(analogRead(A1)==1023){
    Serial.println("Reset");
    //reset counter and settings
    repetitions = 0;
    target=0;
    for(int i=0; i<5; i++){
      for(int j=0; j<6; j++){
        state[i][j]=0;
      }
    }
    state[0][0]=1; 
    //calibrate sensors on reset
    if(mode == 0){
      if(voltage < 4.6){
        threshold_sensor1 = voltage + dist;
      } else {
        threshold_sensor1 = 5;
      }
      calib_sensor1 = true;
      Serial.print("threshold A0: ");
      Serial.println(threshold_sensor1);
    } else if(mode == 1){
      if(voltage < 4.6){
        threshold_sensor2 = voltage + dist;
      } else {
        threshold_sensor2 = 5;
      }
      calib_sensor2 = true;
      Serial.print("threshold A1: ");
      Serial.println(threshold_sensor2);
    }
  }

  //switch from reset mode to start
  if(target>0 && state[0][0]==1){
    Serial.print("target: ");
    Serial.println(target);
    state[3][5]=1;   
    state[2][2]=1;
    state[0][0]=0;
  } 
  
  //interpret sensor values
  if(mode == 0){
    if(voltage >= threshold_sensor1){
      if(bool_sensor1 == false){
        if(bool_sensor1_m ==true){
          bool_sensor1 = true;
          if(bool_sensor2==true){
            //entering tadasana
            countUp();
          }
        } else {
          bool_sensor1_m = true;
        }  
      }  
    } else { //sensor value below threshold
      if(bool_sensor1 == true){
        if(bool_sensor1_m == false){
          bool_sensor1 = false;
        } else {
          bool_sensor1_m = false;
        }
      }
    }
  } else {  //mode == 1
    if(voltage >= threshold_sensor2){
      if(bool_sensor2 == false){
        if(bool_sensor2_m ==true){
          bool_sensor2 = true;
          //bool_sensor2_m = false;
          if(bool_sensor1==true){
            //entering tadasana
            countUp();
          }
        } else {
          bool_sensor2_m = true;
        }  
      }  
    } else { //sensor value below threshold
      if(bool_sensor2 == true){
        if(bool_sensor2_m == false){
          bool_sensor2 = false;
        } else {
          bool_sensor2_m = false;
        }
      }
    }
  }    
  
  
  // print voltage and counter:
//  if(mode == 0){
//    Serial.print("A0: ");
//  } else {
//    Serial.print("A1: ");
//  }
//  Serial.println(voltage);
  
}
  
void timerIsr() {
  // switch multiplexer off
  digitalWrite(12, 1); 

    //print current matrix
  /*for(int i=0; i<5; i++){
     for(int j=0; j<6; j++){
       Serial.print(state[i][j]);
     }
     Serial.println();
   }*/
   
  //control LEDs
  for(int i = 0; i<5; i++){
    for(int j = 0; j<6; j++){
      if(state[i][j] > 0){ // LED is supposed to glow
        // switch multiplexer off
        digitalWrite(12, 1);
        // get pin settings for sensor select pins 
        int set5V = bin[j]; 
        int setGround = bin[i];
        //write to bits
        r3 = bitRead(setGround,0); 
        r4 = bitRead(setGround,1); 
        r5 = bitRead(setGround,2);
        r6 = bitRead(set5V,0); 
        r7 = bitRead(set5V,1); 
        r8 = bitRead(set5V,2);
        // send the bits to the digital pins
        digitalWrite(5, r3);  
        digitalWrite(6, r4);
        digitalWrite(7, r5);
        digitalWrite(8, r6);  
        digitalWrite(9, r7);
        digitalWrite(10,r8);
        // switch multiplexer on
        digitalWrite(12, 0);
        //delay(100);
      }
    }
   }
}

void countUp() {
  //Serial.println("countUp");
  state[0][0]=1;
  if(start == true){
    start = false;
    //Serial.println("start");
  } else {
    ++repetitions;
    //Serial.print("repetitions: ");
    Serial.println(repetitions);
    for(unsigned row = 0; row < 5; ++row) {
      for(unsigned col = 0; col < 6; ++col) {
         if(mapping[row][col] <= repetitions*27/target) {
            state[row][col]=1;
         }
      }
    }
  
    //switch on/off the blue lights to signal side
    state[3][5]=repetitions % 2;
    state[4][4]=(repetitions+1) % 2; 
    state[0][0]=0;  
     
    } 
 }
 
 

