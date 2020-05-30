#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int main(int argc, char **argv)
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(7);
	int m = 10, n = 10, p = world_size, count = m/p;

	
	
	float *data = (float *)malloc(m * n * sizeof(float));
	float **a = (float **)malloc(m * sizeof(float *));
	for (int i = 0; i < m; i++)
	{
		a[i] = &(data[i * n]);
	}
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			a[i][j]=rand()%m;
		}
	}
	
	for (int row = 0; row < m; row++)
	{
		float *root = (float *)malloc(n * sizeof(float));
		

		if(row%p == rank)
		{
			for (int j = n-1; j >= row; j--)
			{
				a[row][j] /= a[row][row];
			}

			for (int j = 0; j < n; j++)
			{
				root[j] = a[row][j];
			}
			
			for (int i = 0; i < p; i++)
			{
				if (i != rank)
				{					
					MPI_Ssend(root,n,MPI_FLOAT,i,1,MPI_COMM_WORLD);
				}
			}
		}
		else
		{
			MPI_Recv(root,n,MPI_FLOAT,row%p,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}

		for (int j = 0; j < n; j++)
		{
			a[row][j] = root[j];
		}

		for (int local_row = row+1; local_row < m; local_row++)
		{
			if(local_row%p != rank)
				continue;
			float multiplier = a[local_row][row] / root[row];
			for (int j = row; j < n; j++)
			{
				a[local_row][j] -= multiplier * root[j];
			}
		}

	}
	
	// Finalize the MPI environment.
	MPI_Finalize();
	
}