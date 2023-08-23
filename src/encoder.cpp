#include"encoder.hpp"
#include<mbed.h>

Encoder::Encoder(){
}
void Encoder::ENCReset(){
    Count = 0;
    PrevCnt = 0;
}

