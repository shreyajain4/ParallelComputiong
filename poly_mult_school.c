#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


long int* p1;
long int* p2;
long int* p3;
int m=4;
int n=5;
int t=2;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

void* sum_runner(void* arg)
{
	int i=(int) arg;


	for (int j = 0; j<n; j++) {
		pthread_mutex_lock(&mutex);
		p3[i+j]+=p1[i]*p2[j];
		pthread_mutex_unlock(&mutex);
		//printf("%d",j);
	}

	pthread_exit(0);
}

int main(int argc, char **argv)
{
	printf("change m,n,t for variations");
	
	p1=(long int*) malloc(m * sizeof(long int));
	p2=(long int*) malloc(n * sizeof(long int));
	
	for(int i=0;i<m;i++)
	{
		p1[i]=1;
	}
	for(int i=0;i<n;i++)
	{
		p2[i]=1;
	}

	
	// Launch thread
	
	
	p3=(long int*) malloc((m+n-1) * sizeof(long int));
	pthread_t tids[t];
	
	for (int i = 0; i < m; i++) {

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i%t], &attr, sum_runner, (void*)i);
		
	}


	for (int i = 0; i < t; i++) {
		pthread_join(tids[i], NULL);
	}
	for (int i = 0; i <m+n-1; i++) {
		printf("%li ",p3[i]);
	}
}
