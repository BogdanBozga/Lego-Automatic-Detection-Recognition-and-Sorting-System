#include <LinkedList.h>
// X motor (the belt)
 #define STEP_PIN_X 2 //X.STEP
 #define DIR_PIN_X 5 // X.DIR
 // Y motor (the distribuitor)
 #define STEP_PIN_Y 3 // step pin for Y-axis
 #define DIR_PIN_Y 6 // direction pin for Y-axis


#define JS_PIN_X A0 
#define JS_PIN_Y A1 
enum State {
  IDLE,
  RECEIVING_DATA,
  PROCESSING_DATA
};

struct piece_coordonate
{
  String *label;
  int *distance;
};

int box_edge_distance = 150; // milimeter
int motorSteps = 0;
LinkedList<String> categorys=LinkedList<String>();
// LinkedList<piece_coordonate*> pieces = LinkedList<piece_coordonate*>();
LinkedList<String> pieces_labels = LinkedList<String>();
LinkedList<int> pieces_distance = LinkedList<int>();

int ON = 0;

double distribuitorAngle = 0;
int beltSpeed = 10; // Stepper Motor Speed 
int speed_delay = 10;



void setup() {
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

void moveBelt(int direction = HIGH){ //HIGH means forward , //each call represents a step
    digitalWrite(DIR_PIN_X,direction);
    digitalWrite(STEP_PIN_X,HIGH); 
    delayMicroseconds(beltSpeed); 
    digitalWrite(STEP_PIN_X,LOW); 
    delayMicroseconds(beltSpeed);  

    if(direction == HIGH){
      motorSteps += 1;
    }else{
      motorSteps -= 1;
    }

}

void magic(){
  if(motorSteps >= 50){ //50 steps = 10 milimeters
    motorSteps -= 50;
    if(pieces_distance.size() > 0){
      for(int i=0; i<pieces_distance.size();i++){
//        piece_coordonate *piece = pieces.get(i); // get the piece
        // pieces.remove(i); // remove the piece from the list
        // distance = pieces_distance.get(i) - 10;
        // piece_distance.remove()
        pieces_distance.set(i,pieces_distance.get(i) - 10); // modify the piece
        Serial.print(pieces_distance.get(i));
        Serial.print("--");
        // pieces.remove(i);
        // pieces.add(i, piece); // insert the piece back into the list
        Serial.print(pieces_distance.size());
        Serial.print(" || ");
      }
      Serial.print("   --->");
      Serial.print(pieces_distance.get(0));
      Serial.println(" <---");

      if(pieces_distance.get(0) <= 10){
        calculateAngle(pieces_labels.get(0));
        if(pieces_distance.get(0) <= 0){
          pieces_distance.shift();
          pieces_labels.shift();

          //delete piece.distance ....
        }
      }
    }
  }
}


// overhault for take into account manual movement
void automaticControl(){
  switch (currentState) {
    case IDLE:
      moveBelt();
      magic();
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

            pieces_labels.add(category);
            pieces_distance.add(box_edge_distance+0);//some pixel magic
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
  int steps = (200.0 / 360) * abs(angle);
  if (angle < 0){
    for(int i = 0;i<steps;i++){
      moveDis(LOW);
      delay(speed_delay);
    }
  }else{
      for(int i = 0;i<steps;i++){
        moveDis(HIGH);
        delay(speed_delay);
      }
  }
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
