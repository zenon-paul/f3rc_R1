#ifndef R1_INCLUDE
#define R1_INCLUDE



#define RATE 115200
#define BUFFER_SIZE 64
#define JOYMAX 512.0
#define ARMOPENBUTTON 6
#define ArMCLOSEBUTTON 7



int JudgeConvergence();
void ArmOpen();
void ArmChatch();

#endif
