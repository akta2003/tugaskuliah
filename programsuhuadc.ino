
  #include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int nilaiADC;
float tegangan;
float teganganAsli;
float suhuCelsius;
const float suhuu = 1000 /10;
const float res = 40000.0/10000.0;
const float pengali = 5.0/1023.0;
byte derajat[8] = {
  0b00110,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
  };

void setup() {
  // put your setup code here, to run once:
  lcd.createChar(1, derajat);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
      //adc();
      suhu();
}

void adc(){

       nilaiADC = analogRead(A3);
       tegangan = nilaiADC * pengali;
       teganganAsli  = tegangan * res;

       //lcd.setCursor(0,0);
       //lcd.print("H43221089");
       //lcd.setCursor(0,1);
       //lcd.print("nADC ,");lcd.print(" vADC");
       //delay(3000);
       //lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("H43221089");
       lcd.setCursor(0,1);
       lcd.print(nilaiADC);
       lcd.print(" , ");
       lcd.print(tegangan,1);
       lcd.print(" V");
       delay(1000);
       lcd.clear();
  }

  void suhu(){

       nilaiADC = analogRead(A3);
       tegangan = nilaiADC * pengali;
       suhuCelsius  = tegangan * suhuu;

       lcd.setCursor(0,0);
       lcd.print("H43221089");
       lcd.setCursor(0,1);
       lcd.print("Suhu = ");
       lcd.print(suhuCelsius,1);
       lcd.write(1);
       lcd.print("C ");
       delay(1000);
       lcd.clear();
    }
