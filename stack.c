#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 5

struct Stack {
    long *data;
    long *top;
    int used;
    int allocated;
};

void stack_init(struct Stack *stack) {
    stack->data = NULL;
    stack->top = NULL;
    stack->used = 0;
    stack->allocated = 0;
}

bool stack_is_empty(struct Stack *stack) {
    if(stack->data == NULL || stack->top == NULL || stack->allocated == 0 || stack->used == 0) {
        return true;
    } else {
        return false;
    }
}

int stack_push(struct Stack *stack, long *data) {
    if(!stack->allocated) {
        stack->data = malloc(INIT_SIZE * sizeof(long));
        if(stack->data == NULL) {
            return -1;
        }
        stack->data[0] = *data;
        stack->allocated = INIT_SIZE;
        stack->used = 1;
        stack->top = &stack->data[0];
        return 0;
    } else {
        if(stack->used >= stack->allocated) {
            stack->data = realloc(stack->data, (stack->allocated *= 2) * sizeof(long));
            if(stack->data == NULL) {
                return -1;
            }
        }
        stack->data[stack->used++] = *data;
        stack->top = &stack->data[stack->used-1];
        return 0;
    }
}

int stack_peek(struct Stack *stack, long *val) {
    if(stack_is_empty(stack)) {
        return -1;
    }
    *val = *stack->top;
    return 0;
}

int stack_pop(struct Stack *stack, long *val) {
    if(stack_is_empty(stack))
        return -1;
    // we won't free any space so its all that is needed, along with changing top pointer
    stack_peek(stack, val);
    stack->used--;
    if(stack->used == 0) 
        stack->top = NULL;
    else
        stack->top = &stack->data[stack->used-1];
    return 0;
}

void stack_display(struct Stack *stack) {
    printf("{ \n");
    printf(" --> [");
    for(int i = 0; i < stack->used; i++) {
        if(i == 0) {
            printf("%ld", stack->data[0]);
        } else {
            printf(", %ld", stack->data[i]);
        }
    }
    if(stack->top != NULL) {
        printf("]\n --> top: %ld\n", *stack->top);
    } else {
        printf("]\n --> top: NULL\n");
    }
    printf(" --> used: %d\n", stack->used);
    printf(" --> allocated: %d\n}\n", stack->allocated);
}

void stack_free(struct Stack *stack) {
    free(stack);
}