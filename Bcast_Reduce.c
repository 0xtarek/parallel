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
    
    int rank, size, x, y, r, a, b, count, total_count;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter lower bound x: ");
        scanf("%d", &x);
        printf("Enter upper bound y: ");
        scanf("%d", &y);
    }

    start_time = MPI_Wtime();

    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);

    r = (y - x + 1) / size;
    a = x + rank * r;
    
    if(rank==(size-1)){
        b=y;
    }else{
        b = a + r - 1;
    }

    count = count_primes(a, b);
    printf("Total number of primes between %d and %d is %d\n", a, b, count);


    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Total number of primes between %d and %d is %d\n", x, y, total_count);
        printf("Elapsed time using MPI_Bcast and MPI_Reduce: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}