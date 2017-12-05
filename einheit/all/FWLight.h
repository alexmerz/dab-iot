#ifndef FWLight_h
#define FWLight_h

#include "Arduino.h"
#include "Framework.h"

#define FWLIGHTTYPE "LIGHT"

class FWLight:public Framework {
    public:
        FWLight(unsigned long duration);
        String getData();
        const char* getType();
        void init();
        void check();
    private:
        int samples;
        uint32_t sum;
};

#endif
