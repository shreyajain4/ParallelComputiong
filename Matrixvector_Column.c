//Assumption made no. of processes is a factor of size of the array
// to run the code: time mpirun -n (no. of process) ./a.out (size of array)


#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int N,pid;
	MPI_Comm_size(MPI_COMM_WORLD,&N);
	MPI_Comm_rank(MPI_COMM_WORLD,&pid);

	int n = atoi(argv[1]);
	int col = n/N;

	int arr[n][col];
	int b[col];
	int *b_final;
	if(pid == 0)
	{
		b_final = (int *)malloc(n*sizeof(int));
	}
	for(int i = 0 ; i < n ; i++)
	{
		for(int j = 0 ; j < col ; j++)
		{
			arr[i][j] = rand()%n;
			b[j] = rand()%n;;
		}
	}

	int temp_b[n];
	for(int i = 0 ; i < n ; i++)
	{
		temp_b[i] = 0;
		for(int j = 0 ; j < col ; j++)
		{
			temp_b[i] += arr[i][j]*b[j];
		}
	}	

	

	MPI_Reduce(temp_b,b_final,n,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);


	/* To print array
	if(pid == 0)
	{
		for(int i=0;i<n;i++)
			printf("%d ",b_final[i]);
	}*/

	MPI_Finalize();
	return 0;
}