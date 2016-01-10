.SUFFIXES:
.PHONY:    pc,pc2,f,clean,pcfinal, filcam

compilador := mpicxx
flagsc     := -g -Wall 

pc: prodcons
	mpirun -np  3 ./prodcons

pc2: prodcons2
	mpirun -np  3 ./prodcons2

f: filosofos
	mpirun -np 10 ./filosofos

pcfinal: prodcons_final
	mpirun -np 10 ./prodcons_final

filcam: filosofos_con_camarero
	mpirun -np 11 ./filosofos_con_camarero

%: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -f prodcons prodcons2 filosofos prodcons_final filosofos_con_camarero
