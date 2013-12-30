#include "mpi.h"
#include "stdio.h"
//Parallel Processing Computing
/*Supposing there are N number of integers, 
foo(i) does the computation of i*i, 
then all these results are accumulated into a total sum
*/
int foo(int i)
{
    return i*i;
}
int main( int argc, char *argv[] )
{
    int i, tmp, sum=0, group_size, my_rank, N,c;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &group_size );
    MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );
    if (my_rank==0)//What process O performs
    {
        printf( "N=" );
        scanf( "%d", &N );
        for (c=1; c<group_size; c++)
        {
S1:
            MPI_Send( &N, 1, MPI_INT, c, c, MPI_COMM_WORLD);
		//Send partitions to other processes
        }
        for (i=my_rank; i<=N; i+=group_size)
        {
            sum += foo(i);
        }
S2:
        for (i=1; i<group_size; i++)
        {
            MPI_Recv( &tmp, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status );
            sum += tmp;
		//Receive results from each partition and aggregate results
        }
	system("clear");//Clear command line in Unix use cls for windows operating system
        printf ("\nThe result=%d\n", sum );
    }
    else//What other processes perform
    {

S3:
        MPI_Recv( &N, 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD,&status );
        for (i=my_rank; i<=N; i+=group_size)
        {
            sum += foo(i);
        }
        printf("Temporary sum = %d", sum);
S4:	
        MPI_Send( &sum, 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD );
	//Send result to process 0
    }
    MPI_Finalize();
    return 0;
}
