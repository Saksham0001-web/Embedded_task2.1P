#include <WiFiNINA.h>  // Including WiFiNINA library to enable WiFi functionality
#include "ThingSpeak.h" // Including ThingSpeak library to send data to the ThingSpeak IoT platform

char ssid[] = "saksham the great";  // WiFi network name (SSID)
char pass[] = "44saksham22";  // WiFi password
WiFiClient client;  // This will create a WiFi client object to connect to ThingSpeak

unsigned long myChannelNumber = 3026830;  // This is my ThingSpeak channel number
const char * myWriteAPIKey = "H9HTGMD79VURUJ1R";  // This is my ThingSpeak Write API Key

const int trigPin = 3;  // Here i have defined the trigger pin of the ultrasonic sensor
const int echoPin = 2;  // Here i have defined the the echo pin of the ultrasonic sensor
long duration;  // This variable stores the duration of the echo
float distance;  // This variable stores the calculated distance

void setup() {
  Serial.begin(115200);  // This will start the Serial Monitor at a baud rate of 115200
  while (!Serial);  // Waitiing for the Serial Monitor to open (only for boards with native USB)

  pinMode(trigPin, OUTPUT);  // Setting trigger pin as OUTPUT
  pinMode(echoPin, INPUT);  // Setting echo pin as INPUT

  WiFi.begin(ssid, pass);  // This will start connecting to the WiFi network
  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    delay(1000);  // Wait 1 second
    Serial.println("Connecting to WiFi...");  // This will print the status to Serial Monitor
  }
  Serial.println("Connected to WiFi.");  // This will print when connected successfully

  ThingSpeak.begin(client);  // This will initialize ThingSpeak with the WiFi client
}

void loop() {
  // Trigger the ultrasonic sensor to send a pulse
  digitalWrite(trigPin, LOW);  // This will make trigger LOW initially
  delayMicroseconds(2);  // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Sending a HIGH pulse to trigger the sensor
  delayMicroseconds(10);  // Keep it HIGH for 10 microseconds
  digitalWrite(trigPin, LOW);  // Setting the trigger LOW again

  // This will measure the time taken by echo to return
  duration = pulseIn(echoPin, HIGH);  // This will read the echo pin and measure the duration

  // Converting the duration to distance in centimeters
  distance = duration * 0.034 / 2;  // Calculating distance using the formula of  speed of sound

  Serial.print("Distance: ");  // Printing the label
  Serial.print(distance);  // Printing the measured distance
  Serial.println(" cm");  // Printing the unit

  // This will send the distance value to ThingSpeak field 1
  ThingSpeak.setField(1, distance);  // This will set field 1 with the distance value
  int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  // Write data to ThingSpeak

  if (status == 200) {  // If data is sent successfully
    Serial.println("Data sent to ThingSpeak");  // This will print confirmation
  } else {
    Serial.print("Error: ");  // This will print error message
    Serial.println(status);  // This will show the error status code
  }

  delay(30000);  // Wait for 30 seconds before sending the next reading
}
