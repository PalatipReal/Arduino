
const int button = 33;
const int led = 2;

int bounceTime = 50;
int holdTime = 250;
int doubleTime = 500;

int lastReading = LOW;
int hold = 0;
int single = 0;
int LEDstate = 0;

long onTime = 0;
long lastSwitchTime = 0;


void setup() {
 pinMode(button, INPUT);
 pinMode(led, OUTPUT);
 digitalWrite(led, LOW);
 Serial.begin(9600);
}

void loop() {

 int reading = digitalRead(button);

//first pressed
 if (reading == HIGH && lastReading == LOW) {
   onTime = millis();
 }

//held
 if (reading == HIGH && lastReading == HIGH) {
   if ((millis() - onTime) > holdTime) {
     invertLED();  
     hold = 1;
   }
 }

//released
 if (reading == LOW && lastReading == HIGH) {
   if (((millis() - onTime) > bounceTime) && hold != 1) {
     onRelease();
   }
   if (hold == 1) {
     Serial.println("held");
     digitalWrite(led, LEDstate);
     hold = 0;
   }   
 }
 lastReading = reading;

 if (single == 1 && (millis() - lastSwitchTime) > doubleTime) {
   Serial.println("single press");
   single = 0;
 }

}


void onRelease() {

 if ((millis() - lastSwitchTime) >= doubleTime) {
   single = 1;
   lastSwitchTime = millis();
   return;
 }  

 if ((millis() - lastSwitchTime) < doubleTime) {
   toggleLED();
   Serial.println("double press");
   single = 0;
   lastSwitchTime = millis();
 }  

}

void toggleLED() {
 if (LEDstate == 0) {
   LEDstate = 1;
 } else {
   LEDstate = 0;
 }
 digitalWrite(led, LEDstate);  
} 
 
void invertLED() {
 if (LEDstate == 0) {
 digitalWrite(led, 1);
 } else {
 digitalWrite(led, 0);
 }
}
