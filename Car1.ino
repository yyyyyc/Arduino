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

// PINs
#define Backward_Left_Pin 4
#define Forward_Left_Pin 5
#define Backward_Right_Pin 6
#define Forward_Right_Pin 7

#define Sensor_Trig_Pin 8 // Trigger 
#define Sensor_Echo_Pin 9 // Echo

#define Display_CLK_Pin 13
#define Display_DIO_Pin 12

#define Buzzer_Pin 10

// Public variables
bool IsBackward = false;

RCSwitch mySwitch = RCSwitch();
TM1637Display myDisplay(Display_CLK_Pin, Display_DIO_Pin);
long duration, distance;

void setup() {
  // put your setup code here, to run once:
  pinMode (Forward_Right_Pin, OUTPUT);
  pinMode (Forward_Left_Pin, OUTPUT);
  pinMode (Backward_Right_Pin, OUTPUT);
  pinMode (Backward_Left_Pin, OUTPUT);
  pinMode (Buzzer_Pin, OUTPUT);
  pinMode (Sensor_Trig_Pin, OUTPUT); // The trigger of the sensor is output
  pinMode (Sensor_Echo_Pin, INPUT); // The echo of the sensor is input
   
  myDisplay.setBrightness(0x0f);
  myDisplay.clear();
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  
  Serial.begin(9600); // Monitor size (the output window size)
}

void loop() {
  tone(Buzzer_Pin, 555, 100);
  SonarSensor(); // Test the distance
  GetRemote(); // Get the RF remote control
  myDisplay.showNumberDec(distance, false);
  
   //Serial.println(distance); // Get out to the monitor the output
}

// ============================================================

void SonarSensor() {
  digitalWrite(Sensor_Trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Sensor_Trig_Pin, HIGH); // Sends the pulse
  delayMicroseconds(10);
  digitalWrite(Sensor_Trig_Pin, LOW);
  duration = pulseIn(Sensor_Echo_Pin, HIGH); // Checks how much time took to the pulse to return from the obstacle
  distance = (duration / 2) / 29.1; // Divides the duration by 2 to make it one way
  //Serial.println(distance); // Get out to the monitor the output

  if (distance < 30 && IsBackward == false) {
    Stop();
    //tone(Buzzer_Pin, distance*2, 100);
  }
  
  // tone(Buzzer_Pin, distance*2, 100);
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
  //Serial.println(IsBackward);
  
  if (mySwitch.available()) {
    
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
  digitalWrite (Forward_Right_Pin, HIGH);
  digitalWrite (Forward_Left_Pin, HIGH);
  digitalWrite (Backward_Right_Pin, LOW);
  digitalWrite (Backward_Left_Pin, LOW);
}

void GoBackward() {
  digitalWrite (Forward_Right_Pin, LOW);
  digitalWrite (Forward_Left_Pin, LOW);
  digitalWrite (Backward_Right_Pin, HIGH);
  digitalWrite (Backward_Left_Pin, HIGH);
}

void GoRight() {
  digitalWrite (Forward_Right_Pin, LOW);
  digitalWrite (Forward_Left_Pin, HIGH);
  digitalWrite (Backward_Right_Pin, LOW);
  digitalWrite (Backward_Left_Pin, LOW);
}

void GoLeft() {
  digitalWrite (Forward_Right_Pin, HIGH);
  digitalWrite (Forward_Left_Pin, LOW);
  digitalWrite (Backward_Right_Pin, LOW);
  digitalWrite (Backward_Left_Pin, LOW);
}

void Stop() {
  digitalWrite (Forward_Right_Pin, LOW);
  digitalWrite (Forward_Left_Pin, LOW);
  digitalWrite (Backward_Right_Pin, LOW);
  digitalWrite (Backward_Left_Pin, LOW);
}
