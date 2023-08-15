#include<mbed.h>
#include"recv_controler_val.hpp"
static UnbufferedSerial recv_serial(D8,D2);//tx,rx
Controler::Controler(){

}
void Controler::Receive(){
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