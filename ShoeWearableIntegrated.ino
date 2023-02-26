char incomingByte;

//accel
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>


Adafruit_MMA8451 mma = Adafruit_MMA8451();

//direction needed
bool needNorth = true;
bool needSouth = false;
bool needWest = false;
bool needEast = false;

//current direction
int northState = 1;
int eastState = 2;
int southState = 3;
int westState = 4;

int currentState = 0;
// int goalDir = northState;

int mapX = 0;
int mapY = 0;
int rangeArray[] = {3};
int length = 0;
//leds
#include <FastLED.h>

#define NUM_LEDS 11
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

int neutralState = 0;
int rightGoState = 1;
int leftGoState = 2;
int stopState = 3;
int goState = 4;

int count = 0;

int counter = 0;

int lightState = neutralState; //initialize state

void setup() {

  // put your setup code here, to run once:
  FastLED.clear();
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(100);

  //accel
  Serial.begin(9600);
  
  Serial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
  
}

void loop() {
  //keyboard input
   //keyboard setup
  if (Serial.available()) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }
  // 49 = a = left, 67 = s = down, 52 = d = right, 71 = w = up 
  if (incomingByte == 'a'){
    //left
    lightState = rightGoState;
  } if (incomingByte == 'd'){
    //right
    lightState = leftGoState;
  } if (incomingByte == 'w'){
    //go straight
    lightState = goState;
  } if (incomingByte == 's'){
    //stop
    lightState = stopState;
  } if (incomingByte == ' '){
    lightState = neutralState;
  }
  // else
  // {
  // digitalWrite(13, LOW);
  // } 
  //accel
  mma.read();

  // if(counter<200){
    Serial.print("X:\t"); Serial.print(mma.x); 
    Serial.print("\tY:\t"); Serial.print(mma.y); 

    //Serial.print("orientation: "); Serial.print(mma.getOrientation());
    //0: Portrait Up Front
    // 1: Portrait Up Back
    // 2: Portrait Down Front
    // 3: Portrait Down Back
    // 4: Landscape Right Front
    // 5: Landscape Right Back
    // 6: Landscape Left Front
    // 7: Landscape Left Back
  // }
  
  //Serial.print("\tZ:\t"); Serial.print(mma.z); 
  Serial.println();
  // counter++;

  delay(100);
  
  // length = 0;
  // rangeArray[length]=mma.x;
  // length ++;
  // Serial.print("range"); Serial.print(rangeArray);

  //Serial.println("this is range"); Serial.print(mma.getRange()); 
  //   Serial.println();
  // mapX = map(mma.x,-1000,1000, 0,360);
  // mapX = constrain(mapX, 0, 360);
  // mapY = map(mma.y,-1000,1000, 0,360);
  // mapY = constrain(mapY, 0, 360);
  // Serial.print(" mapX "); Serial.print(mapX); 
  // Serial.print( "mapY "); Serial.print(mapY); 
  // Serial.println("Hello");

 //set currentState
  if((mma.x > -190 && mma.x < -1086) && (mma.y > 294 && mma.y < 2234)){
    currentState = northState;
    Serial.println("NORTH");
  };
  if((mma.x > -90 && mma.x < 134) && (mma.y > 568 && mma.y < 640)){
    currentState = eastState;
    Serial.println("EAST");
  };
  if((mma.x > -282 && mma.x < -166)&&(mma.y > 454 && mma.y < 704)){
    currentState = southState;
    Serial.println("SOUTH");
  };
  if((mma.x > -260 && mma.x < -28)&&(mma.y > 242 && mma.y < 436)){
    currentState = westState;
    Serial.println("WEST");
  };

 // sense north -  && mma.y >= 560 && mma.y <= 600

 // if needs to be north!
