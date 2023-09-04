#ifndef PARAMETER_INCLUDE
#define PARAMETER_INCLUDE

//未定
//-----math----------
#define PI 3.141592653589
//-------------------
#define RESOLUTION 2048
#define RADIUS 15.44//mm
#define RAD_PULSE (RESOLUTION/(2*PI))//rad * RAD_PULSE == pulse
#define MM_PULSE (RESOLUTION/(2.0*PI*WHEEL_RADIUS))//mm * MM_PULSE == pulse
//-------belt--------
#define BLT_KP 0.0004 //0.0004
#define BLT_KI 0.000003
#define BLT_KD 0
//-------period------------
#define dTs 0.1/*second*/
#define dTms 100/*milisec*/
#define dTus 100000
//------allowable_error-------
#define ALLOWABLEERROR 0.02//目標の1パーセントの誤差以内で許容
#define SPEESTOLERANCE 10 // n pulse/0.1s で許容
#endif