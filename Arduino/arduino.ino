#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Bildr-MLX90614.h>

#define trigMasuk 30 
#define echoMasuk 31
#define trigAir 36 
#define echoAir 37
#define trigSabun 46 
#define echoSabun 47
#define MAX_DISTANCE 200
#define sabun 22 
#define air 23
#define buzzer 4
#define ledMasuk 53
#define ledSuhu 51

MLX90614 sensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(2, 3);

NewPing ultraMasuk(trigMasuk, echoMasuk, MAX_DISTANCE);
NewPing ultraAir(trigAir, echoAir, MAX_DISTANCE);
NewPing ultraSabun(trigSabun, echoSabun, MAX_DISTANCE);

int uMasuk = 0;
int uAir = 0;
int uSabun = 0;

bool keadaanAir = false;
 
void setup() {
  pinMode(trigMasuk, OUTPUT); 
  pinMode(echoMasuk, INPUT);
  pinMode(trigAir, OUTPUT); 
  pinMode(echoAir, INPUT);
  pinMode(trigSabun, OUTPUT); 
  pinMode(echoSabun, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(air, OUTPUT);
  pinMode(sabun, OUTPUT);
  pinMode(ledMasuk, OUTPUT);
  pinMode(ledSuhu, OUTPUT);
  
  Serial.begin (9600);
  mySerial.begin (9600);
  mp3_set_serial (mySerial);
  mp3_set_volume (30);
  delay(10);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Selamat Datang");
  lcd.setCursor(0, 1);
  lcd.print("Rahmat Fadhila");
  delay(2000);

}

void loop() {
  uMasuk = bacaUltraMasuk();
  Serial.println(uMasuk);
  if(uMasuk >= 2 && uMasuk <= 25){
    digitalWrite(ledMasuk, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Patuhi Protokol Kesehatan");
    lcd.setCursor(0,1);
    lcd.print("Cuci Tangan");
    toneBuzzer();
    mp3_play (01);
    Serial.println("Masuk");
    delay(16000);
    digitalWrite(ledMasuk, LOW);
    mp3_play (05);
    prosesAir();
  }
}

int bacaUltraMasuk(){
  delay(250);
  int dataUltraMasuk = ultraMasuk.ping_cm();
  return dataUltraMasuk;  
}

int bacaUltraAir(){
  delay(250);
  int dataUltraAir = ultraAir.ping_cm();
  return dataUltraAir;
}

int bacaUltraSabun(){
  delay(250);
  int dataUltraSabun = ultraSabun.ping_cm();
  return dataUltraSabun;
}

void toneBuzzer(){
  for(int d = 0; d <= 3; d++){
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
}

void prosesAir(){
  while(1){
    uAir = bacaUltraAir();
    Serial.println(uAir);
    if(uAir >= 2 && uAir <= 10){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Proses Cuci Tangan");
      lcd.setCursor(0,1);
      lcd.print("Dengan Air");
      digitalWrite(air, HIGH);
      delay(6000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Langkah Selanjutnya");
      lcd.setCursor(0,1);
      lcd.print("Sabun Cuci");
      digitalWrite(air, LOW);
      mp3_play (06);
      prosesSabun();
      return;
    }
  }
}

void prosesSabun(){
  while(1){
    uSabun = bacaUltraSabun();
    Serial.println(uSabun);
    if(uSabun >= 2 && uSabun <= 10){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Pengambilan");
      lcd.setCursor(0,1);
      lcd.print("Sabun Cuci");
      digitalWrite(sabun, HIGH);
      delay(4000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Langkah Selanjutnya");
      lcd.setCursor(0,1);
      lcd.print("Air Cuci Tangan");
      digitalWrite(sabun, LOW);
      mp3_play (07);
      prosesAir2();
      return;
    }
  }
}

void prosesAir2(){
  while(1){
    uAir = bacaUltraAir();
    Serial.println(uAir);
    if(uAir >= 2 && uAir <= 10){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Proses Cuci Tangan");
      lcd.setCursor(0,1);
      lcd.print("Dengan Air");
      digitalWrite(air, HIGH);
      delay(6000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Langkah Selanjutnya");
      lcd.setCursor(0,1);
      lcd.print("Cek Suhu");
      digitalWrite(air, LOW);
      delay(1000);
      mp3_play (8);
      Serial.println("proses Cek Suhu");
      bacaSuhu();
      return;
    }
  }
}

void bacaSuhu(){
  while(1){
    float celsius = sensor.getC();
    Serial.print("Suhu : ");
    Serial.print(celsius);
    Serial.println(" C");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Suhu");
    lcd.setCursor(0,1);
    lcd.print(celsius);
    delay(500);
    if(celsius >= 35.00 && celsius <= 37.00){
      digitalWrite(ledSuhu, HIGH);
      mp3_play (03);
      Serial.println("d");
      delay(9000);
      digitalWrite(ledSuhu, LOW);
      return;
    }else if(celsius >= 38.00){
      digitalWrite(ledSuhu, HIGH);
      mp3_play (04);
      Serial.println("p");
      delay(8000);
      digitalWrite(ledSuhu, LOW);
      return;
    }  
  }
}
