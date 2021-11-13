#include "ShearInXTests.h"
#define SHEAR_FLOAT_MAX 34028234600000000000000000000000000000.0
#define SHEAR_FLOAT_MIN -1175494351000000000000000000000000000.0

void runXShearTests() {                       // Calls the tests
    struct shape* oldInputShape = inputShape;  // Make sure input shape is not modified by test cases.
    if (tstFlag) {
        struct point* testpoints = createTestpoints();  // Creates a common, constant set of pre-defined points for tests to pull from when creating their shape structs
        errorFlag = 0;
        cleanTest(testpoints);

        errorFlag = 0;
        testShapeNull();

        errorFlag = 0;
        testPointsNull();

        errorFlag = 0;
        testLargeShearValue(testpoints);

        errorFlag = 0;
        testSmallShearValue(testpoints);

        errorFlag = 0;
        testWrongMagnitude(testpoints);
    } else {
        xShear();
    }
    inputShape = oldInputShape;
}

struct point* createTestpoints() {
    struct point* testpoints = malloc(sizeof(struct point) * 10);
    // Origin point
    testpoints[0].element[0] = 0.0;
    testpoints[0].element[1] = 0.0;
    testpoints[0].element[2] = 0.0;
    testpoints[0].element[3] = 1.0;
    // Positive intergers
    testpoints[1].element[0] = 1.0;
    testpoints[1].element[1] = 2.0;
    testpoints[1].element[2] = 3.0;
    testpoints[1].element[3] = 1.0;
    // Real numbers
    testpoints[2].element[0] = 3.14159265358979323846;
    testpoints[2].element[1] = -0.1;
    testpoints[2].element[2] = -2.5;
    testpoints[2].element[3] = 1.0;
    // Large magnitudes
    testpoints[3].element[0] = 21445754606234.325452534;
    testpoints[3].element[1] = -8615416565.15435341455;
    testpoints[3].element[2] = 54145215742658418675456.0;
    testpoints[3].element[3] = 1.0;
    // Dirty 4th value
    testpoints[4].element[0] = 2.0;
    testpoints[4].element[1] = 4.0;
    testpoints[4].element[2] = 6.0;
    testpoints[4].element[3] = 2.0;

    // EXPECTED OUTPUTS FOR SHEAR Y/Z = 2:
    // Origin point
    testpoints[5].element[0] = 0.0;
    testpoints[5].element[1] = 0.0;
    testpoints[5].element[2] = 0.0;
    testpoints[5].element[3] = 1.0;
    // Positive intergers
    testpoints[6].element[0] = 1.0;
    testpoints[6].element[1] = 4.0;
    testpoints[6].element[2] = 5.0;
    testpoints[6].element[3] = 1.0;
    // Real numbers
    testpoints[7].element[0] = 3.14159265358979323846;
    testpoints[7].element[1] = 6.183186;
    testpoints[7].element[2] = 3.783185;
    testpoints[7].element[3] = 1.0;
    // Large magnitudes
    testpoints[8].element[0] = 21445754606234.325452534;
    testpoints[8].element[1] = -8615416565.15435341455;
    testpoints[8].element[2] = 54145215742658418675456.0;
    testpoints[8].element[3] = 1.0;
    // Dirty 4th value
    testpoints[9].element[0] = 1.0;
    testpoints[9].element[1] = 4.0;
    testpoints[9].element[2] = 5.0;
    testpoints[9].element[3] = 1.0;

    return testpoints;
}

// --------------TESTS---------------

//CLEAN TEST:
void cleanTest(struct point* testpoints) {
    inputShape = malloc(sizeof(struct shape));
    inputShape->points = malloc(sizeof(struct point) * 3);
    setPoint(0, &testpoints[0]);
    setPoint(1, &testpoints[1]);
    setPoint(2, &testpoints[2]);
    inputShape->numOfPoints = 3;
    setYShear(2.0);
    setZShear(2.0);
    xShear();

    if (!testHelperCompare(*getPoint(0), testpoints[5]) || !testHelperCompare(*getPoint(1), testpoints[6]) || !testHelperCompare(*getPoint(0), testpoints[5])) {
        printf("Test cleanTest failed: Incorrect output.\n");
    } else if (errorFlag == 1) {
        printf("Test cleanTest failed: An error/warning was produced.\n");
    }

    free(inputShape->points);
    free(inputShape);
}

