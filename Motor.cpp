

#include "Motor.h"


boolean moveStop = false;
boolean received = false;
boolean sent = false;

Motor::Motor()
{
  ID = 0;
  Name = "";
  halfActive = false;
  halfStepPosition = false; 
  currentPosition = 0;
  pStatus = 0;
  mspeed = 5;
  steps = 0;
};

Motor::Motor(int mID, String name)
{
  ID = mID;
  Name = name;
  halfActive = false;
  halfStepPosition = false; 
  currentPosition = 0;
  pStatus = 0;
  mspeed = 5;
  steps = 0;
};

Command Motor::com;

void Motor::setCommand(char* msg)
{
  int counter = Message::Count(msg);
  if(counter < 3)      
    return;

  counter -= 2;
  char comsteps[counter]; 
  
  for(int i = 0; i < counter; i++)
    comsteps[i] = msg[i+1];
  comsteps[counter] = '\0';
   
  com.id = (int)msg[0] - 48; 
  com.halfstep = (int)msg[Message::Count(msg)-1] - 48 >0 ? 1:0;
  com.steps = strtol(comsteps,0,10);

}



void Motor::sendStatus()
{
  if(Serial)
  {
    Serial.print(com.id);
    Serial.print(com.steps);
    Serial.print(com.halfstep);
    Serial.println();
    
  }   
}
            
void Motor::activateMotor(uint8_t mcode)
{
  switch(mcode)
  {
    case 1: 
            digitalWrite(RA0,LOW);
            digitalWrite(RA1,LOW);
            digitalWrite(RA2,LOW);
            break;
    case 2:
            digitalWrite(RA0,HIGH);
            digitalWrite(RA1,LOW);
            digitalWrite(RA2,LOW);
            break;
    case 3:
            digitalWrite(RA0,LOW);
            digitalWrite(RA1,HIGH);
            digitalWrite(RA2,LOW);
            break;
    case 4:
            digitalWrite(RA0,HIGH);
            digitalWrite(RA1,HIGH);
            digitalWrite(RA2,LOW);
            break;
    case 5:
            digitalWrite(RA0,LOW);
            digitalWrite(RA1,LOW);
            digitalWrite(RA2,HIGH);
            break;
    case 6:
            digitalWrite(RA0,HIGH);
            digitalWrite(RA1,LOW);
            digitalWrite(RA2,HIGH);
            break;
    default:
            Serial.print("UNKNOWN MOTOR");    
  }
}

void Motor::moveHalfStep(Direction dir)
{
  if(halfStepPosition)
  {        
    currentPosition = floor(currentPosition);
    moveStep(dir);
    halfStepPosition = false;
    return;
  }
  
  switch(pStatus)
  {
    case 1:
           if(dir == CW)
           { 
             digitalWrite(P0,LOW);
             digitalWrite(P1,LOW);
             digitalWrite(P2,HIGH);
             digitalWrite(P3,LOW);
             currentPosition += 0.5;
           }
           else
           {
             digitalWrite(P0,HIGH);
             digitalWrite(P1,LOW);
             digitalWrite(P2,LOW);
             digitalWrite(P3,LOW);
             currentPosition -= 0.5;
           }
           break;
    case 2:
           if(dir == CW)
           { 
             digitalWrite(P0,LOW);
             digitalWrite(P1,HIGH);
             digitalWrite(P2,LOW);
             digitalWrite(P3,LOW);
             currentPosition += 0.5;
           }
           else
           {
             digitalWrite(P0,LOW);
             digitalWrite(P1,LOW);
             digitalWrite(P2,HIGH);
             digitalWrite(P3,LOW);
             currentPosition -= 0.5;
           }
           break;
    case 3: 
           if(dir == CW)
           { 
             digitalWrite(P0,LOW);
             digitalWrite(P1,LOW);
             digitalWrite(P2,LOW);
             digitalWrite(P3,HIGH);
             currentPosition += 0.5;
           }
           else
           {
             digitalWrite(P0,LOW);
             digitalWrite(P1,HIGH);
             digitalWrite(P2,LOW);
             digitalWrite(P3,LOW);
             currentPosition -= 0.5;
           }
           break;
     case 4:
           if(dir == CW)
           { 
             digitalWrite(P0,HIGH);
             digitalWrite(P1,LOW);
             digitalWrite(P2,LOW);
             digitalWrite(P3,LOW);
             currentPosition += 0.5;
             pStatus = 0;
           }
           else
           {
             digitalWrite(P0,LOW);
             digitalWrite(P1,LOW);
             digitalWrite(P2,LOW);
             digitalWrite(P3,HIGH);
             currentPosition -= 0.5;
           }
           break;
   }
   
   halfStepPosition = true;
  
}

