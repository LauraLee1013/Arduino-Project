#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10                  //RC522 NSS connect Arduino pin 10
#define RST_PIN 9                  //RC522 RST connect Arduino pin 9
#define LED_G 5                    //green LED positive connect Arduino pin 5
#define LED_R 4                    //red LED positive connect Arduino pin 4
#define BUZZER 6                   //buzzer positive connect Arduino pin 2
#define BUTTON 7
#define SIZE 11
int k = 1;
int state = 0;
String id[50];

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
Servo myServo;                     //define servo name

void setup() {
  Serial.begin(9600);  // Initiate a serial communication
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  myServo.attach(3);   // servo moter yellow connect Arduino pin 3
  myServo.write(0);    //servo start position
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader");
  //Serial.println("Put your card to the reader...o");
  //Serial.println();
  lcd.init();       //initialize LCD
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("Please show");
  lcd.setCursor(0, 1);
  lcd.print("your ID card.");
  id[0] = " D5 7F 02 46";
  id[1] = " B5 EF 25 46";

}

void loop() {
  if(digitalRead(BUTTON)==1){
    state++;
    if(state == 3){
      state = 0;
    }
    Serial.print("Now state is: ");
    Serial.println(state);
    //Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Now state is:");
    lcd.print(state);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please show");
    lcd.setCursor(0, 1);
    lcd.print("your ID card.");

  }
  
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();


  //Serial.println(digitalRead(BUTTON));
  if(state == 0){
    int counter = 0;
    for(int m =0;m<k+1;m++){
      if (content.substring(0) == id[m]){
        Serial.println("Access approval");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access approved.");
        delay(500);
        digitalWrite(LED_G, HIGH);
        tone(BUZZER, 500);
        delay(300);
        noTone(BUZZER);
        myServo.write(90);
        delay(3000);
        myServo.write(0);
        digitalWrite(LED_G, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please show");
        lcd.setCursor(0, 1);
        lcd.print("your ID card.");
        break;
      }
    counter++;

    if(counter==k+1){
      Serial.println("Access denied");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Have no access.");
      digitalWrite(LED_R, HIGH);
      tone(BUZZER, 300);
      delay(2000);
      digitalWrite(LED_R, LOW);
      noTone(BUZZER);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please show");
      lcd.setCursor(0, 1);
      lcd.print("your ID card.");
      }
    }
  }

  if(state == 1){
    k++;
    for(int j = 0; j < content.length(); j++){
      id[k].concat(content[j]);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New ID of user.");
    Serial.print("save as number ");
    Serial.println(k+1);
    //Serial.println();
    // Serial.print("id is :");
    // Serial.println(id[k]);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(200);
    noTone(BUZZER);
    delay(2000);
    digitalWrite(LED_G, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please show");
    lcd.setCursor(0, 1);
    lcd.print("your ID card.");
    
  }

  if(state == 2){
    boolean found = true;
    // Serial.println(k);
    for(int p=0;p<k+1;p++){
      if(content.substring(0)==id[p]){
      id[p]="";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Delete user");
      Serial.println("Delete user");
      //Serial.println();
      found = false;
      digitalWrite(LED_G, HIGH);
      tone(BUZZER, 500);
      delay(200);
      noTone(BUZZER);
      delay(2000);
      digitalWrite(LED_G, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please show");
      lcd.setCursor(0, 1);
      lcd.print("your ID card.");
      break;
      }
    }
    if (found){
      Serial.println("ID not found.");
      //Serial.println();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID not found.");
      digitalWrite(LED_R, HIGH);
      tone(BUZZER, 300);
      delay(200);
      noTone(BUZZER);
      delay(2000);
      digitalWrite(LED_R, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please show");
      lcd.setCursor(0, 1);
      lcd.print("your ID card.");
      }
  }
}
