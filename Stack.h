#ifndef STACK_H
#define STACK_H

#include <stdint.h>

// Keep a fixed stack size as a constant. This variable should be used whenever possible, such as
// finding the last element of the stack should do `stack.stackItems[STACK_SIZE - 1]` instead of
// `stack.stackItems[19]`.
#define STACK_SIZE 100

/**
 * Here we declare the stack struct. While a stack works as a FILO (first-in,
 * last-out) we will be implementing this on top of a C array. The way this
 * will work is that the bottom of the stack will be array element 0. The top
 * of the stack will be the last element in the array. The index of that last
 * element (the top of the stack) will be stored in currentItemIndex.
 *
 * So:
 *  - stackItems: Contains all the floats that are in the queue in ascending
 *                order.
 *  - currentItemIndex: Contains the index in stackItems of the top of the
 *                      stack.
 *  - intialized: Stores whether or not the array was initialized. false if
 *                it isn't and true if it is.
 */
struct Stack {
    uint8_t stackItems[STACK_SIZE];
    int currentItemIndex;
    int initialized;
};

/**
 * This function initializes the stack. For a statically allocated stack such
 * as this, this just means initializing currentItemIndex to -1 and setting
 * initialized to true. It does not check if stack is already initialized nor
 * can it fail, so it should always return SUCCESS.
 */
int StackInit(struct Stack *stack);

/**
 * This function pushes a value onto the stack. It returns SUCCESS for success
 * and STANDARD_ERROR for a failure. A failure occurs when trying to push onto full or
 * non-initialized stacks. (SUCCESS and STANDARD_ERROR are declared in the Common.h header file.
 */
int StackPush(struct Stack *stack, uint8_t value);

/**
 * This function returns the top of the stack into the value point location.
 * It also returns a success code: SUCCESS for success and STANDARD_ERROR for failure.
 * Failure occurs when Pop() is called on empty stacks or non-initialized
 * stacks.
 *
 * NOTE: When trying to return the output into the value variable, you'll need
 * to write it with an extra asterisk like:
 *   *value = 7.0;
 * This is because of pointers and K&R 5.1-5.2 should explain it. This is the
 * only time in your lab where you'll need to do something like that for an
 * assignment operation.
 */
int StackPop(struct Stack *stack, uint8_t *value);

/**
 * This function checks for whether the stack is empty or not. In this simple
 * case this merely checks if the currentItemIndex == -1. Returns true if the
 * stack is empty, false if not. Also needs to check if the stack is
 * initialized and return false if it isn't.
 */
int StackIsEmpty(const struct Stack *stack);

/**
 * This function checks for whether the stack is empty or not. For this simple
 * implementation that is merely if the currentItemIndex == STACK_SIZE - 1. 
 * Returns true if the stack is full and false if not. Also returns false if
 * the stack isn't initialized.
 */
int StackIsFull(const struct Stack *stack);

/**
 * Returns the current size of the stack in terms of how many active elements
 * are in it. Returns an error of SIZE_ERROR for uninitialized stacks, 0 for
 * empty and initialized stacks, and the number of elements in the stack
 * otherwise. SIZE_ERROR is declared in the Common.h header file.
 */
int StackGetSize(const struct Stack *stack);

/**
 * ToString() provides a way of string-ifying an entire stack, perhaps useful
 * to pass to printf(). It creates a
 * string of the passed stack according to the following template:
 * "stack[STACK_MEMORY_ADDRESS]:
 *  ELEMENT_NUMBER:DATA
 *  ELEMENT_NUMBER:DATA"
 * where STACK_MEMORY_ADDRESS is the address of the stack argument,
 * ELEMENT_NUMBER and DATA are the index and data of all of the stack elements.
 * The output string is stored into result if there is enough space. If there
 * is not enough space within result (as determined by the argument resultSize)
 * ToString() returns a STANDARD_ERROR and the data in result is undefined.
 * If there is enough space within the result string, the entire string
 * representation of stack is stored there and the return value is SUCCESS.
 */
int StackToString(const struct Stack *stack, char *result, int resultSize);

#endif // STACK_H
