#ifndef PARAMETER_INCLUDE
#define PARAMETER_INCLUDE

//未定
#define RADIUS 1
#define MM_PULSE 1
#define RAD_PULSE 1
//-------belt--------
#define BLT_KP 0.0003 //0.0004
#define BLT_KI 0.000003
#define BLT_KD 0
//-------period------------
#define dTs 0.1/*second*/
#define dTms 100/*milisec*/
#define dTus 100000
//------allowable_error-------
#define ALLOWABLEERROR 0.01//目標の1パーセントの誤差以内で許容
#define SPEESTOLERANCE 10 // n pulse/0.1s で許容
#endif