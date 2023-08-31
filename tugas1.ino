/*
 * This is the sample program to test in proteus. 
 * if you want any help regarding development of product/ project or in any software 
 * you can contact us at info@smedehradun.com and schematicslab@gmail.com 
 * 
 */

int led =13;
int button = 2; 
int statusb = LOW;

bool LedHidup = false; 

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
}

void loop() {

if (digitalRead(button) == LOW){
  delay(50); //Debouncing
     if (digitalRead(button) == LOW){
      LedHidup = !LedHidup;
      digitalWrite(led, LedHidup ? HIGH : LOW);
      while (digitalRead(button) == LOW); }
    }
}
