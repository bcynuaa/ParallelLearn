mpicc send_receive.c -o send_receive.exe
mpiexec -n 10 ./send_receive.exe