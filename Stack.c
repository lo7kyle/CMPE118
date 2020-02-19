#include "Stack.h"
#include "stdio.h"
#include "string.h"
#include "BOARD.h"
#include <stdbool.h>
int StackInit(struct Stack *stack)
{
    int i;
    stack->currentItemIndex = -1;

    for (i = 0; i < STACK_SIZE; i++) {
        stack->stackItems[i] = 0;
    }
    stack->initialized = true;
    return SUCCESS;
}

int StackPush(struct Stack *stack, uint8_t value)
{
    if (stack->initialized == false || StackIsFull(stack)) { 
        return ERROR;
    }
    stack->stackItems[++(stack->currentItemIndex)] = value;
    return SUCCESS;
}

int StackPop(struct Stack *stack, uint8_t *value)
{
    if (stack->initialized == false || StackIsEmpty(stack)) {
        return ERROR;
    } 
    *value = stack->stackItems[stack->currentItemIndex--];
    return SUCCESS;
}

int StackIsEmpty(const struct Stack *stack)
{
    return stack->currentItemIndex == -1 && stack->initialized == true;
}

int StackIsFull(const struct Stack *stack)
{
    return (stack->initialized == true) && (stack->currentItemIndex >= STACK_SIZE - 1); 
}
int StackGetSize(const struct Stack *stack)
{
    if (!stack->initialized) {
        return ERROR;
    }
    return stack->currentItemIndex + 1;
}

int StackToString(const struct Stack *stack, char *result, int resultSize)
{
    char temp[1000] = "\0";
    unsigned int i, prevLen, totalLen = 0, flag = true;

    sprintf(temp, "stack[%p]:\n", stack);
    if (resultSize > strlen(temp)) {
        strcpy(result, temp);
        result += totalLen = strlen(result);
        for (i = 0; i < StackGetSize(stack); i++) {
            sprintf(temp, "%d: %f\n", i, (double) stack->stackItems[i]);
            prevLen = strlen(temp);
            totalLen += prevLen;
            if (totalLen < resultSize) {
              strcpy(result, temp);
              result += prevLen;
            } else {
              // if the loop end prematurely because the total length is greater than 'result'
              // can store, the operation failed.
              flag = false;
              break;
            }
        }
    } else {
      // if the line stack[address] can't fill 'result'.
      flag = false;
    }

    if (i < StackGetSize(stack)) {
       // if not every element can fit into 'result'.
       flag = false;
    }
    return flag ? SUCCESS : ERROR;
}

//#define STACK_TEST
#ifdef STACK_TEST
#include <stdio.h>
#include <math.h>

int FloatEquals(float a, float b)
{
   return fabs(a - b) < 0.0001;
}

