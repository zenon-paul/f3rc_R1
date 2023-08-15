#ifndef RECV_INCLUDE
#define RECV_INCLUDE
#define BUFFER_SIZE 64

class Controler{
    private:
    public:
        char buf;
        char recv_buffer[BUFFER_SIZE];
        int recv_data[6] = {0};
        Controler();
        void Controler::Receive();
};
#endif