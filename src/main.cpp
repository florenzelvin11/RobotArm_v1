#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "Button\Button.h"
#include "ArrayList\ArrayList.h"
#include "ArrayList\ArrayList.cpp"

// Servo Initialisations
Servo base;
Servo arm;
Servo foreArm;
Servo wristRoll;
Servo wristPitch;

// Servo pin - digital In/Outputs
const int pinBase = 8;
const int pinArm = 7;
const int pinForeArm = 6;
const int pinWristRoll = 5;
const int pinWristPitch = 4;

// LCD - Screen
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buttons
const int buttonA = 12;
const int buttonB = 11;
const int buttonC = 10;
const int buttonD = 9;

Button buttons(buttonA, buttonB, buttonC, buttonD);

// PotentioMeters
const int potBase = A0;
const int potArm = A1;
const int potForeArm = A2;
const int potWristRoll = A3;
const int potWristPitch = A7;

// Servo Pos 
int recBase[5] = { 0, 0, 0, 0, 0};
int recArm[5] = { 0, 0, 0, 0, 0};
int recForeArm[5] = { 0, 0, 0, 0, 0};
int recWristRoll[5] = { 0, 0, 0, 0, 0};
int recWristPitch[5] = { 0, 0, 0, 0, 0};
int index = -1, Max = 5;

// A collection of servos - for whe iterating through each servo position on play mode
Servo servos[5] = { base, arm, foreArm, wristRoll, wristPitch };

// Function Prototypes
void servo_init();
void lcd_init();
void RecMode();
void FreeMode();
void MainMenu();
int AnalogToDegree(int pot);
void MoveMotors();
void RecordPos(int pos);
void PlayMode();
void PlayPos();
void servoMoveConfig(int pos);

// Start-Up function
void setup() {
  Serial.begin(9600);
  servo_init();
  lcd_init();
  buttons._init_();
}

// Constant Function
void loop() {
  MainMenu();
}

// Init functions
// Servo
void servo_init(){
  base.attach(pinBase);
  arm.attach(pinArm);
  foreArm.attach(pinForeArm);
  wristRoll.attach(pinWristRoll);
  wristPitch.attach(pinWristPitch);
}

// LCD
void lcd_init(){
  lcd.init();
  lcd.backlight();
}

// User Interface (UI)
// The Main Menu function
void MainMenu(){
  int keyDown = buttons.keyPressed();
  switch(keyDown){
    case 1:
      Serial.println("KeyPressed: A");
      FreeMode();
      break;
    case 2:
      Serial.println("KeyPressed: B");
      RecMode();
      break;
    case 3:
      Serial.println("KeyPressed: C");
      PlayMode();
      break;
    case 4:
      Serial.println("KeyPressed: D");

      break;
    default:
     // Main Menu UI
      lcd.setCursor( 0, 0);
      lcd.print("Mode Selection: ");
      lcd.setCursor( 0, 1);
      lcd.print("Free  Rec  Play");
  }
}

// Free Mode Interface
void FreeMode(){
  lcd.clear();  // Clears the screen for a new interface
  bool status = true;
  while(status){
    if(buttons.keyPressed() == 1){ status = false; }
    lcd.setCursor(0,0);
    lcd.print("Free Mode: ");
    MoveMotors();
  }
  lcd.clear();  // Clears again for when the function is done
}

void RecMode(){
  lcd.clear();
  bool status_1 = true, status_2 = false;
  
  while(status_1){  
    lcd.setCursor(0,0);
    lcd.print("Record Mode:");
    lcd.setCursor(0,1);
    lcd.print("Pos - ");
    MoveMotors();
    RecordPos(index);
    // This is the record function to recording each positions - Max 5 pos to record
    int key = buttons.keyPressed();
    if(index < Max){
      if(key == 2){
        status_1 = false;
      }else if(key == 3){
        index++; lcd.setCursor( 7, 1); lcd.print("   "); lcd.setCursor( 7, 1); lcd.print(index + 1);
        continue;
      }else if(key == 4){
        index--; lcd.setCursor( 7, 1); lcd.print("   "); lcd.setCursor( 7, 1); lcd.print(index + 1);
        continue;
      }
    }else if(index < -1){
      lcd.setCursor( 7, 1); lcd.print("   ");
      index = -1;
    }else if(index >= Max){
      lcd.clear();
      status_2 = true;
    
        while(status_2){
          lcd.setCursor(0,0);
          lcd.print("Record Mode: Max");
          lcd.setCursor(0,1);
          lcd.print("Edit|Reset|Exit");
          int key = buttons.keyPressed();
          if(key == 1){
            // Edit Back the Pos
            index = -1;
            status_1 = true;
            status_2 = false;
            lcd.clear();
          }else if(key == 2){
            // Reset The every array
            index = -1;
            for(int i = 0; i < 5; i++){
              recBase[i] = 0;
              recArm[i] = 0;
              recForeArm[i] = 0;
              recWristRoll[i] = 0;
              recWristPitch[i] = 0;
            }
            status_2 = false;
            lcd.clear();
          }else if(key == 3){
            // Exits The RecMode Menu
            status_1 = false;
            status_2 = false;
            lcd.clear();
            break;
          } 
      }
      continue;
    }
  }

  lcd.clear();
}

