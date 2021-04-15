#ifndef SETUP_LOOP_WRAPPER_H
#define SETUP_LOOP_WRAPPER_H

/**
 * @brief The Setup will be called once after startup
 * 
 */
void setup();

#ifndef STEROIDO_DISABLE_LOOP
    /**
     * @brief The Loop will be called repeatedly after executing the setup
     * 
     */
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