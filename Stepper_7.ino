#include <Stepper.h>
#include <IRremote.h>

int in1Pin=8;
int in2Pin=9;
int in3Pin=10;
int in4Pin=11;

int steps;
int StepsPerRev=2048;
int StepperSpeed=10;

int StepNumber=0;
int ReverseSteps;
int StepsTierTwo; // needs value

//Remote
int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
int RemoteState=1;

// int Distance=0.2; // distance input (m)
int RevsPerMetre; //needs value (experimental)
// int StepsTierTwo=RevsPerMetre*Distance*StepsPerRev;
int StepsPerMetre=1250;
int StepsTierTwoTest=StepsPerMetre*0.1;




//Switch
int switchPin=3;
int SwitchState=1;
int ReverseDirection=0; // breakout condition

//Button
int buttonPin=2;
int ButtonState=1; // off
int RunProgram=0;

Stepper myStepper(StepsPerRev, in1Pin, in3Pin, in2Pin, in4Pin); //The first parameter is the number of 'steps' that the motor will take to complete one revolution.


void setup() {
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);
  
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600);
}

void loop() {
  
while(RemoteState==1){
if(irrecv.decode(&results)){
  if(results.value == 0xFFE21D){
  RemoteState=0;
  delay(1000);
  }
  }
  Serial.println("No remote button has been pressed");
}irrecv.resume(); // Receive the next value

 /*while (ButtonState==1) {
    Serial.println("Button has not been pressed");
      ButtonState=digitalRead(buttonPin); // has to be within while loop since the program is stuck within the while loop and cannot read this outside of the loop, hence why this statement was written once then never again
  }*/

    
    delay(3000); //startup delay of 5 seconds
    Serial.println(ReverseDirection);
  
    while (ReverseDirection==0) {
      Serial.print("Switch has not been pressed yet. Steps taken: ");
      SwitchState = digitalRead(switchPin);
      StepNumber++; // increment step number by 1
      Serial.println(StepNumber);
      // *********** MOTOR********
      steps=10;  // Steps to take per loop
      myStepper.setSpeed(5);   
      myStepper.step(steps);
      // ************MOTOR********
      if (SwitchState == 0){ // switch has been pressed
        ReverseDirection=1; // Causes the program to break out of the while loop's iron grip
        delay(1000);
      }
    } // end of while loop that causes motor to go forward
    
    int RemainingSteps=StepNumber+5;
    Serial.println("Switch has been pressed");
    while (RemainingSteps>=1 && ReverseDirection==1){
      Serial.print("Remaining steps are ");
      Serial.println(RemainingSteps);
      RemainingSteps=RemainingSteps-1;
      steps=-10;  // minus sign reverses direction
      myStepper.setSpeed(5);
      myStepper.step(steps);

      }
      delay(7000); //delay for 7 secoonds before starting tier two
      
      //***** Second phase******
      for (int im = StepsTierTwoTest; im > 0; im--){
      myStepper.setSpeed(5);   
      myStepper.step(10); // replace StepsPerRev with the calculation e.g. StepsTierTwo=RevsPerMetre(experimental)*Distance*StepsPerRev
      //***** Second phase******
      }
      ButtonState=1;
      ReverseDirection=0; // remove this line this if there is an error
      StepNumber=0;
      RemoteState=1;

}
