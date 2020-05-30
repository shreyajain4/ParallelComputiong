#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int np;
    long long p=np;
	long long k=2;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    while(k<=p)
	{
		if(k==p)
		break;
		k=k*2;
	}
	
    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	if(k>p && pid==0)
	{
		printf("no. of process should be a power of two");
		return 0;
	}
	if(k>p)
	return 0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    long long n = 10;
	long long * arr;
	long long no_element_per_process;
	long long sum=0;
	long long usum=0;
	long long start;
	long long height = (long long)log2((double)np);
	no_element_per_process = n/np;
	
	
	// printf("%lld %lld",n,height);
	if(pid == 0){
		arr = (long long*)malloc(n*sizeof(long long));
		
		for(long long i=1;i<=n;++i){
			arr[i-1]=i;
		}
		start = 0;
		if(n%np!=0)
		{
			for(int i=n-n%np;i<n;i++)
			usum+=arr[i];
		}
		printf("%lld",usum);
		
	} 
	n-=(n%np);
	long long sz  = n;
	while(height>0){
		sz = sz>>1;
		if( pid%(1<<height) == 0 ){
			int h = height-1;
			h = 1<<h;
			if(h+pid<np)
			MPI_Send(arr + sz, sz, MPI_LONG_LONG, h + pid , 0, MPI_COMM_WORLD);
			
		}
		else if( pid%(1<<(height-1)) == 0 ){
			int h = height-1;
			h = 1<<h;
			arr = (long long*)malloc(sz*sizeof(long long));
			if(pid-h>=0)
			MPI_Recv(arr, sz, MPI_LONG_LONG, pid - h, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		height--;
	}

	for(int i=0;i<no_element_per_process;++i){
		sum+=arr[i];
	}
	long long depth = 1;
	height = (long long)log2((double)np);
	while(depth <= height){ 
		if(pid%(1<<depth) == 0 ){
			long long temp;
			if((1<<(depth-1)) + pid<np)
			MPI_Recv(&temp, 1, MPI_LONG_LONG, (1<<(depth-1)) + pid , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			sum+=temp;
		}
		else if( pid%(1<<(depth-1)) == 0 ){
			if((pid - (1<<(depth-1)))<np)
			MPI_Send(&sum, 1, MPI_LONG_LONG, pid - (1<<(depth-1)), 0    , MPI_COMM_WORLD);
		}
		depth++;
	}
	if(pid ==0 ){
		printf("sum: %lld \n", sum+usum);
	}
    // Finalize the MPI environment.
    MPI_Finalize();
}
