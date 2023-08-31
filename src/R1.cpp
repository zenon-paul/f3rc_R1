#include<mbed.h>
#include<stdio.h>
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

/*PwmOut MtBeltSpdpin(A3);
DigitalOut MtBeltDirpin(D12);

InterruptIn EncBeltApin(D13);
DigitalIn EncBeltBpin(PA_1);

DigitalOut AirCylipin(A5);*/


static UnbufferedSerial recv_serial(A0,A1);//tx,rx

Arm arm;
Belt blt;
Encoder enc;
OMNI R1omni;

Ticker pidfunc;

DigitalOut led(A3);


char buf;//
char recv_buffer[BUFFER_SIZE];//
int recv_data[6] = {0};

//---------math--------------------------------------------
//--------------------
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
    sscanf(recv_buffer,"%d,%d,%d,%d,%d,%d,\n",&recv_data[0],&recv_data[1],&recv_data[2],&recv_data[3],&recv_data[4],&recv_data[5]);
    memset(recv_buffer,'\0',BUFFER_SIZE);

    //------------------omni-------------------------------------
    double power_l = get_len(recv_data[0]/512.0,-recv_data[1]/512.0);
    double rad_l = get_angle(recv_data[0]/512.0,-recv_data[1]/512.0);

    double power_r = get_len(recv_data[2]/512.0,-recv_data[3]/512.0);
    double rad_r = get_angle(recv_data[2]/512.0,-recv_data[3]/512.0);

    
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
    /*if((recv_data[5]>>3)){
        AirCylipin = CLOSED;
    }
    else{
        AirCylipin = OPENED;
    }*/
    //--belt-----------------------------------------------------
    
}

void CountEncoder(){//inerrupt
    /*if(EncBeltBpin.read() == 1){
        enc.Count++;
    }
    else{
        enc.Count--;
    }*/
}

void BeltOperate(){//ticker
    //MtBeltSpdpin = blt.PID((blt.Direction>0)?enc.Count:-enc.Count);
    //MtBeltDirpin = blt.Dir;
}

void init(){
    recv_serial.baud(RATE);
    recv_serial.attach(received,SerialBase::RxIrq);
    recv_serial.format(8,SerialBase::None,1);
    memset(recv_buffer,'\0',BUFFER_SIZE);

    //pidfunc.attach_us(BeltOperate,dTus);
    //EncBeltApin.rise(CountEncoder);
}

int main(){
    init();
    while(1){
        printf("(%d %d %d %d %d %d)\n",recv_data[0],recv_data[1],recv_data[2],recv_data[3],recv_data[4],recv_data[5]);
        sleep_for(10);
    }
}