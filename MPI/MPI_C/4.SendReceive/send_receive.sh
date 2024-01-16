mpicc send_receive.c -o send_receive.exe
mpiexec -n 4 ./send_receive.exe