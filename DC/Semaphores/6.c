sem_t s1, s2;
int val = 0;



void * runnerA(void *arg1)
{
               while(1)
               {

sem_wait(&s1);
if(val == 0)

sem_post(&s2);

else

val = 1;



sem_post(&s1);




                          //CS – [One time execution in Critical Section]

sem_wait(&s1);
if(val == 1)
val = 0;

sem_post(&s1);


               }
               pthread_exit(0);
}

void * runnerB(void *arg1)
{
               while(1)
               {

sem_wait(&s1);
if(val == 0)
sem_post(&s2);

else

val = 1;

sem_post(&s1);



                          //CS – [One time execution in Critical Section]

sem_wait(&s1);
if(val == 1)
val = 0;

sem_post(&s1);


               }
               pthread_exit(0);
}

void * runnerC(void *arg1)
{
               while(1)
               {

sem_wait(&s2);




                          //CS – [One time execution in Critical Section]

sem_post(&s2);

               }
               pthread_exit(0);
}


int main(int argc,char *argv[]){


sem_init(&s1, 0, 1);
sem_init(&s2, 0, 0);

// CREATE
pthread_t tidA, tidB, tidC;
pthread_attr_t attr;
pthread_attr_init(&attr);


pthread_create(&tidA, &attr, &runnerA, param);
pthread_create(&tidA, &attr, &runnerB, param);
pthread_create(&tidA, &attr, &runnerC, param);

// JOIN

// return int

pthread_join(tidA, (void **) &res_int);
pthread_join(tidB, (void **) &res_int);
pthread_join(tidC, (void **) &res_int);


return 0;
}