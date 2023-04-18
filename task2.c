/*
name 1 : Mohamed Tarek , ID :20200453
name 2 : Manar , post-graduate program
Section: S5 .
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int my_rank;       /* rank of process	*/
    int p;             /* number of process	*/
    int source;        /* rank of sender	*/
    int dest;          /* rank of reciever	*/
    int tag = 0;       /* tag for messages	*/
    char message[100]; 

    MPI_Status status; 
                    

    
    MPI_Init(&argc, &argv);

    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // master
    if (my_rank == 0)
    {
        int **matrix1;
        int **matrix2;

        int option;
        printf("Welcome to vector Matrix multiplication program! \n \n");
        printf("To read dimensions and values from file press 1 \n");
        printf("To read dimensions and values from console press 2 \n");

        fflush(stdout);
        scanf("%d", &option);

        int m1r, m1c, m2r, m2c;

        if (option == 1)
        {
            FILE *fptr;
            fptr = fopen("myf.txt", "r");
            fscanf(fptr, "%d%d%d%d", &m1r, &m1c, &m2r, &m2c);
            matrix1 = (int **)malloc(m1r * sizeof(int *));
            int i1;
            for ( i1 = 0; i1 < m1r; i1++)
            {
                matrix1[i1] = (int *)malloc(m1c * sizeof(int));
            }
            matrix2 = (int **)malloc(m2r * sizeof(int *));
            int i2;
            for ( i2 = 0; i2 < m2r; i2++)
            {
                matrix2[i2] = (int *)malloc(m2c * sizeof(int));
            }

            fclose(fptr);
            if (m1c == m2r)
            {
                fptr = fopen("myf.txt", "r");
                int i3,j3;
                for ( i3 = 0; i3 < m1r; i3++)
                {
                    for ( j3 = 0; j3 < m1c; j3++)
                        fscanf(fptr, "\n%d", &matrix1[i3][j3]);
                }
                int i4,j4;
                for ( i4 = 0; i4 < m2r; i4++)
                {
                    for ( j4 = 0; j4 < m2c; j4++)
                        fscanf(fptr, "\n%d", &matrix2[i4][j4]);
                }
                fclose(fptr);
            }
        }

        else if (option == 2)
        {
            printf("Please enter dimensions of the first matrix: ");
            fflush(stdout);
            scanf("%d %d", &m1r, &m1c);

            matrix1 = (int **)malloc(m1r * sizeof(int *));
            int i5;
            for (i5 = 0; i5 < m1r; i5++)
            {
                matrix1[i5] = (int *)malloc(m1c * sizeof(int));
            }

            printf("Please enter its elements: ");
            printf("\n");
            fflush(stdout);
            int i6,j6;
            for ( i6 = 0; i6 < m1r; i6++)
            {
                for ( j6 = 0; j6 < m1c; j6++)
                {
                    scanf("%d", &matrix1[i6][j6]);
                }
            }
            printf("\n");

            printf("Please enter dimensions of the second matrix: ");
            fflush(stdout);
            scanf("%d %d", &m2r, &m2c);

            matrix2 = (int **)malloc(m2r * sizeof(int *));
            int i7;
            for ( i7 = 0; i7 < m2r; i7++)
            {
                matrix2[i7] = (int *)malloc(m2c * sizeof(int));
            }

            if (m1c != m2r)
            {
                printf("These Two Matrices Can't be Multiplied   \n");
                printf("Please Exit the Program And Run it Again \n");
            }

            printf("Please enter its elements: ");
            printf("\n");
            fflush(stdout);
            int i8,j8;
            for ( i8 = 0; i8 < m2r; i8++)
            {
                for ( j8 = 0; j8 < m2c; j8++)
                {
                    scanf("%d", &matrix2[i8][j8]);
                }
            }
            printf("\n");
        }
        else
        {
            printf("Error not a Vaild Option");
        }

        for (dest = 1; dest < p; dest++)
        {
            MPI_Send(&m1r, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&m1c, 1, MPI_INT, dest, 2, MPI_COMM_WORLD);
            MPI_Send(&m2r, 1, MPI_INT, dest, 3, MPI_COMM_WORLD);
            MPI_Send(&m2c, 1, MPI_INT, dest, 4, MPI_COMM_WORLD);
        }

        for (dest = 1; dest < p; dest++)
        {
            int j9;
            for ( j9 = 0; j9 < m1r; j9++)
            {
                MPI_Send(matrix1[j9], m1c, MPI_INT, dest, tag, MPI_COMM_WORLD);
            }
            int j10;
            for ( j10 = 0; j10 < m2r; j10++)
            {
                MPI_Send(matrix2[j10], m2c, MPI_INT, dest, tag, MPI_COMM_WORLD);
            }
        }

        int matrix3[m1r][m2c];
        int temp;

        
        for (source = 1; source < p; source++)
        {
            int i11,j11;
            for ( i11 = 0; i11 < m1r; i11++)
            {
                for ( j11 = 0; j11 < m2c; j11++)
                {
                    MPI_Recv(&temp, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
                    matrix3[i11][j11] = temp;
                }
            }
            printf("Recived Matrix Succsefully \n");
        }

        
        sprintf(message, "Result Matrix is ( %d x %d ): \n", m1r, m2c);
        printf("%s", message);
        int i12,j12;
        for ( i12 = 0; i12 < m1r; ++i12)
        {
            for ( j12 = 0; j12 < m2c; ++j12)
            {
                printf("%d ", matrix3[i12][j12]);
            }
            printf("\n");
        }

        printf("\n");

        free(matrix1);
        free(matrix2);
    }

    
    else if (my_rank != 0)
    {
        int m1r, m1c, m2r, m2c;
        int **matrix1;
        int **matrix2;

     
        MPI_Recv(&m1r, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&m1c, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&m2r, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&m2c, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);

        matrix1 = (int **)malloc(m1r * sizeof(int *));
        int i12;
        for (i12 = 0; i12 < m1r; i12++)
        {
            matrix1[i12] = (int *)malloc(m1c * sizeof(int));
        }

        matrix2 = (int **)malloc(m2r * sizeof(int *));
        int i13;
        for ( i13 = 0; i13 < m2r; i13++)
        {
            matrix2[i13] = (int *)malloc(m2c * sizeof(int));
        }

        int j14;
        for ( j14 = 0; j14 < m1r; j14++)
        {
            MPI_Recv(matrix1[j14], m1c, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        int j15;
        for ( j15 = 0; j15 < m2r; j15++)
        {
            MPI_Recv(matrix2[j15], m2c, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        int matrix3[m1r][m2c];
        int i16,j16,k;
        for ( i16 = 0; i16 < m1r; ++i16)
        {
            for ( j16 = 0; j16 < m2c; ++j16)
            {
                matrix3[i16][j16] = 0;
                for ( k = 0; k < m1c; k++)
                {
                    matrix3[i16][j16] += (matrix1[i16][k] * matrix2[k][j16]);
                }
            }
        }

        int temp;
        int i17,j17;
        for ( i17 = 0; i17 < m1r; ++i17)
        {
            for ( j17 = 0; j17 < m2c; ++j17)
            {
                temp = matrix3[i17][j17];
                MPI_Send(&temp, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            }
        }

        free(matrix1);
        free(matrix2);
    }

    MPI_Finalize();

    return 0;
}
