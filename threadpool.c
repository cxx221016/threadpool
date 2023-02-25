#include "threadpool.h"

Node *head=NULL,*tail=NULL;



//pthread_t* bee=(pthread_t*)malloc(sizeof(pthread_t)*THREADNUMBERS);
pthread_t bee[THREADNUMBERS];
pthread_mutex_t mutex;
sem_t sem;

int  flag;

/*
int push(task t)
{
    if(!tail)
    {
        head=(Node*) malloc(sizeof(Node));
        tail=(Node*) malloc(sizeof(Node));
        head->val=t;
        tail->val=t;
        head->next=NULL;
        tail->next=NULL;
    }
    else 
    {
        tail->next=(Node*) malloc(sizeof(Node));
        if(tail->next==NULL) return 1;
        tail=tail->next;
        tail->val=t;
        tail->next=NULL;
    }
    return 0;
}

task pop()
{
    if(head==NULL)
    {
        fprintf(stderr,"the queue is empty\n");
        exit(1);
    }
    static task res;
    res=head->val;
    Node* tmp=head;
    head=head->next;
    free(tmp);
    if(!head) tail=NULL;
    return res;
}*/

int push(task t)
{
    tail->next=(Node*)malloc(sizeof(Node));
    if(tail->next==NULL) return 1;
    tail=tail->next;
    tail->val=t;
    return 0;
}

task pop()
{
    if(head==tail)
    {
        fprintf(stderr,"the queue is empty\n");
        exit(1);
    }

    static Node* tmp;
    tmp=head;
    head=head->next;
    free(tmp);
    return head->val;
}

void* pop_back(void* para)
{
    static task curtask;
    while(true)
    {
        sem_wait(&sem);
        if(flag) break;
        pthread_mutex_lock(&mutex);
        curtask=pop();
        pthread_mutex_unlock(&mutex);

        execute(curtask.f,curtask.data);

    }
    pthread_exit(0);
}

void execute(func f,void* ptr)
{
    f(ptr);
}

int push_back(func f,void* ptr)
{
    static task curtask;
    curtask.f=f;
    curtask.data=ptr;
    pthread_mutex_lock(&mutex);
    int res=push(curtask);
    pthread_mutex_unlock(&mutex);
    if(res==0) sem_post(&sem);
    return res;
}
void threadpool(void)
{
    flag=false;
    head = (Node*) malloc (sizeof(Node));
	if (head == NULL) {
		fprintf(stderr, "Error: queue initialization error!\n");
		exit(1);
	}
	head -> next = NULL;
	tail = head;
    static int error;
    error=pthread_mutex_init(&mutex,NULL);
    if(error)
    {
        fprintf(stderr,"pthread_mutex_t initoalization error\n");
        exit(1);
    }

    error=sem_init(&sem,0,0);
    if(error)
    {
        fprintf(stderr,"sem_t initoalization error\n");
        exit(1);
    }

    for(int i=0;i<THREADNUMBERS;++i)
    {
        error=pthread_create(bee+i,NULL,pop_back,NULL);
        if (error) 
        {
			fprintf(stderr, "Error: pthread create error!\n");
			exit(1);
		}
    }
    fprintf(stdout,"the threadpools has been created\n");

}
void _threadpool(void)
{
    static int error;
    flag=true;


    for(int i=0;i<THREADNUMBERS;++i)
    {
        sem_post(&sem);
    }

    for(int i=0;i<THREADNUMBERS;++i)
    {
        error=pthread_join(*(bee+i),NULL);
        if(error)
        {
            fprintf(stderr,"pthread join fail");
            exit(1);
        }
    }
    fprintf(stdout,"pthread join seccessfully\n");
    error=pthread_mutex_destroy(&mutex);
    if(error)
    {
        fprintf(stderr,"pthread_mutex_t destroy fail");
        exit(1);
    }
   
    error=sem_destroy(&sem);
    if(error)
    {
        fprintf(stderr,"sem_t destroy fail");
        exit(1);
    }

}