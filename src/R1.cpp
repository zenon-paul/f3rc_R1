#include<mbed.h>
#include<stdio.h>//上昇でマイナス方向下降でプラス
#include<string.h>
#include"R1.hpp"
#include"parameter.hpp"
#include"arm.hpp"
#include"belt.hpp"
#include"encoder.hpp"
#include"omni.hpp"

asm(".global _printf_float");
using ThisThread::sleep_for;

//-omni------------------------
DigitalOut mt1_dir(D12);
DigitalOut mt2_dir(D11);
DigitalOut mt3_dir(D7);
DigitalOut mt4_dir(D10);

PwmOut mt1_speed(D9);
PwmOut mt2_speed(D6);
PwmOut mt3_speed(D3);
PwmOut mt4_speed(D5);
//------------------------------

PwmOut MtBeltSpdpin(A1);
DigitalOut MtBeltDirpin(D13);

InterruptIn EncBeltApin(D14);
DigitalIn EncBeltBpin(D15);

DigitalOut AirCylipin(A5);


static UnbufferedSerial recv_serial(D8,D2);//tx,rx

Arm arm;
Belt blt;
Encoder enc;
OMNI R1omni;

Ticker pidfunc;

DigitalOut led(A3);

double RAD;
char buf;//
char recv_buffer[BUFFER_SIZE];//
int recv_data[6] = {0};
double inp = 0;
int fff = 0;

//--------operate-----------------------------------
void BeltOperate(){//ticker
    if(blt.Mode == PIDCONTROL){
        inp = blt.PID(enc.Count);
        MtBeltSpdpin = inp;
        MtBeltDirpin = blt.Dir;
        if(JudgeConvergence() == 0){
            blt.BeltReset();
            fff = 1;
        }
    }
    else if(blt.Mode == BTSTOP){
        MtBeltSpdpin = 0;
    }
    int spd = enc.Count - enc.PrevCnt;
    blt.Speed = (spd>=0)?spd:-spd;

    enc.PrevCnt = enc.Count;
}
void CountEncoder(){//inerruptin
    if(EncBeltBpin.read() == 1){
        enc.Count++;
    }
    else{
        enc.Count--;
    }
}

//--------------------------------------------------
void received(){//interruptin
    led = !led;
    int i = 0;
    while(1){
        recv_serial.read(&buf,1);
        if((i>=BUFFER_SIZE)||(buf == '\n')){
            break;
        }
        recv_buffer[i] = buf;
        i++;
    }
    recv_buffer[i] = '\n';
    sscanf(recv_buffer,"%d,%d,%d,%d,%d,%d\n",&recv_data[0],&recv_data[1],&recv_data[2],&recv_data[3],&recv_data[4],&recv_data[5]);
    memset(recv_buffer,'\0',BUFFER_SIZE);
    double power_l = get_len(recv_data[0]/512.0,recv_data[1]/512.0);
    double rad_l = get_angle(recv_data[0]/512.0,recv_data[1]/512.0);

    double power_r = get_len(recv_data[2]/512.0,recv_data[3]/512.0);
    double rad_r = get_angle(recv_data[2]/512.0,recv_data[3]/512.0);


    if(recv_data[4]){
        R1omni.prev_mode = DPAD;
        switch(recv_data[4]){
            case 1:
                R1omni.FRONT();
                break;
            case 2:
                R1omni.BACK();
                break;
            case 4:
                R1omni.RIGHT();
                break;
            case 5:
                R1omni.FRONT_RIGHT();
                break;
            case 6:
                R1omni.BACK_RIGHT();
                break;
            case 8:
                R1omni.LEFT();
                break;
            case 9:
                R1omni.FRONT_LEFT();
                break;
            case 10:
                R1omni.BACK_LEFT();
                break;
            default:
                break;
        }
    }
    else{
        if((power_l<DEAD_ZONE)&&(power_r>DEAD_ZONE)){
        R1omni.rotation(recv_data[2]/512.0,recv_data[3]/512.0);
    }
    else if((power_l>DEAD_ZONE)&&(power_r<DEAD_ZONE)){
        R1omni.translation(rad_l,power_l);
    }
    else if((power_l>DEAD_ZONE)&&(power_r>DEAD_ZONE)){
        if(R1omni.prev_mode == TRANSLATION){
            R1omni.translation(rad_l,power_l);
        }
        else{
            R1omni.rotation(recv_data[2]/512.0,recv_data[3]/512.0);
        }
    }
    else{
        R1omni.OMNIReset();
    }
    }

//-----------ピン入力--------------------------
    mt1_speed = R1omni.s1;
    mt2_speed = R1omni.s2;
    mt3_speed = R1omni.s3;
    mt4_speed = R1omni.s4;

    mt1_dir = R1omni.d1;
    mt2_dir = R1omni.d2;
    mt3_dir = R1omni.d3;
    mt4_dir = R1omni.d4;
    //-----------arm---------------------------------------------
    if((recv_data[5]&(1<<ARMOPENBUTTON))){
        ArmChatch();
    }
    if((recv_data[5]&(1<<ArMCLOSEBUTTON))){
        ArmOpen();
    }
    //--belt-----------------------------------------------------
    
}

