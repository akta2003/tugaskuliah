
const int led4 = 7;
const int led3 = 6;
const int led2 = 5;
const int led1 = 4;
const int button1 = 14;
const int button2 = 15;
bool btncheck = false;

int sttb1 = LOW;
int sttb2 = LOW;

int statusled1 = HIGH;
int statusled2 = HIGH;
int statusled3 = LOW;
int statusled4 = LOW;

void setup() {
  // put your setup code here, to run once:
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(button1, INPUT_PULLUP); 
    pinMode(button2, INPUT_PULLUP); 
}


void loop() {
  // put your main code here, to run repeatedly:

      //tugas1();
      //tugas2();
      tugas3();
}

void tugas1(){

    digitalWrite(led1, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led4, HIGH);
    delay(100);

    digitalWrite(led1, HIGH);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led4, LOW);
    delay(100);
  }

  void tugas2 (){

    //baca status tombole
    sttb1 = digitalRead(button1); 
    sttb2 = digitalRead(button2);
  
    // cek status tombol jika tb1 ditkn = low nyalakan led , jika tidak berarti tmbl 2 ditekan maka matikan led
    if (sttb1 == LOW) {
    statusled1 = LOW;
    statusled2 = LOW;
    statusled3 = HIGH;
    statusled4 = HIGH;
    } 
    else if (sttb2 == LOW) {
    statusled1 = HIGH;
    statusled2 = HIGH;
    statusled3 = LOW;
    statusled4 = LOW;
    }
    // eksekusi ,jika diatas berlogika hihg atau low
    digitalWrite(led1, statusled1); 
    digitalWrite(led2, statusled2); 
    digitalWrite(led3, statusled3); 
    digitalWrite(led4, statusled4); 
  
    }

  void tugas3 (){

    if (digitalRead(button1)==0)
    {
    delay(30);
    if (digitalRead(button1)==0)
    {
     btncheck =! btncheck;
    digitalWrite(led1, btncheck ? LOW : HIGH);
    digitalWrite(led2, btncheck ? LOW : HIGH);
    digitalWrite(led3, btncheck ? HIGH : LOW);
    digitalWrite(led4, btncheck ? HIGH : LOW);
    while(digitalRead(button1)==0){}
}
}
    }