// Play Mode UI
void PlayMode(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Play Mode:");
  lcd.setCursor(0,1);
  lcd.print("Pos - ");
  PlayPos();
  lcd.setCursor( 7, 1); lcd.print("   "); lcd.setCursor( 7, 1); lcd.print("Done");
  delay(1000);
  lcd.clear();
}

// Movement Applications
// Converts the analog value from the potentiometers (i.e. (0V - 5V) = (0 - 1023)) to digital (0 - 255)  
int AnalogToDegree(int pot){
  int read = analogRead(pot);
  return map(read, 0, 1023, 10, 170);
}

// Moves the robot arm components manually, through forward kinematics
void MoveMotors(){
  base.write(AnalogToDegree(potBase));
  arm.write(AnalogToDegree(potArm));
  foreArm.write(AnalogToDegree(potForeArm));
  wristRoll.write(AnalogToDegree(potWristRoll));
  wristPitch.write(AnalogToDegree(potWristPitch));
}

// Reads each current positions of robot's servos then stores it into an array
void RecordPos(int pos){
  if((pos >= 0) && (pos < 5)){
    recBase[pos] = base.read();
    recArm[pos] = arm.read();
    recForeArm[pos] = foreArm.read();
    recWristRoll[pos] = wristRoll.read();
    recWristPitch[pos] = wristPitch.read();
  }
}

void PlayPos(){
  for(int i = 0; i < 5; i++){
    lcd.setCursor( 7, 1); lcd.print("   "); lcd.setCursor( 7, 1); lcd.print(i + 1);
    servoMoveConfig(i);
    delay(250);
  }
}

void servoMoveConfig(int pos){
  int posBase = base.read();
  int posArm = arm.read();
  int posForeArm = foreArm.read();
  int posWristRoll = wristRoll.read();
  int posWristPitch = wristPitch.read();
  delay(250);
  // bool doneA = true, doneB = true, doneC = true, doneD = true, doneE = true;
  while(posBase != recBase[pos] || posArm != recArm[pos] || posForeArm != recForeArm[pos] || posWristRoll != recWristRoll[pos] || posWristPitch != recWristPitch[pos]){
    // Constantly updates the current position of the servos simultaneously
    
    delayMicroseconds(50);
    // WristPitch Movement
    if(posWristPitch > recWristPitch[pos]){
      posWristPitch--;
      wristPitch.write(posWristPitch);
    }else if(posWristPitch < recWristPitch[pos]){
      posWristPitch++;
      wristPitch.write(posWristPitch);     
    }else{
      wristPitch.write(posWristPitch);
    }
    delayMicroseconds(50);

    // WristRoll Movement
    if(posWristRoll > recWristRoll[pos]){
      posWristRoll--;
      wristRoll.write(posWristRoll);
    }else if(posWristRoll < recWristRoll[pos]){
      posWristRoll++;
      wristRoll.write(posWristRoll);     
    }else{
      wristRoll.write(posWristRoll);
    }
    delayMicroseconds(50);

    // ForeArm Movement
    if(posForeArm > recForeArm[pos]){
      posForeArm--;
      foreArm.write(posForeArm);
    }else if(posForeArm < recForeArm[pos]){
      posForeArm++;
      foreArm.write(posForeArm);
    }else{
      foreArm.write(posForeArm);
    }
    delay(5);

    // Arm Movement
    if(posArm > recArm[pos]){
      posArm--;
      arm.write(posArm);
    }else if(posArm < recArm[pos]){
      posArm++;
      arm.write(posArm);
    }else{
      arm.write(posArm);
    }
    delay(5);

    // Base Movement
    if(posBase > recBase[pos]){
      posBase--;
      base.write(posBase);
    }else if(posBase < recBase[pos]){
      posBase++;
      base.write(posBase);
    }else{
      base.write(posBase);
    }
  }
}


