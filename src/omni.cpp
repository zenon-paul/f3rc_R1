#include "omni.hpp"
#include<math.h>

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

OMNI::OMNI(){
    prev_mode = STOP;
}

void OMNI::OMNIReset(){
    d1 = 0;
	d2 = 0;
	d3 = 0;
	d4 = 0;

    s1 = 0;
    s2 = 0;
    s3 = 0;
    s4 = 0;
}

void OMNI::translation(double rad,double power){
    prev_mode = TRANSLATION;
    s1 = power*sin(rad - RAD1);
    s2 = power*sin(rad - RAD2);
    s3 = power*sin(rad - RAD3);
    s4 = power*sin(rad - RAD4);
//-------dir更新--------------------
	d1 = (s1 >= 0) ? 1 : 0;
	d2 = (s2 >= 0) ? 1 : 0;
	d3 = (s3 >= 0) ? 1 : 0;
	d4 = (s4 >= 0) ? 1 : 0;
//---------spd更新--------------------------
    s1 = (s1<0)?-s1:s1;
    s2 = (s2<0)?-s2:s2;
    s3 = (s3<0)?-s3:s3;
    s4 = (s4<0)?-s4:s4;

}

void OMNI::rotation(double x,double y){
    prev_mode = ROTATION;
    double power = get_len(x,y);
    s1 = power;
    s2 = power;
    s3 = power;
    s4 = power;

    
   
    if(x>0){
        d1 = 0;
	    d2 = 0;
	    d3 = 0;
	    d4 = 0;
    }
    else{
        d1 = 1;
	    d2 = 1;
	    d3 = 1;
	    d4 = 1;
    }
}

