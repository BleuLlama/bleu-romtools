@echo off
cd %1
make -DCC=tcc -DCFLAGS=-O -DDEBUG=-M -DMEDIUM=-mm -DE=.exe -DO=.obj "-DOUT=-e$<" -DG=getopt.obj %2
cd ..
