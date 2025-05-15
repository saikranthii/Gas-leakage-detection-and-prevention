#include <Servo.h>
#include <SoftwareSerial.h>

// Initialize Servo and SoftwareSerial for SIM900A
Servo myServo;
SoftwareSerial SIM900A(10, 11); // RX, TX (SIM900A RX to Arduino pin 10, TX to pin 11)

// Pin definitions
int gasSensorPin = A0; 
int ledPin = 13;
int servoPin = 9; 
int gasLevel;
int currentServoPosition = 0;

// Phone number for SMS and call (replace with your number, e.g., +911234567890)
const char phoneNumber[] = "+91XXXXXXXXXX";

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize SIM900A communication
  SIM900A.begin(9600);
  delay(1000); // Allow module to stabilize
  Serial.println("SIM900A Ready");

  // Set SMS mode to text
  SIM900A.println("AT+CMGF=1");
  delay(100);
  
  // Configure pins
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);
}

void loop() {
  // Read gas sensor value
  gasLevel = analogRead(gasSensorPin);
  
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);

  if (gasLevel >= 300) {
    // Gas detected: Turn on LED, move servo, send SMS, and make call
    digitalWrite(ledPin, HIGH);
    myServo.write(180);
    
    // Send SMS
    SendSMS();
    
    // Make call
    MakeCall();
    
    delay(1000); // Wait before next check
  } else {
    // No gas detected: Reset LED and servo
    myServo.write(0);
    digitalWrite(ledPin, LOW);
    delay(1000); // Wait before next check
  }
  
  delay(1000); // Additional delay for loop stability
}

// Function to send SMS
void SendSMS() {
  Serial.println("Sending SMS");
  SIM900A.println("AT+CMGF=1"); // Set SMS to text mode
  delay(100);
  SIM900A.print("AT+CMGS=\"");
  SIM900A.print(phoneNumber);
  SIM900A.println("\""); // Specify phone number
  delay(100);
  SIM900A.println("Gas Leakage Detected! Immediate Action Required."); // Message content
  delay(100);
  SIM900A.println((char)26); // CTRL+Z to send SMS
  delay(1000);
  Serial.println("SMS Sent");
}

// Function to make a call
void MakeCall() {
  Serial.println("Making Call");
  SIM900A.print("ATD");
  SIM900A.print(phoneNumber);
  SIM900A.println(";"); // Initiate call
  delay(20000); // Wait 20 seconds for call to connect
  SIM900A.println("ATH"); // Hang up call
  delay(1000);
  Serial.println("Call Ended");
}