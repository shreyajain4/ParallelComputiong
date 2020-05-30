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
	int row = n/N;

	int arr[row][n];
	int b[row];
	for(int i = 0 ; i < n ; i++)
	{
		for(int j = 0 ; j < row ; j++)
		{
			arr[j][i] = rand()%n;
			if(i==0)
			b[j] = rand()%n;
		}
	}
	int b_final[n];
	MPI_Allgather(b,row,MPI_INT,b_final,row,MPI_INT,MPI_COMM_WORLD);

	for(int i = 0;i < row ;i++)
	{
		b[i] = 0;
		for(int j = 0;j < n ; j++)
		{
			b[i] += arr[i][j]*b_final[j];
		}
	}
	
	MPI_Allgather(b,row,MPI_INT,b_final,row,MPI_INT,MPI_COMM_WORLD);
	//If we need to print the result
	/*if(pid == 0)
	{
		for(int i=0;i<n;i++)
			printf("%d ",b_final[i]);
	}*/
	MPI_Finalize();
	
	return 0;
}