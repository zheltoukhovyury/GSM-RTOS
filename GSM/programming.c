#include "ComPort.h"
#include <stdlib.h>
#include <string.h>
#include "ComPort.h"
#include "FreeRTOS.h"
#include "programming.h"
#include "tick.h"

#include "usbd_cdc_if.h"

#include "spi.h"
#include "SPI_Flash.h"

ComPortHandle* Port = NULL;


uint32_t FlashAddress_XMLSettings = 0x00000000;
uint32_t FlashAddress_XMLSettings_Len = 0x1000;


typedef enum{
  ProgrammingMaschine_State_NotInited,
  ProgrammingMaschine_State_Init,
  ProgrammingMaschine_State_Inited,
  ProgrammingMaschine_State_Idle,


}TProgrammingMaschine_State;


TProgrammingMaschine_State ProgrammingMaschine_State = ProgrammingMaschine_State_NotInited;


unsigned char ProgTestBuff[150];


void ProgrammingBegin(void)
{
  if(Port != NULL)
      ClosePort(Port);
  Port = NULL;
  ProgrammingMaschine_State = ProgrammingMaschine_State_Init;
}


void ProgrammingStop(void)
{
  if(Port != NULL)
    ClosePort(Port);
  Port = NULL;
  ProgrammingMaschine_State = ProgrammingMaschine_State_NotInited;
}


void SupportUtilLoop(void)
{
  ProgrammingBegin();
  while(1)
  {
    ProgrammingMaschine();
  }
}


void ProgrammingMaschine(void)
{
  static unsigned char* RcvBuffer = NULL;
  static int RcvP = 0;
  
  BOOL packetReceived = FALSE;
  unsigned char opcode;
  uint16_t dataInPacketLen;
  
  if(ProgrammingMaschine_State != ProgrammingMaschine_State_NotInited)
  {
    while(GetByte(Port, &RcvBuffer[RcvP]))
    {
      RcvP++;
      if(RcvBuffer[0] != 0x55)
      {
        RcvP = 0;
        continue;
      }
      if(RcvP > 2 && RcvP >= (RcvBuffer[1] + 4))
      {
        RcvP = 0;
        opcode = RcvBuffer[2];
        packetReceived = TRUE;
        dataInPacketLen = RcvBuffer[1];
        

      }
    }
  }
    
  
  
  switch(ProgrammingMaschine_State)
  {
  case ProgrammingMaschine_State_NotInited:
    break;
    //-----------------------------------
  case ProgrammingMaschine_State_Init:
      if(Port == NULL)
        Port = OpenPort(2, 115200);
      //RcvBuffer = malloc(150);
      RcvP = 0;
      RcvBuffer = ProgTestBuff;
      
      ProgrammingMaschine_State = ProgrammingMaschine_State_Inited;
      break;
  //-----------------------------------
  case ProgrammingMaschine_State_Inited:
    if(packetReceived && opcode == 1)
    {
      ProgrammingMaschine_State = ProgrammingMaschine_State_Idle;
    }
    break;
  //-----------------------------------
  case ProgrammingMaschine_State_Idle:
    if(packetReceived)
    {
      opcode = RcvBuffer[2];
      switch(opcode)
      {
      default:
      case 1:
      case 2:{
        unsigned char packet[10];
        int len = CreatePacket(opcode, 0, NULL, packet);
        int i;
        for(i = 0; i < len; i++)
          PutByte(Port, packet[i]);
      break;}
      case 3:{
        uint32_t addr;
        uint16_t len;
        addr = RcvBuffer[3];addr<<=8;
        addr |= RcvBuffer[4];addr<<=8;
        addr |= RcvBuffer[5];addr<<=8;
        addr |= RcvBuffer[6];
        
        len = RcvBuffer[1] - 4;
        
        if(addr == 0)
          SPIFlash_EraseSector(FlashAddress_XMLSettings);
        SPIFlash_WriteArray(addr, &RcvBuffer[7], len);
        
        unsigned char packet[10];
        len = CreatePacket(opcode, 0, NULL, packet);
        int i;
        for(i = 0; i < len; i++)
          PutByte(Port, packet[i]);
        
      break;}
      case 100:{
        unsigned char *packet = malloc(200);
        StartPacket(100, packet);
        AddStrToPacket("opcode 100: curren State.");
        int length = FinishPacket();
        for(int i = 0; i < length; i++)
          PutByte(Port, packet[i]);
        free(packet);
        break;}
      } 
    }
    break;
  }
  packetReceived = FALSE;
}


int CreatePacket(unsigned char Opcode, int dataLen, unsigned char* data, unsigned char* buffer)
{
  buffer[0] = 0x55;
  buffer[1] = dataLen;
  buffer[2] = Opcode;
  memcpy(&buffer[3], data, dataLen);
  buffer[3 + dataLen] = 0xCC;
  return 4 + dataLen;
}

int dataInPacketLen;
unsigned char* packetBuffer;

void StartPacket(unsigned char Opcode, unsigned char * buff)
{
  dataInPacketLen = 0;
  packetBuffer = buff;
  packetBuffer[dataInPacketLen++] = 0x55;
  packetBuffer[dataInPacketLen++] = 0x00; //Len
  packetBuffer[dataInPacketLen++] = Opcode;
}


void AddStrToPacket(unsigned char* data)
{
  AddArrayToPacket(data, strlen(data));
}


void AddArrayToPacket(unsigned char* data, int len)
{
  for(int i= 0; i < len; i++)
    packetBuffer[dataInPacketLen++] = data[i];
}

int FinishPacket()
{
  packetBuffer[1] = dataInPacketLen - 3; //Len
  packetBuffer[dataInPacketLen++] = 0xCC;
  return dataInPacketLen;
}


