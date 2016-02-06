#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define max 1000000
int buffer[max],empty[max],remain[max];
pthread_t thread[max],writer;
sem_t mutex[max];
int var=0,n,itr;
void* read(void *k)
{
	//while(1)
	//{
		int id=*((int *)k);
		int i,j;
		sleep(1);
		for(j=0;j<itr;j++){
			i=j%n;
			sem_wait(&mutex[i]);
			printf("The %d reader read from index %d\n",id,i);
		}
	//}
}
void* write()
{
	int j=itr;
	while(j>0)
	{
		int i=var%n;
		int ttt=-1;
		while(ttt!=0)
			sem_getvalue(&mutex[i],&ttt);
		printf("Writer had written into index %d\n",i);
		buffer[i]=rand();
		//if(count[i]==n)
		int k;
		for(k=0;k<n;k++)
			sem_post(&mutex[i]);
		var++;
		sleep(1);
		j--;
	}
}
int main()
{
	int i;
	scanf("%d",&n);
	itr=2*n;
	var=0;
	for ( i = 0; i < n; ++i)
	{
		sem_init(&mutex[i],0,0);
	}
	int *var1=malloc(sizeof(*var1));
	pthread_create(&writer,NULL,write,NULL);
	for(i=0;i<n;i++)
	{
		*var1=i;
		pthread_create(&thread[i],NULL,read,var1);
		empty[i]=0;
		count[i]=0;
		remain[i]=n;
		sleep(1);
	}
	pthread_join(writer,NULL);
	for(i=0;i<n;i++)
		pthread_join(thread[i],NULL);
	for (i = 0; i < n; ++i)
	{
		sem_destroy(&mutex[i]);
	}
	return 0;
}
