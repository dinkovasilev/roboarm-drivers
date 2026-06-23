
#include "Motor.h"
#include "HardByte.h"

Motor m[6]; 


char* command = NULL;


boolean DONE = false;
//boolean NewCommand = false;

void runAll();


void setup()
{
  
  Init();
  
  m[0] =  Motor(1, "BODY");
  m[1] =  Motor(2, "SHOULDER");
  m[2] =  Motor(3, "ELBOW");
  m[3] =  Motor(4, "HANDL");
  m[4] =  Motor(5, "HANDR");
  m[5] =  Motor(6, "FINGER");
  
  
  
  while(!Serial){/*wait for connection*/}
  Serial.begin(57600);
  
 }
 
 
 
void loop()
{
  delay(10);
  rcvCommand();
  command = receivedChars;
  if (newData == true) {
        
    
        
    if(!Message::Compare(command,"END"))
    { 
       
      if(Message::Count(command)>=3) 
      {     
        Motor::setCommand(command); 
        if(Motor::getComId()>=1 && Motor::getComId()<=6)  
        {
          m[Motor::getComId()-1].setTargetData(); 
        } 
        Motor::sendStatus();         
      }
      
      //Motor::sendStatus();
    }
    else
    {
      if(Message::Compare(command,"END"))
      {
        DONE = false;
        runAll();
      } 
      if(DONE)
      {
        Serial.println("DONE");
        delay(10);
      }
    }
        
        
        
        newData = false;
  }

      
  free(command);
  command = NULL;
}

/*<<<<<<<<<<<<INIT THE CONTROLLER>>>>>>>>>>*/

void Init()
{
  pinMode(P0,OUTPUT);
  pinMode(P1,OUTPUT);
  pinMode(P2,OUTPUT);
  pinMode(P3,OUTPUT);
  pinMode(RA0,OUTPUT);
  pinMode(RA1,OUTPUT);
  pinMode(RA2,OUTPUT);
  pinMode(RA3,OUTPUT);
  pinMode(RA4,OUTPUT);
  pinMode(RA5,OUTPUT);
  pinMode(RA6,OUTPUT);
  pinMode(RA7,OUTPUT);
  digitalWrite(RA0,LOW);//6
  digitalWrite(RA1,LOW);//7
  digitalWrite(RA2,LOW);//8
  digitalWrite(RA3,HIGH);//9
  digitalWrite(RA4,HIGH);//10
  digitalWrite(RA5,LOW);//11
  digitalWrite(RA7,HIGH);//13
  digitalWrite(P0,LOW);
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
  digitalWrite(P3,LOW);

}


void runAll()
{
  while(!DONE)
  {
    for(int i = 0; i < 6; i++)
    {
      if(m[i].stepsRemain() !=0)
      {
        Motor::activateMotor(m[i].getID());
        m[i].executeStep();
        DONE = false;
      }
    }
    if(m[0].stepsRemain() == 0 &&
       m[1].stepsRemain() == 0 &&
       m[2].stepsRemain() == 0 &&
       m[3].stepsRemain() == 0 &&
       m[4].stepsRemain() == 0 && 
       m[5].stepsRemain() == 0 )
       DONE = true;
  }
  
}
