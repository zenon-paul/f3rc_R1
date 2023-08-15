#ifndef R1_INCLUDE
#define R1_INCLUDE
#include<mbed.h>
#include"air_cylinder.hpp"
#include"belt.hpp"
#include"recv_controler_val.hpp"

#define TRANSLATION 1
#define ROTATION 0
#define STOP -1

class R1{
    private:
    public:
        AirCyli Arm;
        Belt Blt;
        Controler Ps4;
        int Motion;

        int StickL[2];
        int StickR[2];
        int 
        R1();
        void Translation();
        void Rotation();
        void RecvVal();

};
#endif
