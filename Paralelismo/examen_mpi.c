#include <unistd.h>
#include <stdio.h>
#include <mpi.h>

#define TAM4 4
#define TAM8 8

void print_vector(int v[], int n)
{
    printf(" [");
    for (int i = 0; i < n; i++)
    {
        printf("%2d", v[i]);
        if (i != n - 1)
            printf(",");
    }
    printf("]");
}

void ex4var()
{
    int a[TAM4], b[TAM4], suma;
    int numprocs, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < numprocs; i++)
    {
        if (rank == i)
        {
            a[0] = rank;
            a[1] = rank;
            a[2] = rank;
            a[3] = rank;

            b[0] = rank;
            b[1] = rank;
            b[2] = rank;
            b[3] = rank;

            suma = rank * numprocs;
        }
    }

    // sleep del proceso para que el enunciado salga "medianamente" ordenados
    sleep(rank);

    printf("P%d -> A =", rank);
    print_vector(a, TAM4);
    printf(" | B =");
    print_vector(b, TAM4);
    printf(" | suma = %d\n", suma);

    if (rank == (numprocs - 1))
        printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // Operaciones ejercicio:
    MPI_Reduce(a, b, 2, MPI_INT, MPI_SUM, 3, MPI_COMM_WORLD);
    MPI_Bcast(&b[1], 1, MPI_INT, 3, MPI_COMM_WORLD);
    //MPI_Scatter(a, 1, MPI_INT, &b[2], 1, MPI_INT, 3, MPI_COMM_WORLD);
    //MPI_Gather(a, 1, MPI_INT, b, 1, MPI_INT, 3, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // sleep del proceso para que el resultado salga "medianamente" ordenados
    sleep(rank);

    printf("P%d -> A =", rank);
    print_vector(a, TAM4);
    printf(" | B =");
    print_vector(b, TAM4);
    printf(" | suma = %d\n", suma);

    MPI_Barrier(MPI_COMM_WORLD);
}

void ex8var()
{
    int a[TAM8], b[TAM8];
    int numprocs, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < numprocs; i++)
    {
        if (rank == i)
        {
            a[0] = rank;
            a[1] = rank;
            a[2] = rank;
            a[3] = rank;
            a[4] = rank + numprocs;
            a[5] = rank + numprocs;
            a[6] = rank + numprocs;
            a[7] = rank + numprocs;

            b[0] = 0;
            b[1] = 0;
            b[2] = 0;
            b[3] = 0;
            b[4] = 0;
            b[5] = 0;
            b[6] = 0;
            b[7] = 0;
        }
    }

    // sleep del proceso para que los printfs salgan "medianamente" ordenados
    sleep(rank);

    printf("P%d -> A =", rank);
    print_vector(a, TAM8);
    printf(" | B =");
    print_vector(b, TAM8);
    printf("\n");

    if (rank == (numprocs - 1))
        printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // Operaciones ejercicio:
    //MPI_Bcast(a, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Reduce(a, b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //MPI_Scatter(a, 1, MPI_INT, b, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Gather(a, 1, MPI_INT, b, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    sleep(rank);

    printf("P%d -> A =", rank);
    print_vector(a, TAM8);
    printf(" | B =");
    print_vector(b, TAM8);
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    ex4var();
    //ex8var();

    MPI_Finalize();
    return 0;
}
