#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.14159265358979

int main(void) {
	double pi, x, y, P, error, error1;
	int count, N, i, end;
	float rx, ry;

	printf("Enter number of random points:");
	scanf("%d",&N);

	count = 0; 
	srand((unsigned)time(NULL)); //taking seed from time 
	for(i=0;i<N;i++)
	{
	
	
	 rx=rand() %100001;
	 x = rx*0.00001;

	 ry=rand() %100001;
	 y = ry*0.00001;

	 P = (x*x) + (y*y);

	 if (P < 1) {
		 count = count + 1;
	 }


	 }

	printf("%d\n",count);


	pi = 4.0*count*(1.0f/N);

	error = fabs(PI-pi);  // checks the absolute error
	error1 = error*100/PI;



	printf("Value obtained of 'pi' with Monte Carlo method is:%f\n", pi);
	printf("The absolute error is:%f\n", error);
	printf("The percentage error in value of pi is:%f\n", error1);
	
	//printf("END?");
	//scanf("%d",&end);
	
	return 0;
}
