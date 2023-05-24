// Stepper Motor X
  const int stepPin = 2; //X.STEP
  const int dirPin = 5; // X.DIR
 // joystick
 int vrx = A0; 
int vry = A1; 
int vrx_data = 0; 
int vry_data = 0; 

int x = 0; 
int SMSpeed = 0; // Stepper Motor Speed 
 void setup() {
 // Sets the two pins as Outputs
 Serial.begin(9600); 
 pinMode(stepPin,OUTPUT); 
 pinMode(dirPin,OUTPUT);
 pinMode(vrx , INPUT); 
 pinMode(vry, INPUT); 
 }

 
 void loop() 
 {
  
joystick(); 

 }

 void joystick()
{
vrx_data = analogRead(vrx);
Serial.print("Vrx:"); 
Serial.println(vrx_data); 


// to stop the stepper motor
if ( (vrx_data > 490)  &&   (vrx_data < 510)   )
{
 digitalWrite(stepPin,LOW); 
 delayMicroseconds(SMSpeed); 
;
  
}


if ( vrx_data > 700  )
{

digitalWrite(dirPin,HIGH);
x = x + 1; 
 digitalWrite(stepPin,HIGH); 
 delayMicroseconds(100*SMSpeed); 
 digitalWrite(stepPin,LOW); 
 delayMicroseconds(SMSpeed); 
  
}




if ( vrx_data < 300   )
{
digitalWrite(dirPin,LOW);
x = x - 1; 

 digitalWrite(stepPin,HIGH); 
 delayMicroseconds(100*SMSpeed); 
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(SMSpeed);  
}

}
