.SUFFIXES:
.PHONY:    pc,pc2,f,clean

compilador := mpicxx
flagsc     := -g -Wall 

pc: prodcons
	mpirun -np  3 ./prodcons

pc2: prodcons2
	mpirun -np  3 ./prodcons2

f: filosofos
	mpirun -np 10 ./filosofos

%: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -f prodcons prodcons2 filosofos
