#include <DHT.h>  // Include the DHT sensor library
#include <Servo.h> // Include the Servo library

const int sensor_pin = A0;  // Connect Soil moisture analog sensor pin to A0 of NodeMCU
const int relay_pin = D1;   // Define the relay control pin (connected to D1 for irrigation system)
const int dht_pin = D2;     // Define the pin for the DHT11 temperature/humidity sensor (connected to D2)
const int servo_pin = D4
  // Define the pin for the servo motor (connected to D3)

// Initialize DHT sensor (DHT11, DHT22, etc.)
#define DHT_TYPE DHT11  // Change to DHT22 if you're using that sensor
DHT dht(dht_pin, DHT_TYPE);  // Initialize DHT sensor

// Initialize the Servo object
Servo irrigationServo; 

void setup() {
  Serial.begin(9600);  // Define baud rate for serial communication
  pinMode(relay_pin, OUTPUT);  // Set relay pin as an output (for irrigation system)
  digitalWrite(relay_pin, LOW);  // Initially set the relay to off (assuming LOW is off)

  irrigationServo.attach(servo_pin);  // Attach the servo to the specified pin
  irrigationServo.write(0);  // Initial position of the servo (0 degrees)

  dht.begin();  // Start the DHT sensor
}

void loop() {
  float moisture_percentage;

  // Read the soil moisture sensor value and convert it to percentage
  moisture_percentage = (100.00 - ((analogRead(sensor_pin) / 1023.00) * 100.00));

  // Print the moisture percentage to the serial monitor
  Serial.print("Soil Moisture (in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  // If moisture is less than 50%, turn the relay ON to water the soil
  if (moisture_percentage < 50.0) {
    digitalWrite(relay_pin, HIGH);  // Turn relay ON (active low for irrigation system)
    irrigationServo.write(90);  // Move the servo to 90 degrees (open position for watering)
    Serial.println("Relay ON (Watering the soil - Soil is dry!)");
  } else {
    digitalWrite(relay_pin, LOW);   // Turn relay OFF
    irrigationServo.write(0);   // Move the servo to 0 degrees (closed position)
    Serial.println("Relay OFF (Soil is wet!)");
  }

  // Read temperature from DHT sensor
  float temperature = dht.readTemperature();  // Get temperature in Celsius
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
  } else {
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  // Read humidity from DHT sensor
  float humidity = dht.readHumidity();  // Get humidity percentage
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity from DHT sensor!");
  } else {
    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  delay(1000);  // Wait for 1 second before the next reading
}
