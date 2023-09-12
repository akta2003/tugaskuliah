
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);


void setup() {
  
  lcd.begin(16, 2);
  lcd.clear();

  displaylcd();
  delay(1000);
  cursor1();
  delay(1000);
  blink1();
  delay(1000);
  geserkanan();
  lcd.clear();
  delay(1000);
  geserkiri();
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

     
  
  
    tugas2();
}

void displaylcd(){

  lcd.setCursor(0,0);
  lcd.print("Teks Di LCD");
  lcd.setCursor(0,1);
  lcd.print("Nyala / Mati");
  
  lcd.noDisplay();
  delay(1000);
  lcd.display();
  delay(3000);
  lcd.clear();
  
  }


void cursor1(){

  lcd.setCursor(0,0);
  lcd.print("Saat Ini,");
  lcd.setCursor(0,1);
  lcd.print("Kursor Nyala");
  lcd.cursor();
  //lcd.display();
  delay(3000);
  //lcd.noDisplay();
  //delay(1000);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Saat Ini,");
  lcd.setCursor(0,1);
  lcd.print("Kursor Mati");
  lcd.noCursor();
  //lcd.display();
  //delay(1000);
  //lcd.noDisplay();
  delay(3000);
  lcd.clear();
  }

void blink1(){

  lcd.setCursor(0,0);
  lcd.print("Saat Ini,");
  lcd.setCursor(0,1);
  lcd.print("Blink Aktif");
  
  lcd.blink();
  delay(3000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Saat Ini,");
  lcd.setCursor(0,1);
  lcd.print("Blink Mati");

  lcd.noBlink();
  delay(3000);
  lcd.clear();
  
  }

void geserkanan(){

  lcd.setCursor(0,0);
  lcd.print("WAKTUNYA SHOLAT");
  lcd.setCursor(2,1);
  lcd.print("Geser Kanan");
  delay(1000);

  lcd.scrollDisplayRight();
  delay(1000);
  }

  void geserkiri(){

  lcd.setCursor(0,0);
  lcd.print("WAKTUNYA SHOLAT");
  lcd.setCursor(0,1);
  lcd.print("Geser Kiri");
  delay(1000);

  lcd.scrollDisplayLeft();
  delay(1000);
  }

void tugas2(){

  lcd.setCursor(0,0);
  lcd.print("AKTA MUHAMAD I. R.");
  lcd.setCursor(0,1);
  lcd.print("H43221089");
  lcd.display();
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T R M");
  lcd.setCursor(0,1);
  lcd.print("TEKNIK");
  lcd.display();
  delay(2000);
  lcd.clear();
  
  }
