#include <string.h>
#include "XMLParser.h"


TIniParser_ParamType StringToIniParameterType(char* string)
{
  if(!strncmp(string, "ipAddr", 16))
    return ipAddr;
  else if(!strcmp(string, "dnsAddr"))
     return dnsAddr;
  else if(!strncmp(string, "gsmNumber", 16))
    return gsmNumber;
  else if(!strncmp(string, "boolean", 16))
    return boolean;
  else if(!strncmp(string, "ui16", 16))
    return unsInteger16;
  else 
    return unknownType;
}

void IniParameterTypeToString(TIniParser_ParamType type, char* outString)
{
  switch(type)
  {
  case unknownType:
    break;
  case unsInteger8:
    strcpy(outString, "ui8");
    break;
  case unsInteger16:
    strcpy(outString, "ui16");
    break;
  case unsInteger32:
    strcpy(outString, "ui32");
    break;
  case integer8:
    strcpy(outString, "i8");
    break;
  case integer16:
    strcpy(outString, "i16");
    break;
  case integer32:
    strcpy(outString, "i32");
    break;
  case string:
    strcpy(outString, "string");
    break;
  case floatPointNumber:
    strcpy(outString, "floatPointNumber");
    break;          
  case doubleFloatPointNumber:
    strcpy(outString, "doubleFloatPointNumber");
    break;
  case dnsAddr:
    strcpy(outString, "dnsAddr");
    break;
  case ipAddr:
    strcpy(outString, "ipAddr");
    break;
  case gsmNumber:
    strcpy(outString, "gsmNumber");
    break;
  case boolean:
    strcpy(outString, "boolean");
    break;
  default:
    strcpy(outString, "default");
    break;
  }

}



void IniParserInit(TIniParser* parser)
{
  QueueCreate(&parser->queueHandle);

}


void IniParserDeinit(TIniParser* parser)
{
  QueueDestroy(&parser->queueHandle);
}


int IniParserGetFreeSpace(TIniParser* parser)
{
  return QueueGetFree(&parser->queueHandle);
}

void IniParserPutByte(TIniParser* parser, unsigned char b)
{
  QueuePutByte(&parser->queueHandle, b);

}


TIniParser_Parameter IniParserGetNexParameter(TIniParser* parser)
{
  typedef enum {
    namePart,
    typePart,
    valuePart
  }TparsingState;
  
  TparsingState parsingState = namePart;
  char typeString[16];
  char nextByte;
  int letterNumber = 0;    
  TIniParser_Parameter parameter;
  
  memset(&parameter, 0x00, sizeof(parameter));
  memset(&typeString, 0x00, sizeof(typeString));

  while(1)
  {
    if(!QueueGetByte(&parser->queueHandle, &nextByte))
    {
      parameter.type = unknownType;
      return parameter;
    }
    switch(parsingState)
    {
    case namePart:
      if((int)nextByte == (int)':')
      {
        parsingState = typePart;
        letterNumber = 0;
        break;
      }
      else if((int)nextByte == (int)'.')
      {
        strncpy(parameter.groupName, parameter.name, groupMaxLen);
        memset(&parameter.name, 0x00, sizeof(parameter.name));
        letterNumber = 0;
      }
      else if (letterNumber < nameMaxLen)
        parameter.name[letterNumber++] = nextByte;
      break;
    case typePart:
      if((int)nextByte == (int)'"')
      {
        parsingState = valuePart;
        letterNumber = 0;
        break;
      }
      else if(letterNumber < 16)
        typeString[letterNumber++] = nextByte;
      break;
    case valuePart:
      if((int)nextByte == (int)'\"')
        nextByte = nextByte;
      else if((int)nextByte == (int)'\r')
      {
        QueueGetByte(&parser->queueHandle, &nextByte);
        if((int)nextByte == (int)'\n')
        {
          parameter.type = StringToIniParameterType(typeString);
          switch(parameter.type)
          {
          case unknownType:
            break;
          case unsInteger8:
            parameter.val.ui8 = (uint8_t)atoi(parameter.valueString);
            break;
          case unsInteger16:
            parameter.val.ui16 = (uint16_t)atoi(parameter.valueString);
            break;
          case unsInteger32:
            parameter.val.ui16 = (uint32_t)atoi(parameter.valueString);
            break;
          case integer8:
            parameter.val.i8 = (int8_t)atoi(parameter.valueString);
            break;
          case integer16:
            parameter.val.i16 = (int16_t)atoi(parameter.valueString);
            break;
          case integer32:
            parameter.val.i16 = (int32_t)atoi(parameter.valueString);
            break;
          case string:
            strncpy(parameter.val.string, parameter.valueString, 32);
            break;
          case floatPointNumber:
          case doubleFloatPointNumber:
            break;
          case dnsAddr:
            strncpy(parameter.val.string, parameter.valueString, 32);
            break;
          case ipAddr:
            strncpy(parameter.val.string, parameter.valueString, 15);
            break;
          case gsmNumber:
            strncpy(parameter.val.string, parameter.valueString, 10);
            break;
          case boolean:
            if(strncmp(parameter.valueString, "True", 4))
              parameter.val.boolean = TRUE;
            else if(strncmp(parameter.valueString, "False", 5))
              parameter.val.boolean = FALSE;
            break;
          default:
            
            break;
          }
          return parameter;
        }
        break;
      }
      else if (letterNumber < valueStringMaxLen )
        parameter.valueString[letterNumber++] = nextByte;
      break;
    }
  }
}




int CreateParameter(char* name, TIniParser_ParamType type, void* value, char* groupName, char* outBuff)
{
  char typeString[16];
  char valueString[16];
  
  IniParameterTypeToString(type, typeString);
  sprintf(outBuff, "%s.%s:\"%s\"\r\n", groupName, name, typeString, valueString);
  return strlen(outBuff);
}