#define FUNCTION_NUM_TO_TEST 7
int main(void) {
    struct Stack s;
    int testNum[FUNCTION_NUM_TO_TEST] = {0};
    int testFailed[FUNCTION_NUM_TO_TEST] = {0};
    int i, flag = 0; 
    float temp;

    // TEST #0: StackInit()
    StackInit(&s);
    testNum[0]++;
    if (s.currentItemIndex != -1) {
        puts("s.currentItemIndex != -1 failed.");
       testFailed[0]++; 
    }
    testNum[0]++;
    if (s.initialized != true) {
        puts("s.initialized == 0 failed.");
        testFailed[0]++; 
    }
    testNum[0]++;
    for (i = 0; i < STACK_SIZE; i++) {
        if (s.stackItems[i] != 0) {
            flag = true;
            break;
        }
    }
    if (flag) {
        puts("elements of stack are not initialized to 0.");
        testFailed[0]++;
    }

    /* TEST #1: StackIsEmpty() */
    testNum[1]++;
    StackInit(&s);
    if (StackIsEmpty(&s) == false) {
        puts("StackIsEmpty(&s) == false failed.");
        testFailed[1]++;
    }

    testNum[1]++;
    s.currentItemIndex = 5;
    if (StackIsEmpty(&s) == true) {
        puts("StackIsEmpty(&s) == true failed.");
        testFailed[1]++;
    }

    testNum[1]++;
    StackInit(&s);
    s.initialized = false;
    if (StackIsEmpty(&s) == true) {
        puts("StackIsEmpty(&s) == true failed.");
        testFailed[1]++;
    }

    /* TEST #2: StackPush() */
    testNum[2]++;
    StackInit(&s);
    flag = StackPush(&s, 0.5);
    if (flag != SUCCESS) {
        puts("StackPush(&s, 0.5) == SUCCESS failed.");
        testFailed[2]++;
    }
    testNum[2]++;
    if (s.currentItemIndex != 0) {
        puts("s.currentItemIndex != 0 failed.");
        testFailed[2]++;
    }
    
    testNum[2]++;
    if (s.stackItems[0] != 0.5) {
        puts("s.stackItems[0] != 0.5 failed.");
        testFailed[2]++;
    }

    testNum[2]++;
    s.currentItemIndex = STACK_SIZE - 1; 
    flag = StackPush(&s, 0.123);
    if (flag != STANDARD_ERROR) {
        puts("flag != STANDARD_ERROR failed.");
        testFailed[2]++;
    }
    testNum[2]++;
    if (s.currentItemIndex != STACK_SIZE - 1) {
        puts("s.currentItemIndex != STACK_SIZE - 1 failed.");
        testFailed[2]++;
    }
    
    testNum[2]++;
    s.initialized = false;
    flag = StackPush(&s, 0.458);
    if (flag != STANDARD_ERROR) {
        puts("StackPush pushing onto an uninitialized stack should return"
             "STANDARD_ERROR.");     
        testFailed[2]++;
    }

    /* TEST #3: StackIsFull() */
    testNum[3]++;
    StackInit(&s);
    s.currentItemIndex = STACK_SIZE - 1;
    if (StackIsFull(&s) != true) {
        puts("StackIsFull() should be true when currentItemIndex == STACK_SIZE - 1");
        testFailed[3]++;
    }

    testNum[3]++;
    StackInit(&s);
    s.initialized = false;
    if (StackIsFull(&s) != false) {
        puts("StackIsFull() should be false when stack is not initialized.");
        testFailed[3]++;
    }

    /* TEST #4: StackPop() */
    testNum[4]++;
    StackInit(&s);
    StackPush(&s, 0.123);
    flag = StackPop(&s, &temp);
    if (flag != SUCCESS) {
        puts("StackPop() should return SUCCESS when it pops an item out.");
        testFailed[4]++;
    }

    testNum[4]++;
    if (FloatEquals(temp, 0.123) != true) {
        puts("StackPop() does not pop out and store the correct value from StackItems[].");
        testFailed[4]++;
    }
    
    testNum[4]++;
    flag = StackPop(&s, &temp);
    if (flag != STANDARD_ERROR) {
        puts("StackPop() should return STANDARD_ERROR when it pops from an empty stack.");
        testFailed[4]++;
    } 

    testNum[4]++;
    s.currentItemIndex = STACK_SIZE - 2;
    StackPush(&s, 0.579);
    StackPush(&s, 0.347); 
    flag = StackPop(&s, &temp);
    if (s.currentItemIndex >= STACK_SIZE - 1) {
        puts("StackPop() does not decrease currentItemIndex correctly");
        testFailed[4]++;
    }

    testNum[4]++;
    if (FloatEquals(temp, 0.579) != true) {
        puts("StackPop() shoudl return 0.579.");
        testFailed[4]++;
    } 

    testNum[4]++;
    StackInit(&s);
    s.initialized = false;
    if (StackPop(&s, &temp) != STANDARD_ERROR) {
        puts("StackPop() should return STANDARD_ERROR when the stack is not initialized.");
        testFailed[4]++;
    }

    /* TEST #5: StackGetSize() */
    testNum[5]++;
    StackInit(&s);
    if (StackGetSize(&s) != 0) {
        puts("StackGetSize() should return 0 for an initialized stack.");
        testFailed[5]++;
    }

    testNum[5]++;
    s.initialized = false;
    if (StackGetSize(&s) != SIZE_ERROR) {
        puts("StackGetSize() should return SIZE_ERROR for an uninitialized stack.");
        testFailed[5]++; 
    } 

    testNum[5]++;
    StackInit(&s);
    for (i = 0; i < 10; i++) {
        StackPush(&s, i);
    }
    if (StackGetSize(&s) != 10) {
        puts("StackGetSize() should return 10 after being push for 10 times from empty stack.");
        testFailed[5]++;
    }

    /* TEST #6: StackToString */
    testNum[6]++;
    char result1[2] = "\0"; 
    flag = StackToString(&s, result1, 2); // using the 10 values pushed from previous test.
    if (flag != STANDARD_ERROR) {
        puts("StackToString() shoudl return STANDARD_ERROR if 'result' does not have enough "
             "space.");
        printf("%s\n", result1);    
        testFailed[6]++;
    }
    printf("%s\n", result1);    

    testNum[6]++;
    char result2[1000];
    flag = StackToString(&s, result2, 1000);
    if (flag != SUCCESS) {
        puts("StackToString() shoudl return SUCCESS if 'result' have enough space.");
        printf("%s\n", result2);    
        testFailed[6]++;
    }
    /* TEST SUMMARY */
    puts("\nTest Summary:");
    for (i = 0; i <FUNCTION_NUM_TO_TEST; i++) {
        printf("TEST #%d: %d tests failed out of %d => %.0f%%\n", i, testFailed[i], testNum[i],
                100.0 * (testNum[i] - testFailed[i]) / testNum[i]);
    }
    return 0;
}

#endif


