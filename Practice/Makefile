FILE := demo.txt
compile : 
	g++ demo.cpp -o demo

run : compile
	@echo "Running..."
	/usr/bin/time -f "%U %M" ./demo 2>>$(FILE)

plot :
	@gnuplot learnplot.plt 