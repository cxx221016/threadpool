#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>

#define QUEUE_SIZE 20
#define THREADNUMBERS 3


typedef void(*func)(void*);

typedef struct
{
    func f;
    void *data;
}task;

struct Node
{
    task val;
    struct Node *next;
};
typedef struct Node Node;



void execute(func f,void* ptr);
int push_back(func f,void* ptr);
void* pop_back(void* para);
void threadpool(void);
void _threadpool(void);