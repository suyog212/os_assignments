#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/ipc.h>

#define BUFFER_SIZE 10

int in = 0;
int buffer[BUFFER_SIZE];
int out = 0;
int wait_time = 1;

sem_t empty,full;
pthread_mutex_t mutex;


void insert(int item){
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    in = (in+1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

}
int delete(){
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    int item = buffer[out];
    out = (out +1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return item;
}

void* producer(void* args){
    int item = 0;
    while (1)
    {
        item++;
        printf("Produced : %d\n",item);
        insert(item);
        sleep(wait_time);
    }
    return NULL;
}
void* consumer(void* args){
    while (1)
    {
        int item = delete();
        printf("Consumed : %d\n",item);
        sleep(wait_time);
    }
    return NULL;
}

int main(){
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&mutex,NULL);
    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread,NULL,producer,NULL);
    pthread_create(&consumer_thread,NULL,producer,NULL);

    pthread_join(producer_thread,NULL);
    pthread_join(consumer_thread,NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);
}
