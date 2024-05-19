#include <LiquidCrystal_I2C.h>
#include <Wire.h>

const int buttonBad = 13; 
const int buttonNotBad = 12; 
const int buttonGood = 2; 
const int ledBad = 8;
const int ledNotBad = 7;
const int ledGood = 4;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int buttonStateBad = 0; 
int lastButtonStateBad = 0;

int buttonStateNotBad = 0; 
int lastButtonStateNotBad = 0;

int buttonStateGood = 0; 
int lastButtonStateGood = 0;

int stateQuestion = 0;
unsigned long antiAfkPreviousTime = 0;
int antiAfk = 0;
String responseQuestion1 = "";
String responseQuestion2 = "";

void eval(const char* choice) {
  Serial.println(stateQuestion);
  if (strcmp(choice, "good") == 0) {
    if (stateQuestion == 0) {
      responseQuestion1 = "Bien";
    }else {
      responseQuestion2 = "Bien";
    }
    digitalWrite(ledGood, HIGH);
    delay(200);
    digitalWrite(ledGood, LOW);
  } else if (strcmp(choice, "notBad") == 0) {
    if (stateQuestion == 0) {
      responseQuestion1 =  "Moyen";
    }else {
      responseQuestion2 = "Moyen";
    }
    digitalWrite(ledNotBad, HIGH);
    delay(200);
    digitalWrite(ledNotBad, LOW);
  } else if (strcmp(choice, "bad") == 0) {
    if (stateQuestion == 0) {
      responseQuestion1 =  "Mauvais";
    }else {
      responseQuestion2 = "Mauvais";
    }
    digitalWrite(ledBad, HIGH);
    delay(200);
    digitalWrite(ledBad, LOW);
  }

  Serial.println(choice);

  stateQuestion++;
  showQuestion();
}

void showQuestion() {
    lcd.init();
    lcd.cursor_on();
    lcd.blink_on();
    lcd.backlight();
    lcd.setCursor(0,0); 
    if (stateQuestion == 0) {
      lcd.print("> Avis prof ?"); 
    }else if (responseQuestion1.length() > 0){
      lcd.print("Avis prof: ");
      lcd.print(responseQuestion1);
    }else {
      lcd.print("Avis prof ?"); 
    }
    lcd.setCursor(0,1);
    if (stateQuestion == 1) {
      lcd.print("> Avis Cours ?");
    }else if (responseQuestion2.length() > 0){
      lcd.print("Avis Cours: ");
      lcd.print(responseQuestion2);
    }else {
      lcd.print("Avis Cours ?");
    }
    delay(100);
    lcd.cursor_off();
    lcd.blink_off();
}


void setup() {
  Serial.begin(9600);
  pinMode(ledBad, OUTPUT);     
  pinMode(ledNotBad, OUTPUT);     
  pinMode(ledGood, OUTPUT);     

  pinMode(buttonBad, INPUT_PULLUP);   
  pinMode(buttonNotBad, INPUT_PULLUP);   
  pinMode(buttonGood, INPUT_PULLUP);   

  lastButtonStateBad = digitalRead(buttonBad);
  lastButtonStateNotBad = digitalRead(buttonNotBad);
  lastButtonStateGood = digitalRead(buttonGood);

  showQuestion();

}

void loop() {
  unsigned long currentTime = millis();
  int buttonStateBad = digitalRead(buttonBad); 
  int buttonStateNotBad = digitalRead(buttonNotBad); 
  int buttonStateGood = digitalRead(buttonGood); 

  if (buttonStateBad == HIGH && lastButtonStateBad == LOW) {eval("bad");}
  if (buttonStateNotBad == HIGH && lastButtonStateNotBad == LOW) {eval("notBad");}
  if (buttonStateGood == HIGH && lastButtonStateGood == LOW) {eval("good");}

  if (stateQuestion == 2) {
    lcd.init();
    lcd.cursor_on();
    lcd.blink_on();
    lcd.backlight();
    lcd.setCursor(5,0); 
    lcd.print("Merci !"); 
    lcd.setCursor(1,1);
    lcd.print("Bonne journee!");
    delay(100);
    lcd.cursor_off();
    lcd.blink_off();
    delay(1000);
    stateQuestion = 0;
    responseQuestion1 = "";
    responseQuestion2 = "";
   showQuestion();
  }

  if (stateQuestion == 1 && currentTime - antiAfkPreviousTime >= 1000) {
    antiAfk++;
    antiAfkPreviousTime = currentTime;
    if (antiAfk > 5) {
      stateQuestion = 0;
      antiAfk = 0;
      responseQuestion1 = "";
      responseQuestion2 = "";
      showQuestion();
    }
  }


  lastButtonStateBad = buttonStateBad;
  lastButtonStateNotBad = buttonStateNotBad;
  lastButtonStateGood = buttonStateGood;
}
