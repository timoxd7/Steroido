#ifndef SETUP_LOOP_WRAPPER_H
#define SETUP_LOOP_WRAPPER_H

void setup();
void loop();

int main() {
    setup();
    
    #ifndef STEROIDO_DISABLE_LOOP
    while(true) {
        loop();
    }
    #endif
}

#endif