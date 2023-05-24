
// X motor (the belt)
//#define STEP_PIN_X 2 //X.STEP
//#define DIR_PIN_X 5 // X.DIR
// Y motor (the distribuitor)
#define STEP_PIN_Y 3 // step pin for Y-axis
#define DIR_PIN_Y 6 // direction pin for Y-axis
#define ON 1



 // joystick
int vrx = A0; 
int vry = A1; 
int vrx_data = 0; 
int vry_data = 0; 
double distribuitorAngle = 0;
int x = 0; 
int SMSpeed = 0; // Stepper Motor Speed 
void setup() {
  // Sets the two pins as Outputs
  Serial.begin(115200);

  pinMode(DIR_PIN_Y, OUTPUT);
  pinMode(STEP_PIN_Y, OUTPUT);
//      
//  pinMode(STEP_PIN_X,OUTPUT); 
//  pinMode(DIR_PIN_X,OUTPUT);
  pinMode(vrx , INPUT); 
  pinMode(vry, INPUT); 
}

 
void loop(){  
//  joystick(); 
  if(ON){
    if(Serial.available() > 0){
    String message = Serial.readStringUntil('\n');
     

    rotateDistribuitor(message.toDouble());
    }
    
  }

delay(1000);
}

void rotateDistribuitor(double angle){

  if (angle < 0){
    angle = angle*(-1);
    digitalWrite(DIR_PIN_Y, LOW); // set direction (HIGH or LOW)
  }else{
    digitalWrite(DIR_PIN_Y, HIGH); // set direction (HIGH or LOW)
  }

  int steps = (200.0 / 360) * angle;
  Serial.println(steps);
  // Pulse the stepper motor driver the appropriate number of times
  for(int i=0; i<steps; i++) {
    digitalWrite(STEP_PIN_Y, HIGH);
    delayMicroseconds(1000); // this delay determines the speed
    digitalWrite(STEP_PIN_Y, LOW);
    delayMicroseconds(1000);
  }




}

//
//
//void joystick(){
//  vrx_data = analogRead(vrx);
//  Serial.print("Vrx:"); 
//  Serial.println(vrx_data); 
//
//
//  // to stop the stepper motor
//  if ( (vrx_data > 490)  &&   (vrx_data < 510)   )
//  {
//  digitalWrite(STEP_PIN_X,LOW); 
//  delayMicroseconds(SMSpeed);   
//  }
//
//
//  if ( vrx_data > 700  )
//  {
//
//  digitalWrite(DIR_PIN_X,HIGH);
//  x = x + 1; 
//  digitalWrite(STEP_PIN_X,HIGH); 
//  delayMicroseconds(100*SMSpeed); 
//  digitalWrite(STEP_PIN_X,LOW); 
//  delayMicroseconds(SMSpeed); 
//    
//  }
//
//  if ( vrx_data < 300   )
//  {
//  digitalWrite(DIR_PIN_X,LOW);
//  x = x - 1; 
//
//  digitalWrite(STEP_PIN_X,HIGH); 
//  delayMicroseconds(100*SMSpeed); 
//    digitalWrite(STEP_PIN_X,LOW); 
//    delayMicroseconds(SMSpeed);  
//  }
//
//}
