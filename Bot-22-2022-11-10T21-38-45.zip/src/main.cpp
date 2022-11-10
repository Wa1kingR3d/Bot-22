/*
 
|---------------------------|
| Author: Avery T.          |
| Team(s): 97045E & 97045G  |
| Date Modified: 11/3/22    |
| Drivetrain(dt): Tank      |
|---------------------------|

*/
//---includes---//
#include "vex.h"
#include <iostream>

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Bumper               bumper        A               
// ---- END VEXCODE CONFIGURED DEVICES ----

//---namespace---//
using namespace vex;

// defines names
brain Brain;
competition Competition;

//---Devices---//
// define motors
motor  LFMotor   = motor(PORT8, ratio18_1, false); // LFmotor stands for left front motor
motor  LBMotor   = motor(PORT7, ratio18_1, false); // LBmotor stands for left back motor
motor  RFMotor   = motor(PORT10, ratio18_1, true); // RFmotor stands for right front motor
motor  RBMotor   = motor(PORT9, ratio18_1, true); // LBmotor stands for right back motor
motor  intake    = motor(PORT2, ratio18_1, false); // the section that sucks up the frisbees   
motor  FW_1      = motor(PORT6, ratio6_1, false); // the wheel that flings the firsbees part 1
motor  FW_2      = motor(PORT4, ratio6_1, false); // the wheel that flings the firsbees part 2
motor  shooter   = motor(PORT5, ratio18_1, false); // the motor that pushes the frisbees into the flywheel
motor  catapult  = motor(PORT14, ratio18_1, false);

// define drivetrain group
motor_group  leftDrive  = motor_group(LFMotor, LBMotor);
motor_group  rightDrive = motor_group(RFMotor, RBMotor);
drivetrain   dt         = drivetrain(leftDrive, rightDrive, 319.19, 295, 40, mm, 1);

// define flywheel group
motor_group flywheel = motor_group(FW_1, FW_2);

//define sensors
bumper Bumper = bumper(Brain.ThreeWirePort.A);

// define controller(s)
controller controller1 = controller(primary);


//---Autonomus section---//
void autonomus(void)
{
  // autonomus here
}

//---TeleOp section---//

void TeleOp(void)
{
  // bools for toggle states
  bool C1LisRunning = true; //C1-controller 1, L-left
  bool C1RisRunning = true; //C1-controller 1, R-right

  // bools for motor status
  bool C1intakeon   = false;
  bool C1intakeFI   = true;  //FI - first iteration
  bool C1flywheelon = false;
  bool C1flywheelFI = true;  //FI - first iteration
  bool C1shooton    = false;

  // init frisbee amount
  int frisCount = 0; // counts how many frisbees are in the bot

  //sets screen
  controller1.Screen.clearScreen();
  // sets frisCount
  controller1.Screen.setCursor(1, 1);
  controller1.Screen.print("Frisbees: 0");
  // sets flywheel
  controller1.Screen.setCursor(2,1);
  controller1.Screen.print("Flywheel: Off");
  // sets intake
  controller1.Screen.setCursor(3,1);
  controller1.Screen.print("Intake: Off");


  // main loop  
  for(int i = 0; i == 0; i += 0)
  {
    // displays to the controller screen
    // flywheel
    if(C1flywheelon && C1flywheelFI) 
    {
      controller1.Screen.clearLine(2);
      controller1.Screen.setCursor(2,1);
      controller1.Screen.print("Flywheel: On");
      C1flywheelFI = false;
    } 
    else if(!C1flywheelon && !C1flywheelFI)
    {
      controller1.Screen.clearLine(2);
      controller1.Screen.setCursor(2,1);
      controller1.Screen.print("Flywheel: Off");
      C1flywheelFI = true;
    }

    // intake
    if(C1intakeon && C1intakeFI) 
    {
      controller1.Screen.clearLine(3);
      controller1.Screen.setCursor(3,1);
      controller1.Screen.print("Intake: On");
      C1intakeFI = false;
    } 
    else if(!C1intakeon && !C1intakeFI)
    {
      controller1.Screen.clearLine(3);
      controller1.Screen.setCursor(3,1);
      controller1.Screen.print("Intake: Off");
      C1intakeFI = true;
    }

    //sets axis position from controllers
    int LDTpos = controller1.Axis3.position();
    int RDTpos = controller1.Axis2.position();
    
    // set velocity of motors
    intake.setVelocity(100, percent);
    FW_1.setVelocity(100, percent);
    FW_2.setVelocity(100, percent);
    shooter.setVelocity(100, percent);
  
    
    // drive train
    // checks if left values are in deadband range
    if(LDTpos < 5 && LDTpos > -5)
    {
      // checks if the bool is already on
      if(C1LisRunning)
      {
        leftDrive.stop(); // stop the motors
        C1LisRunning = false; // set bool to false 
      }
    }
    else
    {
      C1LisRunning = true; // set bool to true
    }

    // checks if right values are in deadband range
    if(RDTpos < 5 && RDTpos > -5)
    {
      // checks if the bool is already on
      if(C1RisRunning)
      {
        rightDrive.stop(); // stop the motors
        C1RisRunning = false; // set bool to false 
      }
    }
    else
    {
      C1RisRunning = true; // set bool to true
    }

    // main drive for left side
    if(C1LisRunning)
    {
      leftDrive.setVelocity(LDTpos, percent);
      leftDrive.spin(forward);
    }

    // main drive for right side
    if(C1RisRunning)
    {
      rightDrive.setVelocity(RDTpos, percent);
      rightDrive.spin(forward);
    }


    // buttons :D
    // intake (toggle)
    if(controller1.ButtonL2.pressing())
    {
      if(!C1intakeon) // acts based on state 'C1intakeon'
      {
        intake.spin(forward); // spins wheel
        C1intakeon = true; // sets state
      }
      else if(C1intakeon)
      {
        intake.stop(); // stops wheel
        C1intakeon = false; // sets state
      }
    }
    while(controller1.ButtonL2.pressing()) // pauses to not waste resources
    {
      wait(1, msec);
    }


    // flywheel (toggle) NOTE: The flywheel has the same mechanics as the intake
    if(controller1.ButtonL1.pressing())
    {
      if(!C1flywheelon)
      {
        flywheel.spin(forward);
        C1flywheelon = true;
      }
      else if(C1flywheelon)
      {
        flywheel.stop();
        C1flywheelon = false;
      }
    }
    while(controller1.ButtonL1.pressing())
    {
      wait(1, msec);
    }

    // shoot
    if(controller1.ButtonR2.pressing() && !C1shooton)
    {
      C1shooton = true;
      shooter.spinFor(forward, 90, degrees);
      shooter.spinFor(reverse, 90, degrees);
      C1shooton = false;
    }
    
    // sensorss XDDDD
    double s = Bumper.value();

    wait(20,msec);
  }
}


int main()
{
  Competition.autonomous(autonomus);
  Competition.drivercontrol(TeleOp);

  while(1)
  {
    task::sleep(100);
  }
}
