#include "TQueue.h"
#include "stdint.h"

typedef struct{
  TQueue queueHandle;
}TIniParser;


#define groupMaxLen 16
#define nameMaxLen 16
#define valueStringMaxLen 16

typedef enum 
{
  unknownType,
  unsInteger8,
  unsInteger16,
  unsInteger32,
  integer8,
  integer16,
  integer32,
  string,
  floatPointNumber,
  doubleFloatPointNumber,
  dnsAddr,
  ipAddr,
  gsmNumber,
  boolean,

}TIniParser_ParamType;


typedef union
{
  uint8_t ui8;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64;
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t port;
  char ipAddr[15];
  char dnsAddr[32];
  char string[32];
  float floatVal;
  double doubleVal;
  BOOL boolean;

}PrameterValue;

typedef struct {
  char groupName[groupMaxLen];
  char name[nameMaxLen];
  TIniParser_ParamType type;
  char valueString[valueStringMaxLen];
  PrameterValue val;
}TIniParser_Parameter;




TIniParser_ParamType StringToIniParameterType(char* string);
void IniParameterTypeToString(TIniParser_ParamType type, char* outString);


extern void IniParserInit(TIniParser* parser);
extern void IniParserDeinit(TIniParser* parser);
extern int IniParserGetFreeSpace(TIniParser* parser);
extern void IniParserPutByte(TIniParser* parser, unsigned char b);

extern TIniParser_Parameter IniParserGetNexParameter(TIniParser* parser);
extern int CreateParameter(char* name, TIniParser_ParamType type, void* value, char* groupName, char* outBuff);

