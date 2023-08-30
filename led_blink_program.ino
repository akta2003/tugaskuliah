/*
 * This is the sample program to test in proteus. 
 * if you want any help regarding development of product/ project or in any software 
 * you can contact us at info@smedehradun.com and schematicslab@gmail.com 
 * 
 */

const int led =13;
const int button = 2; 
int sttb = LOW;
bool ledNyala = false; 

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
}

void loop() {
if (digitalRead(button) == LOW){
delay(50);
if (digitalRead(button) == LOW){
  ledNyala = !ledNyala;
      digitalWrite(led, ledNyala ? HIGH : LOW);
      while (digitalRead(button) == LOW);
  }
}
}
