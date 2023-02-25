#include"threadpool.h"

#define NUMBER_OF_WORK 50

typedef struct
{
    int a;
    int b;
}data;

void add(void* para)
{
    data* tmp=(data*) para;
    printf("%d + %d = %d \n",tmp->a,tmp->b,tmp->a+tmp->b);
}

int main(void)
{
    //data* works=(data*) malloc(sizeof(data)*NUMBER);
    static data works[NUMBER_OF_WORK];
    for(int i=0;i<NUMBER_OF_WORK;++i)
    {
        //works[i].a=rand()%100;
        //works[i].b=rand()%100;
        works[i].a=i;
        works[i].b=i;
    }

    threadpool();
    for(int i=0;i<NUMBER_OF_WORK;++i)
    {
        static int error;
        //printf("%d\n",i);
        error=push_back(&add,works+i);
        if(error) fprintf(stderr,"work id %d error because the threadpool is full\n",i);
    }
    sleep(3);
    _threadpool();
    return 0;
}


