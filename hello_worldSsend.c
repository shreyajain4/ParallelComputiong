//for 2 processes only
#include<stdio.h>
#include<mpi.h>


int main(int argc,char** argv)
{
	MPI_Init(&argc,&argv);
	int p_count;
	MPI_Comm_size(MPI_COMM_WORLD,&p_count);

	int pid;
	MPI_Comm_rank(MPI_COMM_WORLD,&pid);
	
	if(!pid%2)
	{
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		printf("PE %d WORLD\n",pid);
		
		MPI_Recv(&pid,1,MPI_INT,1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("PE %d WORLD\n",pid);
	}
	else
	{
		MPI_Ssend(&pid,1,MPI_INT,0,1,MPI_COMM_WORLD);
		printf("PE %d HELLO\n",pid);
		
	}
	
	
	MPI_Finalize();
	return 0;
}