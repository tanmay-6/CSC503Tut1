CXX := g++
CXXFLAGS := -O2 -std=c++17
NODES := 100 200 300 400 500 600 700 800 900 1000
SPARSITIES := a b c d
ALGORITHMS := DFSI DFSR BFS
FLAGS := VARN VARM

BIN_DIR := bin
RESULTS := results.csv

$(shell mkdir -p $(BIN_DIR))

all: run

init:
	echo "flag,algorithm,param,time(s),memory(KB)" > $(RESULTS)

run: init
	@for flag in $(FLAGS); do \
	  for algo in $(ALGORITHMS); do \
	    if [ $$flag = "VARM" ]; then \
	      for n in $(NODES); do \
	        exe="$(BIN_DIR)/$${flag}_$${algo}_$${n}"; \
	        $(CXX) $(CXXFLAGS) -D$$flag -D$$algo main.cpp -o $$exe; \
	        /usr/bin/time -f "$$flag,$$algo,$$n,%e,%M" -o tmp.csv $$exe $$n >/dev/null; \
	        cat tmp.csv >> $(RESULTS); \
	      done; \
	    else \
	      for sp in $(SPARSITIES); do \
	        exe="$(BIN_DIR)/$${flag}_$${algo}_$${sp}"; \
	        $(CXX) $(CXXFLAGS) -D$$flag -D$$algo main.cpp -o $$exe; \
	        /usr/bin/time -f "$$flag,$$algo,$$sp,%e,%M" -o tmp.csv $$exe $$sp >/dev/null; \
	        cat tmp.csv >> $(RESULTS); \
	      done; \
	    fi; \
	  done; \
	done
	@rm -f tmp.csv
	@echo "All experiments complete. Results in $(RESULTS)"

clean:
	rm -rf $(BIN_DIR) $(RESULTS)
