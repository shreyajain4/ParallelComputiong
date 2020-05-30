#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define n 10

int main(int argc, char** argv) {
   int pid, np; 
  
    MPI_Status status; 
  
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
    MPI_Comm_size(MPI_COMM_WORLD, &np); 
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

	long long * arr;
	long long * rec_arr;
	long long no_element_per_process;
	long long sum=0;
	no_element_per_process = n/np;
	if(n%np!=0)
	{
		printf("No of processes do not perfectly divide the size of array");
		return 0;
	}
	if(pid == 0){
		arr = (long long*)malloc(n*sizeof(long long));
		
		for(long long i=1;i<=n;++i){
			arr[i-1]=i;
		}
	} 
	long long sz  = n;
	rec_arr = (long long*)malloc(no_element_per_process*sizeof(long long));
	MPI_Scatter(arr, no_element_per_process,MPI_LONG_LONG, rec_arr, no_element_per_process, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	for(int i=0;i<no_element_per_process;++i){
		sum+=rec_arr[i];
	}

	long long global_sum;
	MPI_Reduce(&sum,&global_sum,1,MPI_LONG_LONG, MPI_SUM,0,MPI_COMM_WORLD);
	if(pid ==0 ){
		printf("n: %lld sum: %lld \n",n, global_sum);
	}


    MPI_Finalize();
}
