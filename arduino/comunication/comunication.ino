void setup() {
  Serial.begin(115200);
}
void loop() {
//        Serial.println("Hello from Arduino");
  if(Serial.available() > 0){
    String message = Serial.readStringUntil('\n');
    Serial.println(message);
    
  }
        delay(1000);
}