int JudgeConvergence(){//1-収束しない　0-収束
    if((blt.Speed <= SPEESTOLERANCE)&&(blt.PrevErr<ALLOWABLEERROR*((blt.GoalPulse>=0)?blt.GoalPulse:-blt.GoalPulse))){
        return 0;
    }
    return 1;
}

void ArmOpen(){
    if(blt.Mode == PIDCONTROL){
        return;
    }
    blt.Mode = PIDCONTROL;
    AirCylipin = 0;
    blt.GoalPulse = 10;
}

void ArmChatch(){
    if(blt.Mode == PIDCONTROL){
        return;
    }
    blt.Mode = PIDCONTROL;
    AirCylipin = 1;
    blt.GoalPulse = 1000;
}

void init(){
    //recv_serial.baud(RATE);
    //recv_serial.format(8,SerialBase::None,1);
    //recv_serial.attach(received,SerialBase::RxIrq);
    memset(recv_buffer,'\0',BUFFER_SIZE);

    EncBeltApin.rise(CountEncoder);
    pidfunc.attach_us(BeltOperate,dTus);
    blt.BeltSetGein(BLT_KP,BLT_KI,BLT_KD);
}

int main(){
    init();

    //mt1_speed = 0.5;
    blt.Mode = PIDCONTROL;
    AirCylipin = 0;
    blt.GoalPulse = -400;

    while(1){
        printf("%d f%d gp%d cn%d dr%d per%d inp%d s%d\n",fff,(int)(100.0*blt.PrevErr/blt.GoalPulse),blt.GoalPulse,enc.Count,blt.Dir,blt.PrevErr,(int)(100.0*inp),blt.Speed);
        //printf("%d(%d %d %d %d %d %d)\n",R1omni.prev_mode,recv_data[0],recv_data[1],recv_data[2],recv_data[3],recv_data[4],recv_data[5]);
        //printf("d(%d %d %d %d)s(%d %d %d %d)\n",R1omni.d1,R1omni.d2,R1omni.d3,R1omni.d4,(int)(100.0*R1omni.s1),(int)(100.0*R1omni.s2),(int)(100.0*R1omni.s3),(int)(100.0*R1omni.s4));
        /*if(R1omni.prev_mode == TRANSLATION){
            recv_serial.write("a",1);
        }
        else if(R1omni.prev_mode == ROTATION){
            recv_serial.write("b",1);
        }
        else if(R1omni.prev_mode == DPAD){
            recv_serial.write("c",1);
        }
        else if(R1omni.prev_mode == OMNISTOP){
            recv_serial.write("d",1);
        }*/
        sleep_for(10);
    }
}