//DIRTY TESTS:
void testShapeNull() {
    // Test gives xShear a null shape pointer
    inputShape = NULL;
    xShear();
    // Test passes if the null pointer is noticed and program cointinues without failiure.
    if (errorFlag != 1) {
        printf("Test testShapeNull failed: An error was not flagged\n");
    }
    // If the pointer was dereferenced a segfault will occur, triggering an error message
}

void testPointsNull() {
    // Test gives an existing shape with a NULL point list
    inputShape = malloc(sizeof(struct shape));
    inputShape->points = NULL;
    inputShape->numOfPoints = 1;
    setYShear(0.0);
    setZShear(0.0);
    xShear();
    // Test passes if the null pointer is noticed and program continues without failiure, but the errorFlag is set.
    if (errorFlag != 1) {
        printf("Test testpointsNull failed: An error was not flagged\n");
    }
    // If the pointer was dereferenced a segfault will occur, triggering an error message
    free(inputShape);
}

void testLargeShearValue(struct point* testpoints) {
    // Test sets y-shear value and z-shear value to maximum float-values.
    inputShape = malloc(sizeof(struct shape));
    inputShape->points = malloc(sizeof(struct point) * 3);
    setPoint(0, &testpoints[1]);
    setPoint(1, &testpoints[2]);
    setPoint(2, &testpoints[3]);
    inputShape->numOfPoints = 3;

    setYShear(SHEAR_FLOAT_MAX);
    setZShear(SHEAR_FLOAT_MAX);

    xShear();

    // Test passes if points are modified correctly, or untouched if overflow would have occurred.
    // If the test fails, the result is a garbage value.
    if (!testHelperCompare(*getPoint(0), testpoints[1]) || !testHelperCompare(*getPoint(1), testpoints[2]) || !testHelperCompare(*getPoint(2), testpoints[3])) {
        printf("Test testLargeShearValue failed: points were modified when a magnitude error should have occured instead.\n");
    } else if (errorFlag != 1) {
        printf("Test testLargeShearValue failed: An error was not flagged\n");
    }
    free(inputShape->points);
    free(inputShape);
}

void testSmallShearValue(struct point* testpoints) {
    // Test sets y-shear value and z-shear value to minimum float-values.
    inputShape = malloc(sizeof(struct shape));
    inputShape->points = malloc(sizeof(struct point) * 3);
    setPoint(0, &testpoints[1]);
    setPoint(1, &testpoints[2]);
    setPoint(2, &testpoints[3]);
    inputShape->numOfPoints = 3;

    setYShear(SHEAR_FLOAT_MIN);
    setZShear(SHEAR_FLOAT_MIN);

    xShear();

    // Test passes if points are modified correctly, or untouched if overflow would have occurred.
    // If the test fails, the result is a garbage value.
    if (!testHelperCompare(*getPoint(0), testpoints[1]) || !testHelperCompare(*getPoint(1), testpoints[2]) || !testHelperCompare(*getPoint(2), testpoints[3])) {
        printf("Test testSmallShearValue failed: points were modified when a magnitude error should have occured instead.\n");
    } else if (errorFlag != 1) {
        printf("Test testSmallShearValue failed: An error was not flagged\n");
    }
    free(inputShape->points);
    free(inputShape);
}

void testWrongMagnitude(struct point* testpoints) {
    // Test uses a vector where the 4th coord is set to an inappropriate value
    inputShape = malloc(sizeof(struct shape));
    inputShape->points = malloc(sizeof(struct point) * 2);
    setPoint(0, &testpoints[1]);
    setPoint(1, &testpoints[4]);
    inputShape->numOfPoints = 2;
    setYShear(2.0);
    setZShear(2.0);
    xShear();
    // Test passes if the points are equal (i.e: have been normalized.) When not in test mode this will print an warning.
    if (!testHelperCompare(*getPoint(0), *getPoint(1))) {
        printf("Test testWrongManitude failed: Equivalent points were not made equal.\n");
    } else if (errorFlag != 1) {
        printf("Test testWrongMagnitude failed: Passing points that are not normalized should produce a warning message\n");
    }
    free(inputShape->points);
    free(inputShape);
}

int testHelperCompare(struct point vec1, struct point vec2) {
    // Helper function used in other tests
    return (vec1.element[0] == vec2.element[0] && vec1.element[1] == vec2.element[1] && vec1.element[2] == vec2.element[2] && vec1.element[3] == vec2.element[3]);
}