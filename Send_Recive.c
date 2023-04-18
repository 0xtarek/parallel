#include <stdio.h>
#include <mpi.h>
#include <math.h>

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    int i;
    for ( i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}
int count_primes(int x, int y) {
    int count = 0;
    int i ;
    for (i = x; i <= y; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    int rank, size, source=0, dest, x, y, r, a, b, count, total_count=0;
    double start_time, end_time;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank==0)
    {
        printf("Enter lower bound x: ");
        scanf("%d", &x);
        printf("Enter upper bound y: ");
        scanf("%d", &y);

        r = (y-x+1)/(size-1);

        for(dest=1;dest<size;dest++)
        {
            a = x + (dest-1) * r;
            if(dest==(size-1))
            {
                b=y;
            }
            else
            {
                b = a + r - 1;
            }
            MPI_Send(&a, 1, MPI_INT, dest, 0,MPI_COMM_WORLD);
            MPI_Send(&b, 1, MPI_INT, dest, 0,MPI_COMM_WORLD);
        }

        int i;
        for(i=1;i<size;i++)
        {
            MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count+=count;
        }

        end_time = MPI_Wtime();

        printf("Total number of primes between %d and %d is %d\n", x, y, total_count);
        printf("Elapsed time using MPI_Send and MPI_Recv: %f seconds\n", end_time - start_time);

    }
    else if(rank!=0)
    {
        MPI_Recv(&a, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&b, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        count = count_primes(a, b);
        printf("Total number of primes between %d and %d is %d\n", a, b, count);

        MPI_Send(&count, 1, MPI_INT, source, 0,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}