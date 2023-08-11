/*
Stay quet -> if some button pressed, waiting when being released -> read requested potentiometr (left/right) value -> move gear there. 
If pressed both, going to 90 degree position. Also goes there when init.
*/

#include <Servo.h>

Servo gearSG90;                        // create servo object to control a servo

int pinPotLeft = A1;                   // analog pin left potentiometr
int pinPotRight = A2;                  // analog pin right potentiometr
int pinGear = A0;                      // gear pin
int pinGoLeftBtn = 8;                  // button go left
int pinGoRightBtn = 9;                 // button go rigth
int valLeft;                           // left potetiometr position
int valRight;                          // right potetiometr position
int actLeftBtn;                        // to catch the activity at left btn. 0 means pressed/ 1 - released.
int actRightBtn;                       // to catch the activity at right btn. 0 means pressed/ 1 - released.
int potentiometrMaxValue;
long btnSparkDelay;                    // btn request delay to avoid pins sparking
long gearMovingDelay;                  // stay quet while te motor is runnig

//int currValLeft;                          // Reserved for current left potetiometr position
//int currValRight;                         // Reserved for current right potetiometr position
//int currentGearPosition                   // Reserved for current gear position

void setup() {
  Serial.begin(9600);
  delay(100);
  gearSG90.attach(pinGear);            
  pinMode(pinGoLeftBtn, INPUT);
  pinMode(pinGoRightBtn, INPUT);
  potentiometrMaxValue = 1023;
  btnSparkDelay = 50;              
  gearMovingDelay = 1000; 
  gearSG90.write(90); delay(1500);    
  delay(100);   
}

void loop() {

  actLeftBtn = digitalRead(pinGoLeftBtn);   
  actRightBtn = digitalRead(pinGoRightBtn);
  //if ((actLeftBtn == 1)||(actRightBtn == 0)){haveToGo}
  if (actLeftBtn == 0){HaveToGo(0);}
  if (actRightBtn == 0){HaveToGo(1);}

  delay(100);                           
}

  void HaveToGo(int goWhere){
    int bothBtnWerePressed = 0; //trigger, going to 1 if both button ever simultaneously been pressed
    int neededGearPosition = 90;
    Serial.println("I am going...");  
    
    // Start of button activities watching out. Sure that one of them was pressed, may be even both...
    delay (btnSparkDelay);
    while ((actLeftBtn == 0)||(actRightBtn == 0)){ //some button is presset, waiting while all of them are released.        
      actLeftBtn = digitalRead(pinGoLeftBtn);   
      actRightBtn = digitalRead(pinGoRightBtn);
      if ((actLeftBtn == 0) && (actRightBtn == 0)){
        bothBtnWerePressed = 1;    
      }
    }
    delay (btnSparkDelay);
    // Finish of of button activities watching out. Sure that any are pressed now
    
    if (bothBtnWerePressed == 1) {// both buttons were preseed together, go to stright position (init).
      neededGearPosition = map(neededGearPosition, 0, potentiometrMaxValue, 0, 180);
      }
    else{
    if (goWhere == 0) { // going left
      valLeft = analogRead(pinPotLeft);    // reading the value of left potentiometer
      neededGearPosition = map(valLeft, 0, potentiometrMaxValue, 0, 90);
    }
    else { // going right
      valRight = analogRead(pinPotRight);  // reading the value of right potentiometer
      neededGearPosition = map(valRight, 0, potentiometrMaxValue, 90, 180);
    }
    }
    Serial.print("going to ");Serial.println(neededGearPosition);
    gearSG90.write(neededGearPosition);  
    delay(gearMovingDelay);
    Serial.println("Done!");
  }
