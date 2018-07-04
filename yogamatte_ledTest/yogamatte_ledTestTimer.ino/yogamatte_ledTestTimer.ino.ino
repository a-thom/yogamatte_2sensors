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

//initial settings vor pressure sensors
int mode = 0; //timer for sensor change
int sensorValue = 0;
float voltage = 0;
int side = 0; // 0 for right, 1 for left

bool bool_sensor1 = false;
bool bool_sensor2 = false;

//maps the order of the LEDs being turned on to their position in the "matrix"
int mapping[5][6] = { \
    { 0, 24, 15, 13, 22, 27}, \
    {21, 10,  2,  7, 19, 16}, \
    {17,  5,  1,  4,  9, 23}, \
    {25, 12,  3,  6, 11, 29}, \
    {20, 14,  8, 18, 26, 28}  \
};

int state[5][6] = { \
    {1, 0, 0, 0, 0, 0},  \
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
  //on/off pin for 5V-multiplexer
  pinMode(12, OUTPUT);   
  
  Serial.begin(9600);
}

/**********************************************/
void loop() {
  Serial.println("on");

}
  
void timerIsr() {
  // switch multiplexer off
  digitalWrite(12, 1); 
  
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
