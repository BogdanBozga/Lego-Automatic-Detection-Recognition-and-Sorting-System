#include <LinkedList.h>
#include <AccelStepper.h>

// X motor (the belt)
 #define STEP_PIN_X 2 //X.STEP
 #define DIR_PIN_X 5 // X.DIR
 // Y motor (the distribuitor)
 #define STEP_PIN_Y 3 // step pin for Y-axis
 #define DIR_PIN_Y 6 // direction pin for Y-axis

//
//#define STEP_PIN_X 3 //X.STEP
//#define DIR_PIN_X 6 // X.DIR
//// Y motor (the distribuitor)
//#define STEP_PIN_Y 2 // step pin for Y-axis
//#define DIR_PIN_Y 5 // direction pin for Y-axis

enum State {
  IDLE,
  RECEIVING_DATA,
  PROCESSING_DATA
};

struct piece_coordonate
{
  String label;
  double distance;
};


int ON = 0;
#define JS_PIN_X A0 
#define JS_PIN_Y A1 

#define motorInterfaceType 1

LinkedList<String> categorys=LinkedList<String>();



double distribuitorAngle = 0;
int beltSpeed = 10; // Stepper Motor Speed 
AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, STEP_PIN_Y, DIR_PIN_Y);


void setup() {
  // Sets the two pins as Outputs
  Serial.begin(115200);

  pinMode(DIR_PIN_Y, OUTPUT);
  pinMode(STEP_PIN_Y, OUTPUT);
   
  pinMode(STEP_PIN_X,OUTPUT); 
  pinMode(DIR_PIN_X,OUTPUT);
  pinMode(JS_PIN_X , INPUT); 
  pinMode(JS_PIN_Y, INPUT); 

  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH); 
  pinMode(13, INPUT);
//
//
//  stepper.setMaxSpeed(600.0);
//  stepper.setAcceleration(500.0);

}

 
void loop(){  
  checkON();
  if(ON){
    automaticControl();
  }else{
    manualControl(); 
  }
delay(10);
}



void checkON(){
  if(digitalRead(13) == HIGH){
    ON = 1;
  }else{
    ON = 0;
  }  
}

State currentState = IDLE;
String receivedData;

void moveBelt(int direction = HIGH){ //HIGH means forward
    digitalWrite(DIR_PIN_X,direction);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(beltSpeed); 
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(beltSpeed);  
}

// overhault for take into account manual movement
void automaticControl(){
  switch (currentState) {
    case IDLE:
     moveBelt();
      if (Serial.available() > 0) {
        receivedData = Serial.readStringUntil('\n');
        currentState = RECEIVING_DATA;
      }
      break;
    case RECEIVING_DATA:
      if (Serial.available() > 0) {
        receivedData = Serial.readStringUntil('\n');
      }
      if (receivedData.endsWith("END")) {
        currentState = PROCESSING_DATA;
      }
      break;
      
    case PROCESSING_DATA:

          int index1 = receivedData.indexOf(':');
          if(index1 > 0) {
            String category = receivedData.substring(0, index1);
            receivedData.remove(0, index1 + 1);
            index1 = receivedData.indexOf(':');
            String pieceCoordonale = receivedData.substring(0,index1);
            calculateAngle(category);
          }
          int index2 = receivedData.indexOf(',');
          if(index2 > 0){
            while (index2 >= 0) {
              categorys.add(receivedData.substring(0, index2));
              receivedData.remove(0, index2 + 1);
              index2 = receivedData.indexOf(',');
            }
          }
      receivedData = "";
      currentState = IDLE;
      break;
  }
}


