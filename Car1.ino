#include <RCSwitch.h>
#include <TM1637Display.h>

long RF_ReceivedValue;

/*
// for Remote Control #10 (in Lab)
#define RF_BUTTON_UP 95491 // up
#define RF_BUTTON_LEFT 95536 // left
#define RF_BUTTON_CENTER 95695 // center
#define RF_BUTTON_RIGHT 95692 // right
#define RF_BUTTON_DOWN 95500 // down
*/

// for Remote Control I took home (With Antenna)
#define RF_BUTTON_UP 5592323 // up (button 1)
#define RF_BUTTON_LEFT 5592371 // left (button 5)
#define RF_BUTTON_CENTER 5592368 // center (button 4)
#define RF_BUTTON_RIGHT 5592380 // right (button 6)
#define RF_BUTTON_DOWN 5592335 // down (button 3)

#define Backward_Left 4
#define Forward_Left 5
#define Backward_Right 6
#define Forward_Right 7

#define Sensor_TrigPin 8 // Trigger 
#define Sensor_EchoPin 9 // Echo

#define CLK 13
#define DIO 12

bool IsBackward = false;

RCSwitch mySwitch = RCSwitch();
TM1637Display myDisplay(CLK, DIO);
long duration, distance;

void setup() {
  // put your setup code here, to run once:
  pinMode (Forward_Right, OUTPUT);
  pinMode (Forward_Left, OUTPUT);
  pinMode (Backward_Right, OUTPUT);
  pinMode (Backward_Left, OUTPUT);

  Serial.begin(9600); // Monitor size (the output window size)
  pinMode(Sensor_TrigPin, OUTPUT); // The trigger of the sensor is output
  pinMode(Sensor_EchoPin, INPUT); // The echo of the sensor is input  
  myDisplay.setBrightness(0x0f);
  myDisplay.clear();
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  // put your main code here, to run repeatedly:

  SonarSensor(); // Test the distance
  //GetRemote(); // Test the RF remote control
  GetRemote();
  myDisplay.showNumberDec(distance, false);
  
   //Serial.println(distance); // Get out to the monitor the output
  /*Stop();
  delay (300000000);
  GoForward();
  delay (3000);
  Stop();
  delay (3000);  
  GoBackward();
  delay (2000);
  GoRight();
  delay (2000);
  GoLeft();
  delay (1000);
  Stop();
  delay (3000);*/
}

// ============================================================

void SonarSensor() {
  digitalWrite(Sensor_TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(Sensor_TrigPin, HIGH); // Sends the pulse
  delayMicroseconds(10);
  digitalWrite(Sensor_TrigPin, LOW);
  duration = pulseIn(Sensor_EchoPin, HIGH); // Checks how much time took to the pulse to return from the obstacle
  distance = (duration / 2) / 29.1; // Divides the duration by 2 to make it one way
  //Serial.println(distance); // Get out to the monitor the output

  if (distance < 30 && IsBackward == false) {
    Stop();
  }
  
  // tone(buzzerPin, distance*2, 100);
}
void GetRF() {
  RF_ReceivedValue = 0;
  if (mySwitch.available()) {
    if (mySwitch.getReceivedValue()) {
      RF_ReceivedValue = mySwitch.getReceivedValue();
    }
    mySwitch.resetAvailable();
  }

  delay(5);
  //if (RF_ReceivedValue != 0)
  //  Serial.println(RF_ReceivedValue);
}

void GetRemote() {
  //long switchButton = 0;
  Serial.println(IsBackward);
  
  if (mySwitch.available()) {
    //Serial.println(mySwitch.getReceivedValue());
    
    long switchButton = 0;
    switchButton = mySwitch.getReceivedValue();
    IsBackward = false;
    
    switch (switchButton) {
      case RF_BUTTON_UP:
        GoForward();
        break;
      case RF_BUTTON_LEFT:
        GoLeft();
        IsBackward = true;
        break;
      case RF_BUTTON_CENTER:
        Stop();
        break;
      case RF_BUTTON_RIGHT:
        GoRight();
        IsBackward = true;
        break;
      case RF_BUTTON_DOWN:
        GoBackward();
        IsBackward = true;
        break;        
      default:
        Stop();
    }
    
    mySwitch.resetAvailable();
  }
}

// ============================================================

void GoForward() {
  digitalWrite (Forward_Right, HIGH);
  digitalWrite (Forward_Left, HIGH);
  digitalWrite (Backward_Right, LOW);
  digitalWrite (Backward_Left, LOW);
}

void GoBackward() {
  digitalWrite (Forward_Right, LOW);
  digitalWrite (Forward_Left, LOW);
  digitalWrite (Backward_Right, HIGH);
  digitalWrite (Backward_Left, HIGH);
}

void GoRight() {
  digitalWrite (Forward_Right, LOW);
  digitalWrite (Forward_Left, HIGH);
  digitalWrite (Backward_Right, LOW);
  digitalWrite (Backward_Left, LOW);
}

void GoLeft() {
  digitalWrite (Forward_Right, HIGH);
  digitalWrite (Forward_Left, LOW);
  digitalWrite (Backward_Right, LOW);
  digitalWrite (Backward_Left, LOW);
}

void Stop() {
  digitalWrite (Forward_Right, LOW);
  digitalWrite (Forward_Left, LOW);
  digitalWrite (Backward_Right, LOW);
  digitalWrite (Backward_Left, LOW);
}
