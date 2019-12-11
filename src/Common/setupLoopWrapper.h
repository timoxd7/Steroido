#ifndef SETUP_LOOP_WRAPPER_H
#define SETUP_LOOP_WRAPPER_H

void setup();

#ifndef STEROIDO_DISABLE_LOOP
    void loop();
#endif

int main() {
    setup();
    
    #ifndef STEROIDO_DISABLE_LOOP
    while(true) {
        loop();
    }
    #endif

    return 0;
}

#endif