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

DigitalOut Mt1Dirpin();
DigitalOut Mt2Dirpin();
DigitalOut Mt3Dirpin();
DigitalOut Mt4Dirpin();
DigitalOUt MtBeltDirpin();

PwmOut Mt1Spdpin();
PwmOut Mt2Spdpin();
PwmOut Mt3Spdpin();
PwmOut Mt4Spdpin();
PwmOut MtBeltSpdpin();

InterruptIn EncBeltApin();
DigitalIn EncBeltBpin();

DigitalOUt AirCylipin();

static UnbufferedSerial recv_serial(D8,D2);//tx,rx

t_recv Eps;
Arm arm;
Belt blt;
Encoder enc;

Ticker pidfunc;

DigitalOut check(D4);
DigitalOut led(A3);


int cnt = 0;
char buf;//
char str[BUFFER_SIZE];
char recv_buffer[BUFFER_SIZE];//
int recv_data[6] = {0};

void checker(){
    for(int i = 0;i<recv_data[0];i++){
        led = 1;
        sleep_for(50);
        led = 0;
        sleep_for(50);
    }
}

void received(){
    check = check?0:1;
    cnt++;
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
    //recv_buffer[i+1] = '\0';
    sscanf(recv_buffer,"%d,%d,%d,%d,%d,%d,\n",&recv_data[0],&recv_data[1],&recv_data[2],&recv_data[3],&recv_data[4],&recv_data[5]);
    memset(recv_buffer,'\0',BUFFER_SIZE);
}
void init(){
    recv_serial.baud(RATE);
    recv_serial.attach(received,SerialBase::RxIrq);
    recv_serial.format(8,SerialBase::None,1);
    memset(str,'\0',BUFFER_SIZE);
}
int main(){
    init();
    while(1){
        printf("hello\n");
        printf("%d\n",cnt);
        //printf("%d\n",recv_data[0]);
        printf("(%d %d %d %d %d %d)\n",recv_data[0],recv_data[1],recv_data[2],recv_data[3],recv_data[4],recv_data[5]);
        sleep_for(100);
        /*led = 1;
        sleep_for(500);
        led = 0;
        sleep_for(500);*/
    }
}