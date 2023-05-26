//#include <vector>

// X motor (the belt)
#define STEP_PIN_X 2 //X.STEP
#define DIR_PIN_X 5 // X.DIR
// Y motor (the distribuitor)
#define STEP_PIN_Y 3 // step pin for Y-axis
#define DIR_PIN_Y 6 // direction pin for Y-axis
#define ON 0
#define JS_PIN_X A0 
#define JS_PIN_Y A1

//std::vector<String> categorys;

double distribuitorAngle = 0;
int beltSpeed = 10; // Stepper Motor Speed 


void moveBelt();

void setup() {
  // Sets the two pins as Outputs
  Serial.begin(115200);

  pinMode(DIR_PIN_Y, OUTPUT);
  pinMode(STEP_PIN_Y, OUTPUT);
     
  pinMode(STEP_PIN_X,OUTPUT); 
  pinMode(DIR_PIN_X,OUTPUT);
  pinMode(JS_PIN_X , INPUT); 
  pinMode(JS_PIN_Y, INPUT); 
}

 
void loop(){  

  if(ON){
//    automaticControl();
    
    
  }else{
    manualControl(); 
  }

delay(10);
}

/*
void automaticControl(){
  moveBelt();
  if(Serial.available() > 0){
    String message = Serial.readStringUntil('\n');

    int commaIndex = message.indexOf(',');
    if(commaIndex > 0) {
      String category = message.substring(0, commaIndex);
      String pieceCoordonale = message.substring(commaIndex + 1);
      calculateAngle(category);
    }

  }
}

*/


/*
double calculateAngle(String category){

//  int nr_category = categorys.size()+1;
  int destinationAngle = 0;
  for (int i = 0; i < categorys.size(); i++) {
    if(category.equals(categorys[i])){
      destinationAngle = 360/nr_category*(i+1);
      break;
    }
  }

  double angle = 0;

  if (distribuitorAngle > 180){
    if(destinationAngle <= distribuitorAngle)
      angle = destinationAngle - distribuitorAngle;
    else{
      angle = -1*distribuitorAngle+(destinationAngle-360);  
    }
  }else if(distribuitorAngle < -180){
    if(destinationAngle <= abs(distribuitorAngle)){
      angle = -1*distribuitorAngle+destinationAngle;  
    }else{
      angle = -1*distribuitorAngle + destinationAngle;
    }
  }else{
    if(distribuitorAngle>0){
      angle= destinationAngle - distribuitorAngle;
    }else{
      angle = -1*(360-(destinationAngle+abs(distribuitorAngle)))
    }
  }
  rotateDistribuitor(angle);
  return 0;
}

void moveBelt(int direction = HIGH){ //HIGH means forward
    digitalWrite(DIR_PIN_X,direction);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(beltSpeed); 
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(beltSpeed);  
}
*/
void rotateDistribuitor(double angle){
  distribuitorAngle = distribuitorAngle + angle;
  if (angle < 0){
    angle = angle*(-1);
    digitalWrite(DIR_PIN_Y, LOW); 
  }else{
    digitalWrite(DIR_PIN_Y, HIGH); 
  }

  int steps = (200.0 / 360) * angle;
  Serial.println(steps);

  for(int i=0; i<steps; i++) {
    digitalWrite(STEP_PIN_Y, HIGH);
    delayMicroseconds(1000); 
    digitalWrite(STEP_PIN_Y, LOW);
    delayMicroseconds(1000);
  }
}



void manualControl(){

  initializeCategorys();


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

void initializeCategorys(){

  if(Serial.available() > 0){
    String message = Serial.readStringUntil('\n');


    int commaIndex = message.indexOf(',');
    while (commaIndex > 0) {
      String category = message.substring(0, commaIndex);
//      categorys.push_back(category);

      message = message.substring(commaIndex + 1);
      commaIndex = message.indexOf(',');
    }
  }

}
//
//double abs(double nr){
//  if(nr<0){
//    return -1*nr;
//  }
//  return nr;
//}
