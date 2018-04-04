gcc applicationProcess.c queue.c testLib.c -o hash
gcc slaveProcess.c -o slaveProcess
./hash ./*
