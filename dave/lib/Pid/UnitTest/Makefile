all: comp run

comp:
	g++ -g -c ../Pid.cpp -I../ -o pid.o
	g++ -g -c PidTest.cpp -I../ -o pidtest.o
	g++ pid.o pidtest.o -o PidTest

run:
	./PidTest > gnuplot.csv
	gnuplot -p -c gnuplot.script

clean:
	rm *.o PidTest gnuplot.csv
