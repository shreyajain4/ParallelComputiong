// to see time run program like this: time mpirun -n 4 ./a.out

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
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

    int n = world_size*2, source, dest;
    

    if (rank == 0)
    {
    	int *arr =(int *)malloc(n * sizeof(int));
        
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand()%n;
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = i%2; j < n-(i%2); j+=2)
            {
                dest = j/2;
                int *temp= (int *)malloc(2*sizeof(int));
                if(dest)
                {
                    MPI_Ssend((arr + j), 2, MPI_INT, dest, 0, MPI_COMM_WORLD);
                    MPI_Recv(temp, 2, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    
                    arr[j] = temp[0];
                    arr[j+1] = temp[1];
                }
                else
                {
                    if(arr[j] > arr[j+1])
                        swap(&arr[j], &arr[j+1]);
                }
            }
        }
        //To display array uncomment
        /*for (int i = 0; i < n; i++)
	{
	    printf("%d ", arr[i]);
	}
	printf("\n");
	*/
    }
    else
    {
        for (int i = 0; i < (rank==(world_size-1)?(n/2):n); i++)
        {
            int *arr= (int *)malloc(2 * sizeof(int));
            source = 0;
            MPI_Recv(arr, 2, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (arr[0] > arr[1])
                swap(&arr[0], &arr[1]);
            MPI_Ssend(arr, 2, MPI_INT, source, 0, MPI_COMM_WORLD);
        }
    }

    

    // Finalize the MPI environment.
    MPI_Finalize();
    
}