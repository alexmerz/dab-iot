#ifndef FWSound_h
#define FWSound_h

#include "Arduino.h"
#include "Framework.h"

#define FWSOUNDTYPE "SOUND"

class FWSound:public Framework {
    public:
        FWSound(unsigned long duration);
        String getData();
        const char* getType();
        void init();
        void check();
    private:
        int samples;
        uint32_t sum;
};

#endif
