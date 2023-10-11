#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <unistd.h>

int main(){
    int key = ftok("shmfile", 65);
    int shmid = shmget(key,1024,0666|IPC_CREAT); 

    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    printf("Write data : ");
    scanf("%s",str);
  
    printf("Data written in memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 

    pid_t pid = fork();

    if(pid == 0){
    char *str = (char*) shmat(shmid,(void*)0,0); 
    
    printf("Child Process : ");
    printf("Data read from memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 
    }
}