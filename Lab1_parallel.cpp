#include<bits/stdc++.h>
using namespace std;
double A[2000][2000] = {1};
double B[2000][2000] = {1};
double C[2000][2000] = {0};
int n = 2000;
int s = 200;

double timer = double(CLOCKS_PER_SEC);
void mul() {
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			for(int k=0; k<n; k++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mulOpt1() {
	for(int i=0; i<n; i++) {
		for(int k=0; k<n; k++) {
			for(int j=0; j<n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mulOpt2() {
	for(int i=0; i<n/s; i++) {
		int iInit = i*s;
		for(int j=0; j<n/s; j++) {
			int jInit = j*s;
			for(int k=0; k<n/s; k++) {
				int kInit = k*s;
				for(int p=iInit; p<iInit+s; p++) {
					for(int q=jInit; q<jInit+s; q++) {
						for(int r=0; r<s; r++) {
							C[p][q] += A[p][r+kInit] * B[r+kInit][q];
						}
					}
				}
			}
		}
	}
}

void mulOpt2Opt1() {
	for(int i=0; i<n/s; i++) {
		int is = i*s;
		 for(int k=0; k<n/s; k++) {
			int ks = k*s; 
			for(int j=0; j<n/s; j++) {
				int js = j*s;
				for(int p=is; p<is+s; p++) {
					for(int r=0; r<s; r++) {
						for(int q=js; q<js+s; q++) {
							C[p][q] += A[p][r+ks] * B[r+ks][q];
						}
					}
				}
			}
		}
	}
}

const int MAX = 100; 
  
void mulRec(int ax1,int ax2,int ay1,int ay2,int bx1,int bx2,int by1,int by2){
    if(ax1==ax2 && ay1==ay2){
        M3[ax1][by1] += M1[ax1][ay1]*M2[bx1][by1];
        return ;
    }

    int l1 = (ax1+ax2)>>1;
    int l2 = (ay1+ay2)>>1;
    int l3 = (bx1+bx2)>>1;
    int l4 = (by1+by2)>>1;
    
    mulRec(ax1,l1,ay1,l2,bx1,l3,by1,l4);  			
    mulRec(ax1,l1,l2+1,ay2,l3+1,bx2,by1,l4); 		
    mulRec(ax1,l1,ay1,l2,bx1,l3,l4+1,by2);  		
    mulRec(ax1,l1,l2+1,ay2,l3+1,bx2,l4+1,by2); 	
    mulRec(l1+1,ax2,ay1,l2,bx1,l3,by1,l4); 		
    mulRec(l1+1,ax2,l2+1,ay2,l3+1,bx2,by1,l4); 	
    mulRec(l1+1,ax2,ay1,l2,bx1,l3,l4+1,by2); 		
    mulRec(l1+1,ax2,l2+1,ay2,l3+1,bx2,l4+1,by2); 	
    
}

int main() {
	
	double start = clock();
	mul();
	double end = clock();
	double et = (end-start)/timer;
	cout<<"Normal matrix multiplication: "<<et<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			C[i][j]=0;
		}
	}
	start = clock();
	mulOpt1();
	end = clock();
	et = (end-start)/timer;
	cout<<"Optimization 1: "<<et<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			C[i][j]=0;
		}
	}
	start = clock();
	mulOpt2();
	end = clock();
	et = (end-start)/timer;
	cout<<"Optimization 2: "<<et<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			C[i][j]=0;
		}
	}
	start = clock();
	mulOpt2Opt1();
	end = clock();
	et = (end-start)/timer;
	cout<<"Optimization 2 with Optimization 1: "<<et<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			C[i][j]=0;
		}
	}
	start = clock();
	mulRec(0,n-1,0,n-1,0,n-1,0,n-1);
	end = clock();
	et = (end-start)/timer;
	cout<<"Recursion"<<et<<endl;
	
}
