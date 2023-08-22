#include"belt.hpp"
#include"parameter.hpp"
Belt::Belt(){
}
void Belt::BeltSetGein(float p,float i,float d){
    kp = p;
    ki = i;
    kd = d;
}
float Belt::PID(int Current){
//------位置柄---------------------------------
    int err = GoalPulse - Current;
    Acc += (float)err*dTs;
    double errdif = (float)(err - PrevErr)/dTs;
    PrevErr = err;

    if(err>0){//方向決め
        Dir = (Direction>0)?DIR_PLUS:DIR_MINUS;//ピンに出力
    }
    else{
        Dir = (Direction>0)?DIR_MINUS:DIR_PLUS;//ピンに出力
    }
//-------速度型----------------------------------
    float outputp = kp*(float)err + ki*Acc + kd*errdif;

    float outdif = outputp - PrevOutPutp;
    float outputv = outdif + PrevOutPutv;

    PrevOutPutp = outputp;
    PrevOutPutv = outputv;

    return outputv;
}
