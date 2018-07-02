#include <TimerOne.h>

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
int target = 0;

//initial settings vor pressure sensors
int mode = 0; //timer for sensor change
int sensorValue = 0;
float voltage = 0;
int side = 0; // 0 for right, 1 for left

bool bool_sensor1 = false;
bool bool_sensor2 = false;

//maps the order of the LEDs being turned on to their position in the "matrix"
int mapping[5][6] = { \
    { 0, 24, 25, 13, 22, 27}, \
    {21, 10,  2,  7, 19, 16}, \
    {17,  5,  1,  4,  9, 23}, \
    {25, 12,  3,  6, 11, 29}, \
    {20, 14,  8, 18, 26, 28}  \
};

int state[5][6] = { \
    {0, 0, 0, 0, 0, 0},  \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
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
  //read state of the rotary switch
  if(analogRead(A1)==1023){
    Serial.println("Reset");
    repetitions=0;
  }
  if(analogRead(A2)==1023){
    target = 10;
  } else if(analogRead(A3)==1023){
    target = 27;
  } else if(analogRead(A4)==1023){
    target = 54;
  } else if(analogRead(A5)==1023){
    target = 108;
  }
  Serial.println(target);
  if(target>0){
    state[0][0]=1;
  }
}
  
void timerIsr() {
  //control LEDs
  for(int i = 0; i<5; i++){
    for(int j = 0; j<6; j++){
      if(state[i][j] > 0){ // LED is supposed to glow
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
      }
    }
  }
}
