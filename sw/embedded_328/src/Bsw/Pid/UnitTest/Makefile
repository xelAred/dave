all: comp run

comp:
	gcc -g -c ../Pid.c -I../ -I../../../../inc -o pid.o
	gcc -g -c PidTest.c -I../ -I../../../../inc -o pidtest.o
	gcc pid.o pidtest.o -o PidTest

run:
	./PidTest > gnuplot.csv
	gnuplot -c gnuplot.script

clean:
	rm *.o PidTest gnuplot.csv
