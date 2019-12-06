#include "Common/TestingHeader.h"

#ifdef USE_NATIVE
    #include <stdio.h>
#endif

#ifndef USE_MBED
    #include "Common/Callback.h"
#endif

#if defined(USE_MBED) || defined(USE_NATIVE) || defined(TEENSY)
    #include <vector>
#else
    #include "Common/vector.h"
#endif

// OS
#include "OS/ICallable.h"
#include "OS/ScheduledCallable.h"
#include "OS/Scheduler.h"

Scheduler scheduler;

#include "OS/Ticker.h"

uint16_t classCallCounter = 0;
uint16_t functionCallCounter = 0;

class TestClass {
    public:
        void callMe() {
            classCallCounter++;
            printf("Class call\n");
        }
};

void callMeFunc() {
    functionCallCounter++;
    printf("Function call\n");
}

void tickerTest() {
    Ticker testTicker;

    testTicker.attach(callback(callMeFunc), 1.0);

    scheduler.run();

    TEST_ASSERT_EQUAL_MESSAGE(0, functionCallCounter, "T1");

    _millis = 1100;
    scheduler.run();

    TEST_ASSERT_EQUAL_MESSAGE(1, functionCallCounter, "T2");

    testTicker.detach();

    _millis += 1100;
    scheduler.run();

    TEST_ASSERT_EQUAL_MESSAGE(1, functionCallCounter, "T3");

    _millis += 1100;
    TestClass testClass;
    testTicker.attach(callback(testClass, &TestClass::callMe), 2.0);

    scheduler.run();

    TEST_ASSERT_EQUAL_MESSAGE(1, functionCallCounter, "T4");
    TEST_ASSERT_EQUAL_MESSAGE(0, classCallCounter, "T5");

    _millis += 1100;
    scheduler.run();

    TEST_ASSERT_EQUAL_MESSAGE(0, classCallCounter, "T6");

    _millis += 1100;
    scheduler.run();

    TEST_ASSERT_EQUAL_MESSAGE(1, classCallCounter, "T7");
}


void setup() {
    UNITY_BEGIN();
    RUN_TEST(tickerTest);
    UNITY_END();
}