void Motor::moveStep(Direction dir)
{
  switch(dir)
  {
    case CW:
            currentPosition++;
            pStatus++;
            if(pStatus > 4)
              pStatus = 1;
            break;
    case CCW:
            currentPosition--;
            pStatus--;
            if(pStatus < 1)
              pStatus = 4;
            break;          
  }
  switch(pStatus)
  {
    case 1:  
           digitalWrite(P0,HIGH);
           digitalWrite(P1,LOW);
           digitalWrite(P2,HIGH);
           digitalWrite(P3,LOW);
           break;
    case 2:
           digitalWrite(P0,LOW);
           digitalWrite(P1,HIGH);
           digitalWrite(P2,HIGH);
           digitalWrite(P3,LOW);
           break;
    case 3:
           digitalWrite(P0,LOW);
           digitalWrite(P1,HIGH);
           digitalWrite(P2,LOW);
           digitalWrite(P3,HIGH);
           break;
    case 4: 
           digitalWrite(P0,HIGH);
           digitalWrite(P1,LOW);
           digitalWrite(P2,LOW);
           digitalWrite(P3,HIGH);
           break;
   }
}

void Motor::executeStep()
{
  
  if(steps == 0)
    return;
  Direction dir;
  /*
  if(ID == 4)
  {
    com.steps = -com.steps;
    setTargetData();
  }
  */
     
  switch(ID)
  {
    case 1: 
            if(com.steps>0)
              dir = CCW; 
            else if(com.steps < 0)
              dir = CW;
            break;
    case 2:
            if(steps > 0)
              dir = CCW; 
            else if(steps < 0)
              dir = CW;
            break; 
    case 30:
            if(steps < 0)
              dir = CW; 
            else if(steps > 0)
              dir = CCW;
            break; 
    case 4:
            if(steps > 0)
              dir = CCW; 
            else if(steps < 0)
              dir = CW;
            break;
    case 50:
            if(steps > 0)
              dir = CCW; 
            else if(steps < 0)
              dir = CW;
            break;
    default: 
            if(com.steps > 0)
              dir = CW; 
            else if(com.steps < 0)
              dir = CCW;
            break;
  }  
  
  if(halfActive)
    moveHalfStep(dir);
  else
    moveStep(dir);
   
  if(steps > 0)
    steps--;
  else if(steps < 0)
    steps++;
  delay(mspeed);
  
}

void Motor::setTargetData()
{
  steps = com.steps;
  halfActive = (com.halfstep>0) ? 1:0;
  mspeed = com.mspeed;
}



/////////////////////////////////////////////////////////////
//    *comsteps = msg[1];
//    
//    if(Message::Count(msg) > 4)
//    {
//      for(int i = 2; i < Message::Count(msg)-2;i++)
//        comsteps = Message::Addc(msg[i],comsteps);
//      com.steps = strtol(comsteps,0,10);
//    }     
//    else if(Message::Count(msg) == 4)
//    {
//      comsteps = Message::Addc(msg[2],comsteps);
//      com.steps = strtol(comsteps,0,10);
//    }     
//    else
//      com.steps = (byte)(msg[1]) - 48;
      
////////////////////////////////////////////////////////////
//  char* comsteps = (char*)calloc((counter-1),sizeof(char));
//  if(comsteps != NULL)
//    comsteps[counter-1] = '\0';
//  else
//  {
//    Serial.println("Calloc error");
//    return;
//  }
