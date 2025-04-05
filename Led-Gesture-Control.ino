#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);  // Instantiate an MPU6050 object

int ledPin = 10;   // Pin for the LED (PWM-capable pin)
float threshold = 0.3; // Gesture threshold to detect significant movement

int ledBrightness = 0;  // Start with the LED off (brightness = 0)
bool ledOn;

void setup() {
  Serial.begin(9600);  // Start serial communication
  Wire.begin();        // Start I2C communication
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");

  pinMode(ledPin, OUTPUT);  // Set LED pin as output
}

void loop() {
  // Read raw accelerometer and gyroscope values
  mpu.update();
  float ax, ay, az;
  float rx, ry, rz;
  ax = mpu.getAccX();
  ay = mpu.getAccY();
  az = mpu.getAccZ();
  rx = mpu.getAngleX();
  ry = mpu.getAngleY();
  rz = mpu.getAngleZ();
  
  // Calculate the accelerometer values for tilt detection
  float tiltLeftRight = ax;  // X-axis accelerometer value

  Serial.print("tiltLeftRight: ");
  Serial.println(tiltLeftRight);

  // Detect tilt gestures for left/right (to control on/off state)
  if (tiltLeftRight > threshold) {
    Serial.println("Tilt Left Detected");
    digitalWrite(ledPin, LOW); // Turn LED off
    ledOn = false;

  } else if (tiltLeftRight < -threshold) {
    Serial.println("Tilt Right Detected");
    digitalWrite(ledPin, HIGH);  // Turn LED on
    ledOn = true;

  }
  if (ledOn){
    ledBrightness = map(rz, -90, 90 , 0,255);
    analogWrite(ledPin, ledBrightness); // Update LED brightness
  }

  delay(100); // Debounce delay to avoid multiple triggers
}
