#include "unity.h"
#include "Common/NonCopyable.h"
#include "Common/memCpy.h"
#include "Common/memSet.h"

#include "Common/vector.h"
using namespace std;

#define STEROIDO_DISABLE_LOOP

#if defined(USE_ARDUINO) || defined(USE_TEENSY)
    #include "Arduino.h"
#else
    #include "Common/setupLoopWrapper.h"
#endif

#ifdef USE_MBED
    #include "mbed.h"
#endif

#ifdef USE_NATIVE
    #include <stdio.h>
#endif


#define ELEMENT_RESERVE_COUNT 64
#define ELEMENT_ADD_COUNT 256
#define ELEMENT_ERASE_NUMBER 100
#define ELEMENT_AFTER_ADD_COUNT 32
#define ELEMENT_DELETE_ALL_FROM_ID 64


uint16_t runningId = 0;

class testObject {
    public:
        testObject() {
            thisId = runningId++;
            printf("Construct: %i\n", thisId);
        }

        testObject(const testObject &that) {
            thisId = that.thisId;
            printf("Copy: %i\n", thisId);
        }

        operator=(const testObject &that) {
            thisId = that.thisId;
            printf("Copy Assign: %i\n", thisId);
        }

        ~testObject() {
            printf("Destroyed: %i\n", thisId);
        }

        uint16_t getId() {
            return thisId;
        }

        uint16_t thisId;
};


vector<testObject> testVector;


void vectorTest() {
    // Pre reserve
    testVector.reserve(ELEMENT_RESERVE_COUNT);
    TEST_ASSERT_EQUAL_MESSAGE(ELEMENT_RESERVE_COUNT, testVector.capacity(), "T1");

    printf("Fill up\n");
    for (uint16_t i = 0; i < ELEMENT_ADD_COUNT; i++) {
        testObject currentObj;
        testVector.push_back(currentObj);
    }

    printf("Read back out\n");
    for (uint16_t i = 0; i < ELEMENT_ADD_COUNT; i++) {
        testObject &currentObj = testVector.at(i);
        TEST_ASSERT_TRUE_MESSAGE(currentObj.getId() == i, "T2");
    }

    printf("Read out with Iterator\n");
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            TEST_ASSERT_TRUE_MESSAGE(it->getId() == i++, "T3");
        }
    }

    printf("Delete by ID\n");
    for (auto it = testVector.begin(); it != testVector.end();) {
        if (it->getId() == ELEMENT_ERASE_NUMBER) {
            it = testVector.erase(it);
        } else {
            it++;
        }
    }

    printf("Check deleted\n");
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            if (i == ELEMENT_ERASE_NUMBER) i++;

            TEST_ASSERT_EQUAL_MESSAGE(i, it->thisId, "T4");

            i++;
        }
    }
    
    printf("After add\n");
    for (uint16_t i = 0; i < ELEMENT_AFTER_ADD_COUNT; i++) {
        testObject currentObject;
        testVector.push_back(currentObject);
    }

    printf("Read back out again\n");
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            if (i == ELEMENT_ERASE_NUMBER) i++;

            TEST_ASSERT_EQUAL_MESSAGE(i, it->getId(), "T5");

            i++;
        }
    }

    printf("Delete all after delete ID\n");
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end();) {
            if (i++ >= ELEMENT_DELETE_ALL_FROM_ID) {
                it = testVector.erase(it);
            } else {
                it++;
            }
        }
    }

    printf("Shrink to Fit\n");
    testVector.shrink_to_fit();

    printf("Check shrinked Vector\n");
    TEST_ASSERT_EQUAL_MESSAGE(ELEMENT_DELETE_ALL_FROM_ID, testVector.size(), "T6");
    TEST_ASSERT_EQUAL_MESSAGE(ELEMENT_DELETE_ALL_FROM_ID, testVector.capacity(), "T7");
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            TEST_ASSERT_EQUAL_MESSAGE(i++, it->getId(), "T8");
        }
    }
}


void setup() {
    UNITY_BEGIN();
    RUN_TEST(vectorTest);
    UNITY_END();
}

void loop() {}