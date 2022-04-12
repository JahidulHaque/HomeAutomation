int room11=7;
int room12=8;
int room13=9;

int room21=A2;
int room22=A3;


int room31=3;
int room32=5;

String data;

bool ledOn=false;
bool ledOff=false;
bool ledAuto=true;
bool fireClear=false;

int in1 = 11;
int in2 = 10;

#include<Servo.h>
Servo Myservo;

#include "DHT.h"               //Including DHT Library

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//DHT Sensor Codes
 
#define DHTPIN 2                //Defining DHTPIN
#define DHTTYPE DHT11           //Defining DHTVersion
DHT dht(DHTPIN, DHTTYPE);       //DHT Initialisation

//Flame and Buzzer

int flameSensor = A0; // select the input pin for the LDR
int flameValue = 0; // variable to store the value coming from the sensor
int buzzer = 4; // Output pin for Buzzer

//LDR Sensor

const int ldrPin = A1;

//Ultrasonic
#define echoPin 12
#define trigPin 13
long duration;
int distance;

void setup()
{ 
    // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("NYA's Home");
  lcd.setCursor(0,1);
  lcd.print("Init. Service");
  delay(3000);
  
  
  dht.begin();
  

  pinMode(buzzer, OUTPUT);
  pinMode(room11, OUTPUT);
  pinMode(room12, OUTPUT);
  pinMode(room13, OUTPUT);
  pinMode(room21, OUTPUT);
  pinMode(room22, OUTPUT);
  pinMode(room31, OUTPUT);
  pinMode(room32, OUTPUT);
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  Myservo.attach(6);
  Myservo.write(180);  
  
  
  
  pinMode(flameSensor, INPUT);
  pinMode(ldrPin, INPUT);

  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  digitalWrite(room11, HIGH);
  digitalWrite(room12, HIGH);
  digitalWrite(room13, HIGH);
  
  analogWrite(room21, 0);
  analogWrite(room22, 0);
  
  digitalWrite(room31, HIGH);
  digitalWrite(room32, HIGH);
  
  
  
  
  
  
  
  Serial.begin(9600);
}

void loop()
{
  
  
  if(Serial.available()){
    data=Serial.readString();
    Serial.println(data);
    if(data.equalsIgnoreCase("on")){
      allLEDOn();
      ledOn=true;
      ledOff=false;
    }
    else if(data.equalsIgnoreCase("off")){
      allLEDOff();
      ledOn=false;
      ledOff=true;
    }
    else if(data.equalsIgnoreCase("auto")){
      ledAuto=true;
    }
    else if(data.equalsIgnoreCase("static")){
      ledAuto=false;
    }
    else if(data.equalsIgnoreCase("50")){
      
      analogWrite(in1, 50);
      analogWrite(in2, 0);
    }
    else if(data.equalsIgnoreCase("150")){
      
      analogWrite(in1, 150);
      analogWrite(in2, 0);
    }else if(data.equalsIgnoreCase("200")){
      
      analogWrite(in1, 200);
      analogWrite(in2, 0);
    }
    else if(data.equalsIgnoreCase("255")){
      
      analogWrite(in1, 255);
      analogWrite(in2, 0);
    }
    
    
    else if(data.equalsIgnoreCase("0")){
      
      analogWrite(in1, 0);
      analogWrite(in2, 0);
    }
    else if(data.equalsIgnoreCase("g")){
      Myservo.write(90);
      delay(5000);
    }
    
    
  }
  int t = (int)dht.readTemperature();   //Reading temperature from the sensor and preserving it to t variable
  int h = (int)dht.readHumidity();
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.print(" | Humidity: ");
//  Serial.println(h);
  lcd.setCursor(3,0);
  lcd.print("NYA's Home");
  lcd.setCursor(0,1);
  lcd.print("T: ");
  lcd.setCursor(3,1);
  lcd.print(t);
  lcd.setCursor(5,1);
  lcd.print(" | H: ");
  lcd.setCursor(11,1);
  lcd.print(h);
  

  flameValue = analogRead(flameSensor);

//  Serial.println(flameValue);

  if (flameValue < 100){
    tone(buzzer, 2000); // Send 1KHz sound signal...
    lcd.clear();
    lcd.print("Fire Alert");
    allLEDOn();
    garageOn();
    delay(50);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    allLEDOff();
    garageOff();
    delay(50);        // ...for 1sec
    fireClear=false;
  }else{
    if(!fireClear){
      lcd.clear();
      fireClear=true;
    }
    digitalWrite(buzzer,LOW);
  }




  //LDR system
  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus <= 200 && !ledOn) {
//    Serial.print("Bright...");
//    Serial.println(ldrStatus);
    allLEDOff();
    
  } 
  if(ldrStatus>=200 && ledAuto) {
//    Serial.print("Dark...");
//    Serial.println(ldrStatus);
    allLEDOn();
  }

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  duration=pulseIn(echoPin,HIGH);
  distance=(duration*0.034/2);
//  Serial.print("Distance : ");
//  Serial.print(distance);
//  Serial.println(" cm ");

  if(distance<5){
    analogWrite(room21, 255);
    analogWrite(room22, 255);
    Myservo.write(90);
    delay(5000);
  }else{
    analogWrite(room21, 0);
    analogWrite(room22, 0);
    Myservo.write(180);
  }
  
}

void allLEDOff(){
  digitalWrite(room11, LOW);
  digitalWrite(room12, LOW);
  digitalWrite(room13, LOW);

  digitalWrite(room31, LOW);
  digitalWrite(room32, LOW);

  
}

void allLEDOn(){
  digitalWrite(room11, HIGH);
  digitalWrite(room12, HIGH);
  digitalWrite(room13, HIGH);

  digitalWrite(room31, HIGH);
  digitalWrite(room32, HIGH);


}

void garageOn(){
  analogWrite(room21, 255);
  analogWrite(room22, 255);
}

void garageOff(){
  analogWrite(room21, 0);
  analogWrite(room22, 0);
}
