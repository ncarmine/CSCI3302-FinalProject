#include <Sparki.h>       // include the sparki library
#include "pitches.h" // include a list of pitches

#define DETECT 0 //Line Following and looking for pokemon
#define DRIVE 1 //Drive to goal using inverse kinematics
#define PICK_UP 2 //Grab pokemon
#define DROP_OFF 3 //Drop off pokemon at origin
#define GO_HOME 4 //Reset goal 

int state = DETECT; 
const float pi = 3.14159;

float maxspeed=0.0285;    // [m/s] speed of the robot that you measured
float alength=0.0851;     // [m] axle length  
float phildotr=0, phirdotr=0; // wheel speeds that you sent to the motors
float Xi=0.05, Yi=-0.05, Thetai=0; // where the robot is in the world
float Xrdot, Thetardot;    // how fast the robot moves in its coordinate system

float Xg=0.15;     // Where the robot should go
float Yg=-0.15;
float Thetag=0;

float Xline = 0.0; // Keep track of where sparki stopped searching
float Yline = 0.0;
float Thetaline = 0.0;

float alpha, rho, eta; // error between positions in terms of angle to the goal, distance to the goal, and final angle
float a=0.1, b=1, c=0.1; // controller gains

String direction;
bool returned = false; 
bool linewardBound = false; 

float ping; 

// notes in the melody:
int melody[] = { NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4 };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 150, 400, 400, 150, 150, 150, 150, 150 };

int thisNote=0;

void setup(){
    sparki.servo(45); //initial servo position
      // initialize timer1 
         noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 22250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();
}


ISR(TIMER1_COMPA_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  noInterrupts();
  tone(11, melody[thisNote],noteDurations[thisNote]);
  thisNote++;
  if(thisNote == 8){
    thisNote=0;
  }
  interrupts();
}

void loop() {
  long int time_start = millis();
  int threshold = 700;

    /* Set Sparki Direction*/
  if(Thetai <= 0 && Thetai >-0.5) {direction = "East";}
  if(Thetai <= -0.5 && Thetai >-2.75) {direction = "South";}
  if(Thetai < -2.75 && Thetai >-4.15) {direction = "West";}
  if(Thetai < -4.15 && Thetai >-6.28) {direction = "North";}
  
  switch(state){
    case DETECT:{
      ping = sparki.ping();
      int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
      int lineCenter = sparki.lineCenter(); // measure the center IR sensor
      int lineRight  = sparki.lineRight();  // measure the right IR sensor

      if ( lineCenter < threshold ) // if line is below left line sensor
      {  
        sparki.moveForward(); // move forward
        phildotr=maxspeed;
        phirdotr=maxspeed;
      }
      else{
        if ( lineLeft < threshold ) // if line is below left line sensor
        {  
          sparki.moveLeft(); // turn left
          phildotr=-maxspeed;
          phirdotr=maxspeed;
        }
      
        if ( lineRight < threshold ) // if line is below right line sensor
        {  
          sparki.moveRight(); // turn right
          phildotr=maxspeed;
          phirdotr=-maxspeed;
        }
      }
      if(ping != -1){
        if(ping <= 15){
          sparki.moveStop();
          if(direction == "East"){
            Xg = Xi + ((ping)/100)*cos(45); //servo angle is at 45 degrees
            Yg = Yi - ((ping - 10)/100)*sin(45);
         }
         if(direction == "South"){
            Xg = Xi + ((ping-10)/100)*cos(45); //servo angle is at 45 degrees
            Yg = Yi - ((ping)/100)*sin(45);
         }

         // Set location on line where sparki stopped searching
         Xline = Xi; 
         Yline = Yi; 
         Thetaline = Thetai;
         state = DRIVE;
        } 
      }
      break;
    }
    case DRIVE:{
      /*Use Inverse Kinematics to drive to goal*/
        // CALCULATE ERROR 
      rho   = sqrt((Xi-Xg)*(Xi-Xg)+(Yi-Yg)*(Yi-Yg));
      alpha = atan2(Yg-Yi,Xg-Xi)-Thetai;  
      eta   = Thetai-Thetag;

      // CALCULATE SPEED IN ROBOT COORDINATE SYSTEM
      Xrdot = a*rho;
      Thetardot = b*alpha+c*eta;

      // CALCULATE WHEEL SPEED
      phildotr = (2*Xrdot - Thetardot*alength)/(2.0);
      phirdotr = (2*Xrdot + Thetardot*alength)/(2.0);

      // SET WHEELSPEED
      if(phildotr>maxspeed){
       phildotr=maxspeed;
      }
      else if(phildotr<-maxspeed){
        phildotr=-maxspeed;
      }
      if(phirdotr>maxspeed){
        phirdotr=maxspeed;
      } else if(phirdotr<-maxspeed){
        phirdotr=-maxspeed;
      }

      float leftspeed  = abs(phildotr);
      float rightspeed = abs(phirdotr);

      leftspeed = (leftspeed/maxspeed)*100;//100
      rightspeed = (rightspeed/maxspeed)*100;//100

      if(rho > 0.01)  // if farther away than 1cm
      {
        if(phildotr > 0)
        {
          sparki.motorRotate(MOTOR_LEFT, DIR_CCW,leftspeed);
        }
        else
        {
          sparki.motorRotate(MOTOR_LEFT, DIR_CW,leftspeed);
        }
        if(phirdotr > 0)
        {
          sparki.motorRotate(MOTOR_RIGHT, DIR_CW,rightspeed);
        }
        else
        {
          sparki.motorRotate(MOTOR_RIGHT, DIR_CCW,rightspeed);
        }
      }
      else
      {
        sparki.moveStop();
        if(returned){
          state = DROP_OFF;
        }
        else if(linewardBound){
          linewardBound = false;
          state = DETECT;
        }
        else {state = PICK_UP;}
      }
      break;
    }
    case PICK_UP:{
      /* Grab Pokemon */
      sparki.gripperClose();
      delay(3500);
      sparki.gripperStop();
      sparki.moveStop();
      state = GO_HOME;
      break;
    }
    case DROP_OFF:{
      /* Drop off Pokemon */
      sparki.gripperOpen();
      delay(3500);
      returned = false;
      Xg = Xline;
      Yg = Yline;
      Thetag = Thetaline;
      linewardBound = true;
      state = DRIVE;
      break;
    }
    case GO_HOME:{
      /*Set goal to Origin*/
      Xg=0;
      Yg=0;
      state = DRIVE;
      break;
    }
  }
  
  sparki.clearLCD(); // wipe the screen
  
  sparki.print(Xi);
  sparki.print("/");
  sparki.print(Yi);
  sparki.print("/");
  sparki.print(Thetai);
  sparki.println();
  sparki.print(alpha/PI*180);
  sparki.println();
  sparki.print("State: ");
  sparki.println(state);
  sparki.print("Goal:");
  sparki.print(Xg);
  sparki.print("/");
  sparki.print(Yg);
  sparki.print("/");
  sparki.print(Thetag);
    
  sparki.updateLCD(); // display all of the information written to the screen

  // perform odometry
  Xrdot=phildotr/2.0+phirdotr/2.0;
  Thetardot=phirdotr/alength-phildotr/alength;
  
  Xi=Xi+cos(Thetai)*Xrdot*0.1;
  Yi=Yi+sin(Thetai)*Xrdot*0.1;
  Thetai=Thetai+Thetardot*0.1;

  while(millis()<time_start+100); // wait until 100ms have elapsed
}

