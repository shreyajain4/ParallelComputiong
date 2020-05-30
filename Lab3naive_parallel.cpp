
#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
  

#define n 10  
  

  
int main(int argc, char* argv[]) 
{
    int pid, np, elements_per_process, n_elements_recieved; 
  
    MPI_Status status; 
  
    MPI_Init(&argc, &argv); 
  

    MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
    MPI_Comm_size(MPI_COMM_WORLD, &np); 
  
 
    if (pid == 0) { 
        int index, i; 
        elements_per_process = n / np; 
		int a[n];
		for(int i=0;i<n;i++)
		{
			a[i]=2;
		}
        if (np > 1) { 

            for (i = 1; i < np ; i++) { 
                index = i * elements_per_process; 
  
                MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
                MPI_Send(&a[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD); 
            }   
        } 

        int sum = 0; 
        for (i = i * elements_per_process; i < n; i++) 
            sum += a[i]; 
        for (i = 0; i < elements_per_process; i++) 
            sum += a[i]; 
        int tmp; 
        for (i = 1; i < np; i++) { 
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); 
            sum += tmp; 
        } 
  

        printf("Sum of array is : %d\n", sum); 
    } 
    else { 
		 
        MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
		int a2[n_elements_recieved];
        MPI_Recv(&a2, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 

        int partial_sum = 0; 
        for (int i = 0; i < n_elements_recieved; i++) 
            partial_sum += a2[i]; 
		//printf("%d",a[1]);

        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
    }  
    MPI_Finalize(); 
  
    return 0; 
} 
