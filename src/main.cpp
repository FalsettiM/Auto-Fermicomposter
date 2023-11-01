#include <Arduino.h>
#include <Servo.h>
#include <Stepper.h>
#include <dht_nonblocking.h>
#include "SR04.h"

// Stepper motor parameters
const int stepsPerRevolution =
    2048;  // change this to fit the number of steps per revolution
const int rolePerMinute =
    15;  // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // Define stepper

// DC motor parameters
const int dcEnable = 5;  // DC enable pin
const int dcDirA = 3;    // Pin for direction A
const int dcDirB = 4;    // Pin for direction B

// Distance Sensor Parameters
const int sensTrigPin = 6;
const int sensEchoPin = 7;
SR04 sr04 = SR04(sensEchoPin, sensTrigPin);
long a;

// Servo motor
const int servoPin = 13;
Servo myservo;

// Humidity Sensor
static const int DHT_SENSOR_PIN = 52;
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_TYPE_11);

// Liquid level sensor int adc_id = 0;
int adc_id = 13;
char printBuffer[128];

// Buttons
const int stepperButton = 40;
const int dcButton = 42;
const int servoButton = 2;

// Reverse toggle
bool reverseState = false;

void setup() {
    // Stepper motor
    myStepper.setSpeed(rolePerMinute);

    // DC Motor
    pinMode(dcEnable, OUTPUT);
    pinMode(dcDirA, OUTPUT);
    pinMode(dcDirB, OUTPUT);

    digitalWrite(dcDirA, HIGH);
    digitalWrite(dcDirB, LOW);

    // initialize the serial port:
    Serial.begin(9600);

    // Servo
    myservo.attach(servoPin);
    myservo.write(0);
}

void loop() {
    digitalWrite(dcDirA, HIGH);
    digitalWrite(dcDirB, LOW);
    
    // Run the stepper motor
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution/4);
    delay(1000);

    // Other direction
    Serial.println("counterclockwise");
    myStepper.step(-stepsPerRevolution /4);
    delay(1000);

    Serial.println("Servo Movement");
    myservo.write(80);  // Open
    delay(5000);
    Serial.println("Servo Movement");
    myservo.write(0);  // Close
    delay(5000);

    // Serial.println("DC Motor");
    // digitalWrite(dcEnable, HIGH);  // enable on

    // digitalWrite(dcDirA, HIGH);  // one way
    // digitalWrite(dcDirB, LOW);
    // delay(500);

    // digitalWrite(dcDirA, LOW);  // OFF
    // digitalWrite(dcDirB, LOW);
    // delay(1000);

    // Ultrasonic Sensor
    a = sr04.Distance();
    Serial.print(a);
    Serial.println("cm");

    

    // Humidity and Temperature
    float temperature;
    float humidity;
    while (true) {
        if (dht_sensor.measure(&temperature, &humidity) == true) {
            Serial.print("T = ");
            Serial.print(temperature, 1);
            Serial.print(" deg. C, H = ");
            Serial.print(humidity, 1);
            Serial.println("%");
            break;
        }
    }

    // Water level sensor
    int value = analogRead(adc_id);  // get adc value
    sprintf(printBuffer, "ADC%d level is %d\n", adc_id, value);
    Serial.print(printBuffer);
}