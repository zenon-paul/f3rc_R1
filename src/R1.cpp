#include"R1.hpp"
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

InterruptIn EncApin();
DigitalOut EncBpin();

DigitalOUt AirCylipin();
R1::R1(){
    
}
void R1::Translation(){

}
void R1::Rotation(){

}
void R1::RecvVal(){//割込みで呼ばれる

}