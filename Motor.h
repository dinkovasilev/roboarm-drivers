

#ifndef MOTOR_H

#define MOTOR_H

#include "Message.h"

#define P0 2 //фаза 0
#define P1 3 //фаза 1
#define P2 4 //фаза 2
#define P3 5 //фаза 3
#define RA0 6 
#define RA1 7
#define RA2 8
#define RA3 9
#define RA4 10
#define RA5 11
#define RA6 12
#define RA7 13

typedef struct 
{
  uint8_t id = 0;
  boolean halfstep = true;
  long steps = 0;
  uint8_t mspeed = 5;
}Command;

typedef struct 
{
  uint8_t id = 0;
  uint8_t steps[sizeof(long)] = {0x00,0x00,0x00,0x00};
  uint8_t halfstep = 1;
  uint8_t mspeed = 5;
}CommandBytes;


enum Direction {CW,CCW};

class Motor
{
  public:
         Motor(); 
         Motor(int, String);

         static void setCommand(char*); 
         static void activateMotor(uint8_t);         
         static void sendStatus();
         
         void setTargetData();
         void moveStep(Direction);
         void moveHalfStep(Direction);
         void executeStep();

         void setZero();
         
         inline float getPosition(){return this->currentPosition;};
         inline unsigned char getID(){return this->ID;};
         inline String getMotorName(){return this->Name;};
         inline long stepsRemain(){return steps;};
         inline static uint8_t getComId(){return com.id;};
         
  private:
         static Command com;
         unsigned int ID;
         String Name;
         boolean halfActive;//подадена е команда за полустъпка
         boolean halfStepPosition;//мотора е на полустъпка калкулация +- 0.5 
         unsigned char pStatus;//статус на фазите
         long steps;
         uint8_t mspeed;        
         float currentPosition;//текущата позиция в стъпки отчетени от зададената 0
         float targetPosition;//целева позиция в стъпки отчетена от 0-та

          
};

#endif
