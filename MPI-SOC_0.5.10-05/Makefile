mpi-soc-init: mpi-soc-init.c soc-init_options.c *.h
	echo "#define MPI" > def_mpi.h
	mpicc -pc mpi-soc-init.c -o mpi-soc-init

mpicc: 
	echo "#define MPI" > def_mpi.h
	mpicc -pc mpi-soc-init.c -o mpi-soc-init

gcc:
	echo "//#define MPI" > def_mpi.h
	gcc -lm mpi-soc-init.c -o mpi-soc-init
