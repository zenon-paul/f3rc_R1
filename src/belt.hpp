#ifndef BELT_INCLUDAE
#define BELT_INCLUDAE

#define DIR_PLUS 1
#define DIR_MINUS 0

asm(".global _printf_float");
using ThisThread::sleep_for;


class Belt{
    int GoalPulse;//目標パルス数(絶対値)//
        int Direction;//前進方向に進んだときエンコーダーを加算/後進方向に進んだときエンコーダーを加算//
        //float OutPutv;//PID速度型の出力値(最終結果PWMに突っ込む値)
        int Dir;//各モーターの回転方向
        int PrevErr;//前回の差分//
        float PrevOutPutp;//前回のPID位置型の出力値//
        float PrevOutPutv;//前回のPID速度型の出力値//
        float Acc;//累積和//

        float kp;
        float ki;
        float kd;

        int Speed;
        Belt();
        void BeltSetGein(float p,float i,float d);
        void BeltReset();
        float PID(int Current);
};
#endif