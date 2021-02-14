#ifndef bool
#include <stdbool.h>
#endif
struct Stack {
    long *data;
    long *top;
    int used;
    int allocated;
};
void stack_init(struct Stack *stack);
bool stack_is_empty(struct Stack *stack);
int stack_push(struct Stack *stack, long *data);
int stack_peek(struct Stack *stack, long *val);
int stack_pop(struct Stack *stack, long *val);
void stack_display(struct Stack *stack);
void stack_free(struct Stack *stack);