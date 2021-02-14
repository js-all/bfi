#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 5

struct Queue {
    int *data;
    int *front;
    int *rear;
    int used;
    int allocated;
};

void queue_init(struct Queue *queue) {
    queue->data = NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->used = 0;
    queue->allocated = 0;
}

bool queue_is_empty(struct Queue *queue) {
    if(queue->used == 0 || queue->allocated == 0 || queue->data == NULL || queue->front == NULL || queue->rear == NULL) {
        return true;
    } else {
        return false;
    }
}

int queue_enqueue(struct Queue *queue, int *data) {
    if(!queue->allocated) {
        queue->data = malloc(INIT_SIZE * sizeof(int));
        if(queue->data == NULL) {
            return -1;
        }
        queue->data[0] = *data;
        queue->allocated = INIT_SIZE;
        queue->used = 1;
        queue->front = &queue->data[0];
        queue->rear = &queue->data[0];
        return 0;
    } else {
        if(queue->used >= queue->allocated) {
            queue->data = realloc(queue->data, (queue->allocated *= 2) * sizeof(int));
            if(queue->data == NULL) {
                return -1;
            }
        }
        for(int i = queue->used; i > 0; i--) {
            queue->data[i] = queue->data[i-1];
        }
        queue->data[0] = *data;
        queue->used++;
        queue->front = &queue->data[0];
        queue->rear = &queue->data[queue->used-1];
        return 0;
    }
}

int queue_peek(struct Queue *queue, int *val) {
    if(queue_is_empty(queue)) {
        return -1;
    }
    *val = *queue->front;
    return 0;
}

int queue_dequeue(struct Queue *queue, int *val) {
    if(queue_is_empty(queue))
        return -1;
    // we won't free any space so its all that is needed, along with changing top pointer
    queue_peek(queue, val);
    queue->used--;
    if(queue->used == 0)  {
        queue->rear = NULL;
        queue->front = NULL;
    }
    else {
        queue->rear = &queue->data[queue->used-1];
    }
    return 0;
}
// /!\ broken
void queue_display(struct Queue *queue) {
    printf("{ \n");
    printf(" --> [");
    for(int i = queue->used-1; i >= 0; i++) {
        if(i == queue->used-1) {
            printf("%d", queue->data[queue->used-1]);
        } else {
            printf(", %d", queue->data[i]);
        }
    }
    if(queue->rear != NULL) {
        printf("]\n --> rear: %d\n", *queue->rear);
    } else {
        printf("]\n --> rear: NULL\n");
    }
    printf(" --> used: %d\n", queue->used);
    printf(" --> allocated: %d\n}\n", queue->allocated);
}

void queue_free(struct Queue *queue) {
    free(queue->data);
}