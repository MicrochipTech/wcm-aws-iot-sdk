#include <stdint.h>
#include <bsp_config.h>

// The max number of objects that can be put onto queue
#define MY_QUEUE_SIZE 20

// The obtect struct type that gets stored into the queue
typedef struct queue_object{
    //If true, send current value in publish
    bool potentiometer_value;  // Potentiometer "POT" on board
    bool s1, s2, s3, s4; // Switches S1-S4 states on board
    bool led1, led2, led3, led4;
} MY_QUEUE_OBJECT;

// Queue specific parameters that manage the FIFO queueing
typedef struct queue{
    MY_QUEUE_OBJECT message_queue[MY_QUEUE_SIZE];
    volatile int32_t front;
    volatile int32_t back;
    volatile int32_t count;
} MY_QUEUE;

//Function Prototypes
uint32_t queue_isFull(MY_QUEUE * q);
void enqueue(MY_QUEUE * q, MY_QUEUE_OBJECT queueObject);
void dequeue(MY_QUEUE * q);
uint32_t queue_isEmpty(MY_QUEUE * q);
void init_queue(MY_QUEUE * q);