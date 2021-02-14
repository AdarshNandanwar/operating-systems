
// Semaphore solution for Readers-Writer Problem - Readers have priority

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#define N 1000
#define M 3

sem_t mutex, rw;
int data = 0, rcount = 0;
int readers_finished = 0, writers_finished = 0;

void * writer (int writer_no)
{
  sem_wait (&rw);
  // Write operation on data
  data++;
  printf ("\nWrite operation by writer : %d , Data = %d ", writer_no, data);
   // if(++writers_finished == N)
   //      printf("\n\nALL WRITERS FINISHED\n\n");
  sem_post (&rw);
}

void * reader (int reader_no)
{
  // Reader enters and updates rcount
  sem_wait (&mutex);
  rcount++;
  if (rcount == 1)
    sem_wait (&rw);		// Wait for writer to finish writing
  sem_post (&mutex);

  // Read operation on data
  printf ("\nRead operation by reader :  %d , Data = %d ", reader_no, data);
   // if(++readers_finished == N)
   //      printf("\n\nALL READERS FINISHED\n\n");
  // Readers exits 
  sem_wait (&mutex);
  rcount--;
  if (rcount == 0)
    sem_post (&rw);
  sem_post (&mutex);

}

int main ()
{
  printf ("\nProgram has begun \n");
  pthread_t readers[N], writers[N];
  sem_init (&rw, 0, 1);
  sem_init (&mutex, 0, 1);
  for (int i = 0; i < N; i++)
    {
      pthread_create (&readers[i], NULL, reader, i + 1);
      pthread_create (&writers[i], NULL, writer, i + 1);
    }

  for (int i = 0; i < N; i++)
    {
      pthread_join (readers[i], NULL);
      pthread_join (writers[i], NULL);
    }

  printf ("\n\nProgram has ended \n");
  return 0;
}