//  Serial.println(currentState);
//  Serial.println(needNorth);

  if (needNorth == true) {
    Serial.println("in east");
    if (currentState == northState) {
      lightState = stopState;
      delay(1000);
      lightState = neutralState ;
    }   
    if (currentState == westState){ 
      lightState = rightGoState;
    }
    if (currentState == southState){
      lightState = leftGoState;
    }
    if (currentState == eastState){
      lightState = leftGoState;
    }
  };

  //if needs to be east!
  if (needEast == true) {
    if (currentState == eastState) {
      lightState = stopState;
      delay(1000);
      lightState = neutralState ;
    }   
    if (currentState == westState){ // west but need north
      lightState = leftGoState;
    }
    if (currentState == southState){
      lightState == leftGoState;
    }
    if (currentState == northState){
      lightState == rightGoState;
    }
  };

  //if needs to be west!
  if (needWest == true) {
    if (currentState == westState) {
      lightState = stopState;
      delay(1000);
      lightState = neutralState ;
    }   
    if (currentState == eastState){ // west but need north
      lightState = leftGoState;
    }
    if (currentState == southState){
      lightState == rightGoState;
    }
    if (currentState == northState){
      lightState == leftGoState;
    }
  };

  //if needs to be south!
  if (needSouth == true) {
    if (currentState == southState) {
      lightState = stopState;
      delay(1000);
      lightState = neutralState ;
    }   
    if (currentState == eastState){ // west but need north
      lightState = rightGoState;
    }
    if (currentState == westState){
      lightState == leftGoState;
    }
    if (currentState == northState){
      lightState == leftGoState;
    }
  };

  //led
  if (lightState == neutralState){ //neutral
      FastLED.clear();
  } else if (lightState == rightGoState) { //left
      FastLED.clear();
      for(int i = 5; i < NUM_LEDS; i++) {
        leds[i] = CRGB(128,0,0);
        FastLED.show();
        delay(100);
      }
  } else if (lightState == leftGoState) { //right
      FastLED.clear();
      for(int i = 5; i >= 0; i--) {
        leds[i] = CRGB(128,0,0);
        FastLED.show();
        delay(100);
      }
  } else if (lightState == stopState) { //stop
      FastLED.clear();
      count = 0;
      for(int i = 0; i <= 5; i++) {
        if (count == 0){
          leds[5] = CRGB(0,128,0);
        }
        if (count == 1){
          leds[4] = CRGB(0,128,0);
          leds[6] = CRGB(0,128,0);
        }
        if (count == 2){
          leds[3] = CRGB(0,128,0);
          leds[7] = CRGB(0,128,0);
        }
        if (count == 3){
          leds[2] = CRGB(0,128,0);
          leds[8] = CRGB(0,128,0);
        }
        if (count == 4){
          leds[1] = CRGB(0,128,0);
          leds[9] = CRGB(0,128,0);
        }
        if (count == 5){
          leds[0] = CRGB(0,128,0);
          leds[10] = CRGB(0,128,0);
        }
        //leds[i+1*i] = CRGB(0,128,0) ;       
        FastLED.show();
        delay(100);
        count += 1;
      }
     
  } else if (lightState == goState) { 
      FastLED.clear();
      count = 0;
      for(int i = 0; i <= 5; i++) {
        if (count == 0){
          leds[5] = CRGB(128,0,0);
        }
        if (count == 1){
          leds[4] = CRGB(128,0,0);
          leds[6] = CRGB(128,0,0);
        }
        if (count == 2){
          leds[3] = CRGB(128,0,0);
          leds[7] = CRGB(128,0,0);
        }
        if (count == 3){
          leds[2] = CRGB(128,0,0);
          leds[8] = CRGB(128,0,0);
        }
        if (count == 4){
          leds[1] = CRGB(128,0,0);
          leds[9] = CRGB(128,0,0);
        }
        if (count == 5){
          leds[0] = CRGB(128,0,0);
          leds[10] = CRGB(128,0,0);
        }
        //leds[i+1*i] = CRGB(0,128,0) ;       
        FastLED.show();
        delay(100);
        count += 1;
      }
     
  }
  
  FastLED.clear();
  FastLED.show();
  delay(1000);
}
