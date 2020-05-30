#include <bits/stdc++.h> 
using namespace std; 
typedef complex<double> cd; 
struct data{
    cd* a;
    cd* ans;
    int n;
    data(cd* pa,cd* pans,int pn){
        a = pa;
        ans=pans;
        n = pn;
    }
};
void* inv_fft(void* aa) 
{ 

    struct data* af = (struct data*)aa;
    cd* a = af->a;
    cd* ans = af->ans;
    int n = af->n;
  
    // if input contains just one element 
    if (n == 1){
        ans[0] = a[0];
        pthread_exit(NULL);
    }
    // For storing n complex nth roots of unity 
    vector<cd> w(n); 
    for (int i = 0; i < n; i++) { 
        double alpha = 2 * M_PI * i / n;
        alpha=-alpha; 
        w[i] = cd(cos(alpha), sin(alpha)); 
    } 
    cd* A0  = new cd[n/2];
    cd* A1  = new cd[n/2];
    cd* y0 = new cd[n/2];
    cd* y1 = new cd[n/2];
    for (int i = 0; i < n / 2; i++) { 
  
        // even indexed coefficients 
        A0[i] = a[i * 2]; 
        // odd indexed coefficients 
        A1[i] = a[i * 2 + 1]; 
    } 
    //cout<<"ok"<<endl;
    pthread_t th[2];

    
    struct data d0(A0,y0,n/2);

    struct data d1(A1,y1,n/2);
    // Recursive call for even indexed coefficients 
   
    pthread_create(&th[0],NULL,inv_fft,(void*)&d0);
    pthread_create(&th[1],NULL,inv_fft,(void*)&d1);

    // for storing values of y0, y1, y2, ..., yn-1. 
    for(int i=0;i<2;++i){
        pthread_join(th[i],NULL);
    }
    delete[] A0;
    delete[] A1;
    for (int k = 0; k < n / 2; k++) { 
        ans[k] = y0[k] + w[k] * y1[k]; 
        ans[k + n / 2] = y0[k] - w[k] * y1[k]; 
    } 
    delete[] y0;
    delete[] y1;
    pthread_exit(NULL);
} 
// Recursive function of FFT 
void * fft(void* aa) 
{ 
    struct data* af = (struct data*)aa;
    cd* a = af->a;
    cd* ans = af->ans;
    int n = af->n;
    // if input contains just one element 
    if (n == 1){
        ans[0] = a[0];
        pthread_exit(NULL);
    }
    // For storing n complex nth roots of unity 
    vector<cd> w(n); 
    for (int i = 0; i < n; i++) { 
        double alpha = 2 * M_PI * i / n; 
        w[i] = cd(cos(alpha), sin(alpha)); 
    } 
    
    cd* A0  = new cd[n/2];
    cd* A1  = new cd[n/2];
    cd* y0 = new cd[n/2];
    cd* y1 = new cd[n/2];
    for (int i = 0; i < n / 2; i++) { 
  
        // even indexed coefficients 
        A0[i] = a[i * 2]; 
  
        // odd indexed coefficients 
        A1[i] = a[i * 2 + 1]; 
    } 
    pthread_t th[2];

    
    struct data d0(A0,y0,n/2);

    struct data d1(A1,y1,n/2);
    // Recursive call for even indexed coefficients 
   
    pthread_create(&th[0],NULL,fft,(void*)&d0);
    pthread_create(&th[1],NULL,fft,(void*)&d1);

    // for storing values of y0, y1, y2, ..., yn-1. 
    for(int i=0;i<2;++i){
        pthread_join(th[i],NULL);
    }
    delete[] A0;
    delete[] A1;
    for (int k = 0; k < n / 2; k++) { 
        ans[k] = y0[k] + w[k] * y1[k]; 
        ans[k + n / 2] = y0[k] - w[k] * y1[k]; 
    } 
    delete[] y0;
    delete[] y1;
    pthread_exit(NULL);
} 
  
  
// Driver code 
int main() 
{ 
    	// Change N for polynomial degree of A and B power of 2
    	int N=4;
        cd *a = new cd[2*N];
        cd *b = new cd[2*N];
        cd *af = new cd[2*N];
        cd *bf = new cd[2*N];
        for(int i=0;i<N;++i)a[i] = 1.0;
        for(int i=0;i<N;++i)b[i] = 1.0;
        for(int i=N;i<2*N;++i){
            a[i] = b[i] = 0.0;
        }
        
        pthread_t th[3];
        struct data da(a,af,2*N);
        pthread_create(&th[0],NULL,fft,(void*)&da);
        struct data db(b,bf,2*N);
        pthread_create(&th[1],NULL,fft,(void*)&db);
        for(int i=0;i<2;++i){
            pthread_join(th[i],NULL);
        }

        delete[] a;
        delete[] b;
        for(int i=0;i<2*N;++i){
            af[i] = af[i]*bf[i];
        } 
        struct data dans(af,bf,2*N);
        pthread_create(&th[2],NULL,inv_fft,(void*)&dans);
        pthread_join(th[2],NULL);
        for(int i=0;i<2*N-1;++i){
            bf[i]/=(2*N);
        } 
        delete[] af;
        /** Print Polynomial by removing this comment
        for(int i=0;i<2*N-1;i++)
        {
        	cout<<bf[i].real()<<"  x^"<<i<<" + "<<endl;
        }**/
        delete[] bf;

} 