#include"belt.hpp"
#include"parameter.hpp"
Belt::Belt(){
    GoalPulse = 0;
    //Direction = 0;
    PrevErr = 0;
    PrevOutPutp = 0;
    PrevOutPutv = 0;
    Acc = 0;
    Dir = DIR_PLUS;
    Speed = 0;
    Mode = BTSTOP;
}
void Belt::BeltSetGein(float p,float i,float d){
    kp = p;
    ki = i;
    kd = d;
}
void Belt::BeltReset(){
    //GoalPulse = 0;
    //Direction = 0;
    PrevErr = 0;
    PrevOutPutp = 0;
    PrevOutPutv = 0;
    Acc = 0;
    Dir = DIR_PLUS;
    Speed = 0;
    Mode = BTSTOP;
}
double Belt::PID(int Current){
//------位置型---------------------------------
    int errparity = GoalPulse - Current;
    int err = (errparity>=0)?errparity:-errparity;
    Acc += (float)err*dTs;
    double errdif = (float)(err - PrevErr)/dTs;
    PrevErr = err;

    if(err>0){//方向決め
        Dir = (errparity>=0)?DIR_MINUS:DIR_PLUS;//ピンに出力
    }
    else{
        Dir = (errparity>=0)?DIR_PLUS:DIR_MINUS;//ピンに出力
    }
//-------速度型----------------------------------
    float outputp = kp*(float)err + ki*Acc + kd*errdif;

    float outdif = outputp - PrevOutPutp;
    float outputv = outdif + PrevOutPutv;

    PrevOutPutp = outputp;
    PrevOutPutv = outputv;

    return outputv;
}
