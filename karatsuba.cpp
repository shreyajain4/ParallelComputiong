#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>

using namespace std;

struct arg
{
	int n;
	int *p1, *p2, *p3;
};

void *polyMultiplication(void *a)
{
	arg* args = (arg*) a;
	int n = args -> n;


	if (n <= 128)
	{
		args->p3 = new int[2*n-1]();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				args->p3[i+j] += args->p1[i] * args->p2[j];
			}
		}
		
	}
	else
	{
		arg* args_temp = new arg[3];
		for (int i = 0; i < 3; ++i)
		{
			args_temp[i].n = (n) / 2;
		}


		args_temp[0].p1 = args->p1;
		args_temp[0].p2 = args->p2;

		args_temp[1].p1 = new int[n/2];
		args_temp[1].p2 = new int[n/2];

		for (int i = 0; i < n/2; i++)
		{
			args_temp[1].p1[i] = args->p1[i] + args->p1[i+n/2];
			args_temp[1].p2[i] = args->p2[i] + args->p2[i+n/2];
		}

		args_temp[2].p1 = &(args->p1[n/2]);
		args_temp[2].p2 = &(args->p2[n/2]);

		pthread_t threads[3];
		pthread_attr_t attr[3];

		for (int i = 0; i < 3; i++)
		{
			pthread_attr_init(&attr[i]);
			pthread_create(&threads[i], &attr[i], polyMultiplication, (void *)&args_temp[i]);
		}
	
		for (int i = 0; i < 3; i++)
		{
			pthread_join(threads[i], NULL);
		}

		args->p3 = new int[2*n-1]();

		for (int i = 0; i < n; i++)
		{
			args->p3[i] = args_temp[0].p3[i];
			args->p3[i+n] = args_temp[2].p3[i];
		}

		for (int i = 0; i < n; i++)
		{
			args->p3[i + n / 2] += args_temp[1].p3[i] - args_temp[0].p3[i] - args_temp[2].p3[i];
		}
	}
}

int main(int argc, char *argv[])
{
	cout<<"Change the value of n"<<endl;
	int n = 5;
	int* p1=(int*) malloc(n * sizeof( int));
	int* p2=(int*) malloc(n * sizeof( int));
	int* p3=(int*) malloc((2*n-1) * sizeof( int));
	for(int i=0;i<n;i++)
	{
		p1[i]=1;
	}
	for(int i=0;i<n;i++)
	{
		p2[i]=1;
	}

	
	double start, stop;

	arg* args = new arg();
	args -> n = n;
	args -> p1 = p1;
	args -> p2 = p2;
	args -> p3 = p3;

	polyMultiplication((void *)args);
	for(int i=0;i<2*n-1;i++)
	cout<<args->p3[i]<<endl;



	return 0;
}