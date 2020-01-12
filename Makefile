CC = g++ -Wall -pedantic

all: bin kompilator

bin:
	mkdir -p bin

kompilator: bin/parser.o bin/lexer.o bin/Memory.o bin/Variable.o bin/Constant.o bin/Code.o bin/Operation.o bin/labels.o
	$(CC) -o $@ $^

bin/parser.o: bin/parser.tab.cpp
	$(CC) -c $^ -o $@

bin/lexer.o: bin/lexer.c
	$(CC) -c $^ -o $@

bin/parser.tab.cpp: src/parser.ypp
	bison -d $^ -o $@
    
bin/lexer.c: src/lexer.l
	flex -o $@ $^

bin/Memory.o: src/Memory.cpp
	$(CC) -c $^ -o $@

bin/Variable.o: src/Variable.cpp
	$(CC) -c $^ -o $@

bin/Constant.o: src/Constant.cpp
	$(CC) -c $^ -o $@

bin/Code.o: src/Code.cpp
	$(CC) -c $^ -o $@

bin/Operation.o: src/Operation.cpp
	$(CC) -c $^ -o $@

bin/labels.o: src/labels.cpp
	$(CC) -c $^ -o $@

cleanall: clean
	rm -f kompilator

clean:
	rm -rf bin


