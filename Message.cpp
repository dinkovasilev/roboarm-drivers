#include "Message.h"


boolean Message::Reading = false;
boolean Message::NewCommand = false;

int Message::Count(const char* str)
{   
  int counter = 0;
  while(str[counter]!='\0')
    counter++;
  return counter;
}

void Message::Copy(const char* from, char* to)
{
  
  for(int i=0; i< Count(from); i++ )
    to[i] = from[i];
}

char* Message::Addc(char c, const char* to)
{   
  int len = Count(to);
  
  char* result = (char*)malloc((len+1)*sizeof(char));
  
  Copy(to,result); 
  result[len] = c;
  result[len+1] = '\0';  
  return result;
}

char* Message::GetMessage()
{
  char* buff = NULL;
  char start = '<';
  char final = '>';
  char c;
  while(Serial.available() > 0 )
  {
    c = Serial.read();
    if(c == start)
      Reading = true; 
    if(c == final)
    {
       Reading = false;
       NewCommand = true; 
       return buff;
    }  
    if(Reading)    
      buff = Addc(c,buff);
        
  }
 Reading = false; 
 return buff;
}

uint8_t* Message::IntToBytes(long iNumber)
{
    uint8_t*  Ibytes = (uint8_t*)malloc(sizeof(long)*sizeof(uint8_t));
    
    for(int i=0; i < sizeof(long);i++)
        Ibytes[i] = (iNumber >> i*8) & 0xFF;
        
    return Ibytes;
}

long Message::BytesToInt(uint8_t* Ibytes)
{
    long value = 0;
    
    for(int i = sizeof(long); i >=0 ; i--)
        value +=  Ibytes[i] << i*8; 
        
    return value;
}

boolean Message::Compare(const char* s1, const char* s2)
{
  if(Count(s1) != Count(s2))
    return false;
  for(int i = 0; i < Count(s1); i++)
  {
    if(s1[i] != s2[i])
      return false;
  }
  
  return true;
}
