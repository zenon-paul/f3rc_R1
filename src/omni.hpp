#ifndef INCLUDE_OMNI//グランドをさせ
#define INCLUDE_OMNI

#define RAD1 0.785398163397
#define RAD2 2.356194490192
#define RAD3 3.926990816987
#define RAD4 5.497787143782
#define PI 3.141592653589
#define D_HALF 180
#define D_RIGHTANGLE 90
#define R_HALF 3.14159265358979
#define R_RAINTANGLE 1.57079632679489
#define DEAD_ZONE 0.12
#define WEAKEN 0.5
#define SLOW 0.3
#define GOAL_MM 100

enum {TRANSLATION,ROTATION,DPAD,OMNISTOP};

class OMNI{
    private:
    public:
        int prev_mode;
        double s1;
        double s2;
        double s3;
        double s4;
        
        int d1;
        int d2;
        int d3;
        int d4;

        OMNI();
        void OMNIReset();
        void translation(double rad,double power);
        void rotation(double x,double y);
        void FRONT();
        void BACK();
        void LEFT();
        void RIGHT();
        void FRONT_RIGHT();
        void FRONT_LEFT();
        void BACK_RIGHT();
        void BACK_LEFT();


};
double get_len(double x, double y);
double get_angle(double x, double y);


#endif