clear
rm examen_mpi
mpicc examen_mpi.c -o examen_mpi -Wall
mpirun --oversubscribe -np 4 ./examen_mpi