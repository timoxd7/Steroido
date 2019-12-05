#include <unity.h>
#include "AbstractionLayer/Arduino/NonCopyable.h"
#define STEROIDO_DISABLE_LOOP
#include "Common/setupLoopWrapper.h"
#include "Common/ITimer.h"

#if defined(USE_ARDUINO) || defined(USE_TEENSY)
    #include "Arduino.h"
#endif

#ifdef USE_MBED
    #include "mbed.h"
#endif

unsigned long _millis = 0;

class Timer : public ITimer {
    private:
        virtual unsigned long getMillis() {
            return _millis;
        }
};

#include "Common/DelayedSwitch.h"


void testDelayedSwitch() {
    DelayedSwitch delayedSwitch;

    // Test without timeings
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch, "T1");
    delayedSwitch = true;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch, "T2");
    delayedSwitch.set(false);
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.get(), "T3");

    delayedSwitch.reset();
    delayedSwitch.setEnableTime(150);
    delayedSwitch.setDisableTime(400);
    _millis = 100;

    // Test Enable 1
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(true), "T4");
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(true), "T5");
    _millis += 100;
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(true), "T6");
    _millis += 51;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(true), "T7");

    // Test Enable 2
    delayedSwitch.reset();
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(true), "T8");
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(true), "T9");
    _millis += 100;
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(false), "T10");
    _millis += 51;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(true), "T11");

    // Test Disable
    _millis += 1000;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(true), "T12");
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(false), "T13");
    _millis += 300;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(false), "T14");
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(true), "T15");
    _millis += 50;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(true), "T16");
    _millis += 51;
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(false), "T17");

    // Test Enable Again
    _millis += 1;
    TEST_ASSERT_FALSE_MESSAGE(delayedSwitch.set(true), "T18");
    _millis += 151;
    TEST_ASSERT_TRUE_MESSAGE(delayedSwitch.set(true), "T19");
}


void setup() {
    UNITY_BEGIN();
    RUN_TEST(testDelayedSwitch);
    UNITY_END();
}

void loop() {}