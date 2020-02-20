#ifdef STEROIDO_UNIT_TEST_ENABLED

#include "Common/TestingHeader.h"

#ifdef USE_NATIVE
    #include <stdio.h>
#endif


#include "Common/vector.h"
using namespace std;


#define ELEMENT_RESERVE_COUNT 64
#define ELEMENT_ADD_COUNT 256
#define ELEMENT_ERASE_NUMBER 100
#define ELEMENT_AFTER_ADD_COUNT 32
#define ELEMENT_DELETE_ALL_FROM_ID 64

//#define DOC_PRINTF


uint16_t runningId = 0;
uint16_t copyCount = 0;

class testObject {
    public:
        testObject() {
            thisId = runningId++;
            #ifdef DOC_PRINTF
            printf("Construct: %i\n", thisId);
            #endif
        }

        testObject(const testObject &that) {
            thisId = that.thisId;
            #ifdef DOC_PRINTF
            printf("Copy: %i\n", thisId);
            #endif
            copyCount++;
        }

        testObject& operator=(const testObject &that) {
            thisId = that.thisId;
            #ifdef DOC_PRINTF
            printf("Copy Assign: %i\n", thisId);
            #endif
            copyCount++;
            return *this;
        }

        ~testObject() {
            #ifdef DOC_PRINTF
            printf("Destroyed: %i\n", thisId);
            #endif
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

    #ifdef DOC_PRINTF
    printf("Fill up\n");
    #endif
    for (uint16_t i = 0; i < ELEMENT_ADD_COUNT; i++) {
        testObject currentObj;
        testVector.push_back(currentObj);
    }

    #ifdef DOC_PRINTF
    printf("Read back out\n");
    #endif
    for (uint16_t i = 0; i < ELEMENT_ADD_COUNT; i++) {
        testObject &currentObj = testVector.at(i);
        TEST_ASSERT_TRUE_MESSAGE(currentObj.getId() == i, "T2");
    }

    #ifdef DOC_PRINTF
    printf("Read out with Iterator\n");
    #endif
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            TEST_ASSERT_TRUE_MESSAGE(it->getId() == i++, "T3");
        }
    }

    #ifdef DOC_PRINTF
    printf("Delete by ID\n");
    #endif
    for (auto it = testVector.begin(); it != testVector.end();) {
        if (it->getId() == ELEMENT_ERASE_NUMBER) {
            it = testVector.erase(it);
        } else {
            it++;
        }
    }

    #ifdef DOC_PRINTF
    printf("Check deleted\n");
    #endif
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            if (i == ELEMENT_ERASE_NUMBER) i++;

            TEST_ASSERT_EQUAL_MESSAGE(i, it->thisId, "T4");

            i++;
        }
    }

    #ifdef DOC_PRINTF
    printf("After add\n");
    #endif
    for (uint16_t i = 0; i < ELEMENT_AFTER_ADD_COUNT; i++) {
        testObject currentObject;
        testVector.push_back(currentObject);
    }

    #ifdef DOC_PRINTF
    printf("Read back out again\n");
    #endif
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            if (i == ELEMENT_ERASE_NUMBER) i++;

            TEST_ASSERT_EQUAL_MESSAGE(i, it->getId(), "T5");

            i++;
        }
    }

    #ifdef DOC_PRINTF
    printf("Delete all after delete ID\n");
    #endif
    for (auto it = testVector.end() - 1; it != testVector.begin();) {
        if (it->getId() >= ELEMENT_DELETE_ALL_FROM_ID) {
            it = testVector.erase(it);
            it--;
        } else {
            it--;
        }
    }

    #ifdef DOC_PRINTF
    printf("Shrink to Fit\n");
    #endif
    testVector.shrink_to_fit();

    #ifdef DOC_PRINTF
    printf("Check shrinked Vector\n");
    #endif
    TEST_ASSERT_EQUAL_MESSAGE(ELEMENT_DELETE_ALL_FROM_ID, testVector.size(), "T6");
    TEST_ASSERT_EQUAL_MESSAGE(ELEMENT_DELETE_ALL_FROM_ID, testVector.capacity(), "T7");
    {
        uint16_t i = 0;
        for (auto it = testVector.begin(); it != testVector.end(); it++) {
            TEST_ASSERT_EQUAL_MESSAGE(i++, it->getId(), "T8");
        }
    }

    #ifdef DOC_PRINTF
    printf("A total of %i elements got copied\n", copyCount);
    #endif
}


void setup() {
    UNITY_BEGIN();
    RUN_TEST(vectorTest);
    UNITY_END();
}

#endif // STEROIDO_UNIT_TEST_ENABLED