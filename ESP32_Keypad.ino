#include <Keypad.h>                 //Library keypad
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>   //Library LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);   //Alamat I2C
const int servoPin = 13;
Servo servo1;
int buka= 180;
int tutup= 0;
int buzz= 27;
const byte ROWS = 4;    //Jumlah baris keypad
const byte COLS = 4;    //Jumlah kolom keypad
#define ledHijau 25
#define ledMerah 26

char Keys[ROWS][COLS] = {   //Membuat array keypad
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 14, 12, 19, 18 }; //pin baris
byte colPins[COLS] = { 5, 4, 2, 15 };   //pin kolom

Keypad customKeypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS);    //Masukkan info keypad pada library

byte Locked[] = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
};

byte Unlocked[] = {
  0b01110,
  0b10000,
  0b10000,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
};

byte panah[] = {
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b10001,
  0b01010,
  0b00100,
};

char customKey;         //Variabel penampung input keypad
int number = 0;         //Variabel penampung nilai angka
int password = 1111;    //Password
int batasPass = 0;      // variabel penampung nilai pass"**" 
int limit = 4;          // variabel batas nilai pass
int cursorPass = 6;     // variabel set cursor password lcd

void setup() {
  servo1.attach(servoPin);
  pinMode (ledHijau, OUTPUT);
  pinMode (ledMerah, OUTPUT);
  pinMode (buzz, OUTPUT);
  lcd.init ();              //Mulai LCD
  lcd.setBacklight(HIGH);   //Nyalakan backlight
  lcd.setCursor(0, 0);
  lcd.print(" START ENGINE ");
  tone (buzz, 3000);
  delay(100);
  noTone(buzz);
  lcd.createChar(0, Locked);
  lcd.createChar(1, Unlocked);
  lcd.createChar(2, panah);
  lcd.clear();
}

void loop() {
  digitalWrite (ledHijau, LOW);
  digitalWrite (ledMerah, LOW);  
  lcd.setCursor(0,0);
  lcd.write(byte(2));
  lcd.print(" Please Enter ");      //Tampilan pada layar LCD
  lcd.write(byte(2));
  customKey = customKeypad.getKey();    //Baca input keypad

  //------------Prosedur jika input berupa angka------------//
  switch(customKey){
    case '0' ... '9':
    batasPass++; 
    if (batasPass <= limit){   // jika nilai batasPass tidak lebih dari nilai limit
      lcd.setCursor(0,1);
      number = number * 10 + (customKey - '0');
      lcd.setCursor(cursorPass, 6); 
      lcd.print("*");
      cursorPass+=1;  // jarak spasi pada nilai cursorPass password
    }
    break;

    //------------Jika input '#' maka cek password------------//
    case '#':
      if(number == password){           //Jika password benar, maka
        lcd.setCursor(0, 1);
        lcd.write(byte(1));
        lcd.print(" Silakan Masuk ");  //Tampilan LCD
        digitalWrite (ledHijau, HIGH);
        tone(buzz, 3000);
        delay(200);
        noTone(buzz);
        digitalWrite(ledHijau, LOW);
        delay(300);
        noTone(buzz);
        digitalWrite(ledHijau, HIGH);
        tone(buzz, 3000);
        delay(200);
        noTone(buzz);
        digitalWrite(ledHijau, LOW);
        digitalWrite (ledMerah, LOW);
        delay(200);
        servo1.write(buka);
        number = 0;
        batasPass= 0;
        cursorPass= 5;
        lcd.clear(); 
      }
      else{                             //Jika salah, maka
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print(" Salah Password");  //Tampilan LCD
        digitalWrite (ledHijau, LOW);
        digitalWrite (ledMerah, HIGH);
        servo1.write(tutup);
        tone(buzz, 700);
        delay(2000);
        noTone(buzz);
        number = 0;
        batasPass= 0;
        cursorPass=5;
        lcd.clear();
      }
    break;
    //------------Jika input 'D' maka hapus tampilan dan servo lock pintu------------//
    case 'D':
      number = 0;
      batasPass = 0;
      cursorPass = 5;
      digitalWrite (ledMerah, HIGH);
        tone(buzz, 3000);
        delay(200);
        noTone(buzz);
        digitalWrite(ledMerah, LOW);
        delay(300);
        noTone(buzz);
        digitalWrite(ledMerah, HIGH);
        tone(buzz, 3000);
        delay(200);
        noTone(buzz);
        digitalWrite(ledMerah, LOW);
        delay(100);
        digitalWrite (ledHijau, LOW);
      servo1.write(tutup);
      lcd.clear();
    break;
  }
}// Program Kunci Pintu dengan password input Keypad 4x4 
