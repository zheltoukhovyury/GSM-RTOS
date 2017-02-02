extern void SupportUtilLoop(void);
int CreatePacket(unsigned char Opcode, int dataLen, unsigned char* data, unsigned char* buffer);
void StartPacket(unsigned char Opcode, unsigned char * buff);
void AddStrToPacket(unsigned char* data);
void AddArrayToPacket(unsigned char* data, int len);
int FinishPacket();
void ProgrammingMaschine(void);
