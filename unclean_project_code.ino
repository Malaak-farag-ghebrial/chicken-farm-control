#define PUMP_PIN 3 
#define SERVO_PIN 6 
#define DHT_PIN 5
#define DHT_LED_PIN 10 
#define WATER_PIN 4 
#define WATER_ANALOG_PIN A5
#define DHTTYPE DHT11

#include <Servo.h>
#include <DHT.h>

double waterLevelValue = 0;
double humidity = 0;
double temperature = 0;
double openFoodValveDelayInHours = 6; // hours interval to open valve of food again
double foodValveTimeInSecond = 6; // second for keep food valve open then close it

Servo servo;  
DHT dht(DHT_PIN, DHTTYPE);


void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);  
    pinMode(PUMP_PIN,OUTPUT);
    pinMode(WATER_PIN,OUTPUT);
    digitalWrite(WATER_PIN, LOW);
    dht.begin();
    pinMode(DHT_PIN,INPUT);
}

void loop() {
  // pump control start
    digitalWrite(WATER_PIN,HIGH);
    delay(10); 
    waterLevelValue = analogRead(WATER_ANALOG_PIN);
    digitalWrite(WATER_PIN, LOW);
    Serial.print("Water Level value: ");
    Serial.println(waterLevelValue);
    if(waterLevelValue < 400){ // 400 for the level you wanna detect
      digitalWrite(PUMP_PIN, HIGH);
      Serial.println("The Water Pump is ON!");
    }else{
      digitalWrite(PUMP_PIN, LOW);
      Serial.println("The Water Pump is OFF!");
    }
    // pump control end
    // humidity control start
    delay(500);
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
     if(temperature >= 20 && temperature <= 35 && humidity >= 40 && humidity <= 70){
      digitalWrite(DHT_LED_PIN,LOW);
    }else{
      digitalWrite(DHT_LED_PIN, HIGH);
      delay(500);
      digitalWrite(DHT_LED_PIN,LOW);
    }
    // humidity control end 
    // servo contol start
    if(timeDelay(openFoodValveDelayInHours)){
       Serial.println("The Servo Open valve!");
      for (int pos = 90; pos >= 0; pos -= 1) {  
        servo.write(pos);  
        delay(10);      
      }
       delay(1000*foodValveTimeInSecond);
      for (int pos = 0; pos <= 90; pos += 1) {  
        servo.write(pos);                        
        delay(10);                          
     }
    }
   // servo contol end

}
  // function for calculate time in hours instead of delay cause delay pause all program for X Hours
  bool timeDelay(int hours){
    unsigned long previousMillis = 0; 
    long int time = millis()/(60L * 60L * 1000L) + 1;
    Serial.println(millis()/(60L * 60L * 1000L) + 1);
    if(time % hours == 0 && (time - previousMillis) >= hours){
        previousMillis = time;
         return true;
    }else{
         return false;
    }
  }
