#ifndef MESSAGE_H

#define MESSAGE_H


#include <Arduino.h>



class Message
{
  public:
        static int Count(const char*);
        static void Copy(const char*, char*);
        static char* Addc(char, const char*);
        static char* GetMessage();
        static uint8_t* IntToBytes(long);
        static long BytesToInt(uint8_t*);
        static boolean Compare(const char*, const char*); 
        static boolean Reading;
        static boolean NewCommand;
        
        
};

#endif
