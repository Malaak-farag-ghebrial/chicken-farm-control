#define PUMP_PIN 3 
#define SERVO_PIN 6 
#define DHT_PIN 5
#define DHT_LED_PIN 10 
#define WATER_PIN 4 
#define WATER_ANALOG_PIN A5

double servoStartAngle = 50;
double openFoodValveDelayInHours = 1;
double foodValveTimeInSecond = 10;
double pumpingTimeInSecond = 5;
long int hourLengthInSecond = 60L * 60L;
double humidity = 0;
double temperature = 0;
double waterLevelValue = 0;
unsigned long previousMillis = 0; 

#define O OUTPUT
#define I INPUT
#define H HIGH
#define L LOW

#define DHTTYPE DHT11

#include <Servo.h>
#include <DHT.h>

DHT dht(DHT_PIN, DHTTYPE);

class ServoMotor {
  public:
  Servo servo;  
  void servoSetUp(){
      servo.attach(SERVO_PIN);  
      servo.write(servoStartAngle);   
  }
  void openValve(){
    Serial.println("The Servo Open valve!");
    for (int pos = 0; pos <= 150; pos += 1) {  
      servo.write(pos);  
      delay(10);      
    }
  }

  void closeValve(){
    Serial.println("The Servo Close valve!");
    for (int pos = 150; pos >= 0; pos -= 1) {  
     servo.write(pos);                        
     delay(10);                          
    }

  }

  bool timeDelay(int hours){
    long int time = millis()/(hourLengthInSecond * 1000L) + 1;
    Serial.println(millis()/(hourLengthInSecond * 1000L) + 1);

    if(time % hours == 0 && (time - previousMillis) >= hours){
    
        previousMillis = time;
         return true;
    }else{
      return false;
    }
  }

  void controlServo(){
    if(timeDelay(openFoodValveDelayInHours)){
       openValve();
       delay(1000*foodValveTimeInSecond);
       closeValve();
    }
  }

};

class WaterPump {

  public: 
  void pumpWaterSetUp(){
    pinMode(PUMP_PIN,O);
    pinMode(WATER_PIN,O);
    digitalWrite(WATER_PIN, L);
  }
  void pumpOn(){
    digitalWrite(PUMP_PIN, H);
    Serial.println("The Water Pump is ON!");
  }
  void pumpOff(){
    digitalWrite(PUMP_PIN, L);
    Serial.println("The Water Pump is OFF!");
  }

  void controlPump(){
    waterLevelOn();
    if(waterLevelValue < 300){
      pumpOn();
    }else{
      pumpOff();
    }
  }

  double waterLevelOn(){
    digitalWrite(WATER_PIN,H);
    delay(10); 
    waterLevelValue = analogRead(WATER_ANALOG_PIN);
    digitalWrite(WATER_PIN, L);
    Serial.print("Water Level value: ");
    Serial.println(waterLevelValue);
    return waterLevelValue;
  }

};

class HumidityTemperature {
  public:

  void dhtSetUp(){
    dht.begin();
    pinMode(DHT_PIN,I);
  }

  double getHumidity(){
    Serial.print("Humidity : ");
    Serial.println(dht.readHumidity());
   return dht.readHumidity();
  }

  double getTemperature(){
    Serial.print("Temp : ");
    Serial.println(dht.readTemperature());
    return dht.readTemperature();
  }

  void humidityWarn(){
    digitalWrite(DHT_LED_PIN, H);
    delay(500);
    digitalWrite(DHT_LED_PIN,L);
  }

  void dhtControl(){
    delay(500);
    humidity = getHumidity();
    temperature = getTemperature();
    if(temperature >= 20 && temperature <= 35 && humidity >= 40 && humidity <= 70){
      digitalWrite(DHT_LED_PIN,L);
    }else{
      humidityWarn();
    }
  }

};





ServoMotor servo;
WaterPump pump;
HumidityTemperature HT;


void setup() {
  Serial.begin(9600);
  servo.servoSetUp();
  pump.pumpWaterSetUp();
  HT.dhtSetUp();
}

void loop() {
  HT.dhtControl();
  pump.controlPump();
  servo.controlServo();
  Serial.print("Hi There");
}





