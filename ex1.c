#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <time.h>
float Q_rsqrt( float number ) // number = x*x + y*y + z*z
{
        int i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y  = number;
        i  = * ( int * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the ****?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

        return y;
}
float mh(int i){
	return 1/sqrtf(i);
}
void main() {
    int i;
    double time_spent;
    double time;
    double t[100];
    float d[100];
    float a[100];
    float b[100];
    for (i=1;i<101;i++){
    	clock_t begin=clock();
    	a[i-1]=Q_rsqrt(i);
    	clock_t end = clock();
    	time=(double)(end - begin) / CLOCKS_PER_SEC;
    	begin = clock();
    	b[i-1]=mh(i);
    	end = clock();
    	time_spent=(double)(end - begin) / CLOCKS_PER_SEC;
    	t[i-1]=time_spent/time;
    	d[i-1]=b[i-1]-a[i-1];
    }
}
