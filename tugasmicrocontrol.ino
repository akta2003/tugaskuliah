
const int led4 = 7;
const int led3 = 6;
const int led2 = 5;
const int led1 = 4;
const int button1 = 14;
const int button2 = 15;

void setup() {
  // put your setup code here, to run once:
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);

    //pinMode(4, OUTPUT);
    //pinMode(5, OUTPUT);
    //pinMode(6, OUTPUT);
    //pinMode(7, OUTPUT);

    pinMode(button1,INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);

}


void loop() {
  // put your main code here, to run repeatedly:

    int state1 = digitalRead(button1);
    int state2 = digitalRead(button2);
    
   // if(state1 == 0){

   //    mati();
    //  }

    if (state1 == 0){

      normal();
      }
 
    if (state2 == 0){
      
      mati(); 
      }

    //delay(1000);
}

void normal(){

      digitalWrite(led1, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led4, HIGH);
    delay(100);
  }

void mati(){
    digitalWrite(led1, HIGH);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led4, LOW);
    delay(100);
  }
