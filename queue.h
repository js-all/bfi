struct QueueItem {
    int data;
    struct QueueItem *next;
};
struct Queue {
    struct QueueItem *front;
    struct QueueItem *rear;
};
void queue_init(struct Queue *queue);
void queue_enqueue(struct Queue *queue, int *data);
int queue_is_empty(struct Queue *queue);
int queue_peek(struct Queue *queue, int *val);
void queue_display(struct Queue *queue);
int queue_dequeue(struct Queue *queue, int *val);
void queue_free(struct Queue *queue);