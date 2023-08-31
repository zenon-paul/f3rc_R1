#ifndef ENCODER_INCLUDE
#define ENCODER_INCLUDE

#define ENC_PLUS 1
#define Enc_MINUS -1

class Encoder{
    private:
    public:
        int Count;

        int PrevCnt;

        Encoder();
        void ENCReset();
};
#endif