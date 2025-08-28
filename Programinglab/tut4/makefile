N = 1000000
M = 1 2 3 4 5 6 7 8
Ns = 10 100 1000 5000 10000 25000 50000 75000 100000
Ms = 4
CSV1 = dfm.csv
CSV2 = dfn.csv
SRC = main.cpp
BIN = main

build: $(SRC)
	g++ $(SRC) -o $(BIN) -lpthread

run: build
	@rm -f $(CSV1)
	@echo "M,time" > $(CSV1)   # CSV header
	for m in $(M); do \
		echo "Running M=$$m"; \
		/usr/bin/time -f "$$m,%e" ./$(BIN) $(N) $$m 2>> $(CSV1); \
	done
	@echo "Results logged in $(CSV1)"

	@rm -f $(CSV2)
	@echo "N,time" > $(CSV2)
	for n in $(Ns); do \
		echo "Running N=$$n"; \
		/usr/bin/time -f "$$n,%e" ./$(BIN) $$n $(Ms) 2>> $(CSV2); \
	done
	@echo "Results logged in $(CSV2)"

plot : run
	@gnuplot plot.plt
	@echo "Plot saved to dfm.png"

clean:
	@rm -f $(CSV1) $(BIN)
