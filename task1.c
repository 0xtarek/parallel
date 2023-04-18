/*
name stud 1 : Yahia Hasan Ewas , ID :20200637
name stud 2 : Nada Arafat , ID:20200588
Section: S8 .
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, numprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    int n, *arr, max_val, max_index, slave_max_val, slave_max_index;
    if (rank == 0) { 
        printf("Hello from master process.\n");
        printf("Number of slave processes is %d\n", numprocs - 1);
        printf("Please enter size of array...\n");
        fflush(stdout);
        scanf("%d", &n);
        arr = (int*) malloc(n * sizeof(int));
        printf("Please enter array elements ...\n");
		int p;
        for ( p = 0; p < n; p++) {
            scanf("%d", &arr[p]);
        }
        int i;
		for (i = 1; i < numprocs; i++) {
            int start = (i - 1) * (n / (numprocs - 1));
			int end ;
			if(i==numprocs-1){
				end=n;
			}else{
				end=i*(n/(numprocs-1));
			}
            int size = end - start;
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&arr[start], size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        max_val = arr[0];
        max_index = 0;
		int j;
        for ( j = 1; j < numprocs; j++) {
            MPI_Recv(&slave_max_val, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&slave_max_index, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (slave_max_val > max_val) {
                max_val = slave_max_val;
            }
			int h ;
			for(h=0;h<n;h++){
				if(arr[h]==max_val){
					max_index=h;
					break;
				}
			}
            printf("Max number from slave #%d is %d and its index is %d.\n", j, slave_max_val, slave_max_index);
        }
        printf("Master process announces the final max which is %d and its index is %d.\n", max_val, max_index);
        free(arr);
    } else { 
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int size;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int* sub_arr = (int*) malloc(size * sizeof(int));
        MPI_Recv(sub_arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        slave_max_val = sub_arr[0];
        slave_max_index = 0;
		int f;
        for (f = 1; f < size; f++) {
            if (sub_arr[f] > slave_max_val) {
                slave_max_val = sub_arr[f];
                slave_max_index = f;
            }
        }
        MPI_Send(&slave_max_val,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(&slave_max_index,1,MPI_INT,0,0,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
