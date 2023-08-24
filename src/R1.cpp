#include<mbed.h>
#include<stdio.h>
#include<string.h>
#include"R1.hpp"
#include"parameter.hpp"
#include"arm.hpp"
#include"belt.hpp"
#include"encoder.hpp"

asm(".global _printf_float");
using ThisThread::sleep_for;
//-omni------------------------
DigitalOut mt1_dir(D12);
DigitalOut mt2_dir(D11);
DigitalOut mt4_dir(D10);
DigitalOut mt3_dir(D7);

PwmOut mt1_speed(D9);
PwmOut mt2_speed(D6);
PwmOut mt4_speed(D5);
PwmOut mt3_speed(D3);
//------------------------------

PwmOut MtBeltSpdpin();
DigitalOut MtBeltDirpin();

InterruptIn EncBeltApin();
DigitalIn EncBeltBpin();

DigitalOUt AirCylipin();


static UnbufferedSerial recv_serial(D8,D2);//tx,rx

Arm arm;
Belt blt;
Encoder enc;

Ticker pidfunc;

DigitalOut check(D4);
DigitalOut led(A3);


char buf;//
char recv_buffer[BUFFER_SIZE];//
int recv_data[6] = {0};
//---------math--------------------------------------------
double get_len(double x, double y) {//座標と原点との距離
	return pow(x * x + y * y, 0.5);
}

double get_angle(double x, double y) {//座標の偏角(rad)を求める
	double len = get_len(x, y);
	double sin = y / len;
	double cos = x / len;
	int parity_s = (sin >= 0) ? 1 : -1;
	int parity_c = (cos >= 0) ? 1 : -1;
	double d_r = (cos >= 0) ? 0 : R_HALF;
	return parity_c * asin(sin) + parity_s * d_r;
}
//-------------------------------------------------------------
void translation(double rad,double power){

    float s1,s2,s3,s4;

    s1 = power*sin(rad - RAD1);
    s2 = power*sin(rad - RAD2);
    s3 = power*sin(rad - RAD3);
    s4 = power*sin(rad - RAD4);
//-------dir更新--------------------
	mt1_dir = (s1 >= 0) ? 1 : 0;
	mt2_dir = (s2 >= 0) ? 1 : 0;
	mt3_dir = (s3 >= 0) ? 1 : 0;
	mt4_dir = (s4 >= 0) ? 1 : 0;
//-----------------------------------
    s1 = (s1<0)?-s1:s1;
    s2 = (s2<0)?-s2:s2;
    s3 = (s3<0)?-s3:s3;
    s4 = (s4<0)?-s4:s4;
//------spd更新---------------------
    mt1_speed = s1;
    mt2_speed = s2;
    mt3_speed = s3;
    mt3_speed = s4;
//----------------------------------
}

void rotation(double x,double y){
    double power = get_len(x,y);
   
    if(x>0){
        mt1_speed = power;
        mt2_speed = power;
        mt3_speed = power;
        mt4_speed = power;

        mt1_dir = 0;
	    mt2_dir = 0;
	    mt3_dir = 0;
	    mt4_dir = 0;
    }
    else{
        mt1_speed = power;
        mt2_speed = power;
        mt3_speed = power;
        mt4_speed = power;

        mt1_dir = 1;
	    mt2_dir = 1;
	    mt3_dir = 1;
	    mt4_dir = 1;
    }
}
//------------------------------------------------------------------
void received(){//interruptin
    static int prev_mode = TRANSLATION;
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
    //translation(recv_data[0]/512.0,recv_data[1]/512.0);
    //------------------omni-------------------------------------
    double power_l = get_len(recv_data[0]/512.0,recv_data[1]/512.0);
    double rad_l = get_angle(recv_data[0]/512.0,recv_data[1]/512.0);

    double power_r = get_len(recv_data[2]/512.0,recv_data[3]/512.0);
    double rad_r = get_angle(recv_data[2]/512.0,recv_data[3]/512.0);

    if((power_l<DEAD_ZONE)&&(power_r>DEAD_ZONE)){
        prev_mode = ROTATION;
        rotation(recv_data[2]/512.0,recv_data[3]/512.0);
    }
    else if((power_l>DEAD_ZONE)&&(power_r<DEAD_ZONE)){
        prev_mode = TRANSLATION;
        translation(rad_l,power_l);
    }
    else if((power_l>DEAD_ZONE)&&(power_r>DEAD_ZONE)){
        if(prev_mode == TRANSLATION){
            prev_mode = TRANSLATION;
            translation(rad_l,power_l);
        }
        else{
            prev_mode = ROTATION;
            rotation(recv_data[2]/512.0,recv_data[3]/512.0);
        }
    }
    //-----------arm---------------------------------------------
    if((recv_data[5]>>3)){
        AirCylipin = CLOSED;
    }
    else{
        AirCylipin = OPENED;
    }
    //--belt-----------------------------------------------------
    
}

void CountEncoder(){//inerrupt
//-left-----------------
    if(EncBpin.read() == 1){
        enc.Count++;
    }
    else{
        enc.Count--;
    }
}

void BeltOperate(){//ticker
    MtBeltSpdpin = blt.PID((blt.Direction>0)?enc.Count:-enc.Count);
    MtBeltDirpin = blt.Dir;
}

void init(){
    recv_serial.baud(RATE);
    recv_serial.attach(received,SerialBase::RxIrq);
    recv_serial.format(8,SerialBase::None,1);
    memset(recv_buffer,'\0',BUFFER_SIZE);

    pidfunc.attach(BeltOperate);
    EncBeltApin.rise(CountEncoder);
}

int main(){
    init();
    while(1){
        printf("(%d %d %d %d %d %d)\n",recv_data[0],recv_data[1],recv_data[2],recv_data[3],recv_data[4],recv_data[5]);
        sleep_for(10);
    }
}