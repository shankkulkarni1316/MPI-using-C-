#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"
#define PI 3.14159265358979

int main(int argc, char *argv[]) {
	double pi, x, y, P1, error, error1;
	int count[2], N, i, j, end, tcount, S, totalcount[2], ftotalcount;
	float rx, ry;
	double time1, time2; 		// for timing
	int S1[100], SS1, k, T, Tt, r, l;
	char hostname[256]; // to get host name

	MPI_Status status; 		// MPI variables
	MPI_Request req1;
	int rank, P;
	MPI_Init(&argc, &argv);  		// Start MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	gethostname(hostname,255);    //getting computers names



	// Names of master and slaves
	if (rank == 0) {
		printf("Master is: %s\n", hostname);
		fflush( stdout );
		
	}
	else {
		printf("%s is slave.\n", hostname);
		fflush( stdout );
	}

	MPI_Barrier(MPI_COMM_WORLD); //Barrier inserted so that all processes wait here

	if (rank == 0 ) {
		printf("Enter number of seeds 'S':");
		fflush( stdout ); 
		scanf("%d",&S);
		printf("Enter number of random points 'N':");
		fflush( stdout );
		scanf("%d",&N);
		

		srand((unsigned)time(NULL)); //taking seed from time
	}
	
	
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD); //broadcast Number of points
	MPI_Bcast(&S, 1, MPI_INT, 0, MPI_COMM_WORLD); //broadcast Number of points

	if (rank == 0 ) {
		ftotalcount = 0;
		k = 1;
		T = 100;
		r = 1;
		for (i=0;i<S;i++) {
			
			S1[i] = rand() %1000;
		
			//send seed SS1
			if (i == 0) {    // Initially sending seed to all slaves 
				time1 = MPI_Wtime();  	// start time
				for (l=1;l<P;l++) {
					MPI_Send(&S1[i],1,MPI_INT,l,0,MPI_COMM_WORLD);
				}
			}
			else {		// next seed is send to only that slave which finishes the job first
				MPI_Send(&S1[i],1,MPI_INT,k,0,MPI_COMM_WORLD);
			}
			//receive the count
			MPI_Recv(&totalcount,2,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
			ftotalcount = ftotalcount + totalcount[0];
			k = totalcount[1];

			if (i == (S-1)) {	//termination notice
				time2 = MPI_Wtime();  	// end time
				for (r=1;r<P;r++) {
					MPI_Isend(&T,1,MPI_INT,r,90,MPI_COMM_WORLD, &req1);
					MPI_Send(&S1[i],1,MPI_INT,r,0,MPI_COMM_WORLD);
					
				}
			}
		}
		


	}
	else 	{
		for (j=0;j<S;j++) {

			// termination notice
			MPI_Irecv(&Tt,1,MPI_INT,0,90,MPI_COMM_WORLD, &req1);
			//printf("received %d\n",Tt);
			if (Tt==100) {
				//break;
				printf("Process with rank %d exiting...in between\n", rank);
				MPI_Finalize();
				return 0;
			}

			// Receive seed SS1
			MPI_Recv(&SS1,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
			srand(SS1);
			count[0] = 0;
			count[1] = rank; 
			for(i=0;i<N;i++) {
	
				 rx=rand() %100001;
				 x = rx*0.00001;
	
				 ry=rand() %100001;
				 y = ry*0.00001;

				 P1 = (x*x) + (y*y);

				 if (P1 < 1) {
					 count[0] = count[0] + 1;
				 }


			}
			// send count
			MPI_Send(count,2,MPI_INT,0,0,MPI_COMM_WORLD);
		}
	}



	if (rank == 0) {
		

		pi = 4.0*ftotalcount*(1.0f/(N*S));  // calculation of pi by master here

		error = fabs(PI-pi);  // checks the absolute error
		error1 = error*100/PI;



		printf("Value obtained of 'pi' with Monte Carlo method is:%f\n", pi);
		printf("The absolute error is:%f\n", error);
		printf("The percentage error in value of pi is:%f\n", error1);
		printf("elapsed_time parallel = %f (seconds)\n", time2 - time1); // print out execution time

		
	}
		
	printf("Process with rank %d exiting...at last\n", rank);
	MPI_Finalize();
	return 0;
	
}