double calculateAngle(String category){

  int nr_category = categorys.size();
  int destinationAngle = 0;
  for (int i = 0; i < categorys.size(); i++) {
    if(category.equals(categorys.get(i))){
      destinationAngle = 360/categorys.size()*(i+1);
      break;
    }
  }

  double angle = 0;
  angle = destinationAngle - distribuitorAngle;

  // if (distribuitorAngle > 180){
  //   if(destinationAngle <= distribuitorAngle)
  //     angle = destinationAngle - distribuitorAngle;
  //   else{
  //     angle = -1*distribuitorAngle+(destinationAngle-360);  
  //   }
  // }else if(distribuitorAngle < -180){
  //   if(destinationAngle <= abs(distribuitorAngle)){
  //     angle = -1*distribuitorAngle+destinationAngle;  
  //   }else{
  //     angle = -1*distribuitorAngle + destinationAngle;
  //   }
  // }else{
  //   if(distribuitorAngle>0){
  //     angle= destinationAngle - distribuitorAngle;
  //   }else{
  //     angle = -1*(360-(destinationAngle+abs(distribuitorAngle)));
  //   }
  // }
  rotateDistribuitor(angle);
  return 0;
}

void moveDis(int direction = HIGH){ //HIGH means forward
    digitalWrite(DIR_PIN_Y,direction);
    digitalWrite(STEP_PIN_Y,HIGH); 
    delayMicroseconds(beltSpeed); 
    digitalWrite(STEP_PIN_Y,LOW); 
    delayMicroseconds(beltSpeed);  
}


void rotateDistribuitor(double angle){
  distribuitorAngle = distribuitorAngle + angle;
//  Serial.print("Angle ");
//  Serial.print(angle);
//Serial.print(" and steps ");
  int steps = (200.0 / 360) * abs(angle);
  if (angle < 0){
    // digitalWrite(DIR_PIN_Y, LOW); 
    for(int i = 0;i<steps;i++){
      moveDis(LOW);
      delay(10);
    }
  }else{
      for(int i = 0;i<steps;i++){
      moveDis(HIGH);
      delay(10);
    }

  }

  // for(int i = 0;i<steps;i++){
  //   moveDis();
  //   delay(10);
  // }
  // Serial.println("--");
//  Serial.println(steps);
//   stepper.move(steps);
//  // // Run to the target position at the maximum speed set in setup()
//  stepper.runToPosition();
/*
  
  if (angle < 0){
    angle = angle*(-1);
    digitalWrite(DIR_PIN_Y, LOW); 
  }else{
    digitalWrite(DIR_PIN_Y, HIGH); 
  }

  int steps = (200.0 / 360) * angle;
    Serial.println(steps);
//  Serial.println(steps);

  for(int i=0; i<steps; i++) {
    digitalWrite(STEP_PIN_Y, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN_Y, LOW);
    delayMicroseconds(500);
  }
  */
}



void manualControl(){
  int x_direction = analogRead(JS_PIN_X);
  int y_direction = analogRead(JS_PIN_Y);

  // belt movement
  if ( (x_direction > 490)  &&   (x_direction < 510)){
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(beltSpeed);   
  }else if( x_direction > 700  ){
    digitalWrite(DIR_PIN_X,HIGH);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(beltSpeed); 
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(beltSpeed);  
  }else if( x_direction < 300){
    digitalWrite(DIR_PIN_X,LOW);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(beltSpeed); 
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(beltSpeed);  
  }

  // distribuitor movment

  if ( (y_direction > 490)  &&   (y_direction < 510)){
    // digitalWrite(STEP_PIN_Y,LOW);  
    delayMicroseconds(beltSpeed);   
  }else if( y_direction > 700  ){
    rotateDistribuitor(10);

    // digitalWrite(DIR_PIN_X,HIGH);
    // digitalWrite(STEP_PIN_Y,HIGH); 
    // delayMicroseconds(beltSpeed); 
    // digitalWrite(STEP_PIN_Y,LOW); 
    delayMicroseconds(beltSpeed);  
  }else if( y_direction < 300){
    rotateDistribuitor(-10);

    // digitalWrite(DIR_PIN_Y,LOW);
    // digitalWrite(STEP_PIN_Y,HIGH); 
    // delayMicroseconds(beltSpeed); 
    // digitalWrite(STEP_PIN_Y,LOW); 
    delayMicroseconds(beltSpeed);  
  }
}
