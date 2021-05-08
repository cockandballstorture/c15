#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#define DATA_SIZE 1000

typedef struct Result {
	clock_t t1;
	clock_t t2;
	size_t len;
	float *accuracy;
	float time_ratio;
} Res;

float Q_rsqrt( float number ) { // number = x*x + y*y + z*z
    int i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( int * ) &y;                        // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the ****?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//   y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

float rsqrtf( float number ) {
	return 1/sqrtf(number);
}

void compare(float *array, size_t len, float (*f1) (float), float (*f2) (float), Res* result) {
	clock_t begin, end;
	float *dummy1 = (float*) malloc (len * sizeof(float));
	float *dummy2 = (float*) malloc (len * sizeof(float));
	result->len = len;

	begin = clock();
	for (size_t i = 0; i < len; i++) {
		dummy1[i] = f1(array[i]);
	}
	end = clock();
	result->t1 = end - begin;

	begin = clock();
	for (size_t i = 0; i < len; i++) {
		dummy2[i] = f2(array[i]);
	}
	end = clock();
	result->t2 = end - begin;

	result->time_ratio = (float) result->t1 / result->t2;

	for (size_t i = 0; i < len; i++) {
		(result->accuracy)[i] = dummy2[i] - dummy1[i];
	}
	free(dummy1);
	free(dummy2);
}


void main() {
	float x_arr[DATA_SIZE];
	float d[DATA_SIZE];
	FILE *w;
	char name[10]="res.csv";
	w=fopen(name,"w+");
	fprintf(w,"x,abs_accuracy");
	for(int i = 1; i <= DATA_SIZE; i++) {
        x_arr[i-1] = i;
	}
	Res origin;
	Res* result = &origin;
	result->accuracy = (float*) malloc (DATA_SIZE * sizeof(float));
	compare(x_arr, DATA_SIZE, Q_rsqrt, rsqrtf, result);
	for (int i = 0; i < DATA_SIZE; i++){
		d[i]=result->accuracy[i];
		fprintf(w,"\n%d,%f",i+1,d[i]);
	}
	printf("t1/t2:%f\n", result->time_ratio);
	fclose(w);

	free(result->accuracy);
}
