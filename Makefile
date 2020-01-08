all: bin kompilator

bin:
	mkdir -p bin

kompilator: bin/parser.o bin/lexer.o bin/Memory.o bin/Variable.o bin/Constant.o bin/Code.o bin/Operation.o bin/labels.o
	g++ -o $@ $^

bin/parser.o: bin/parser.tab.cpp
	g++ -c $^ -o $@

bin/lexer.o: bin/lexer.c
	g++ -c $^ -o $@

bin/parser.tab.cpp: src/parser.ypp
	bison -d $^ -o $@
    
bin/lexer.c: src/lexer.l
	flex -o $@ $^

bin/Memory.o: src/Memory.cpp
	g++ -c $^ -o $@

bin/Variable.o: src/Variable.cpp
	g++ -c $^ -o $@

bin/Constant.o: src/Constant.cpp
	g++ -c $^ -o $@

bin/Code.o: src/Code.cpp
	g++ -c $^ -o $@

bin/Operation.o: src/Operation.cpp
	g++ -c $^ -o $@

bin/labels.o: src/labels.cpp
	g++ -c $^ -o $@

cleanall: clean
	rm -f kompilator

clean:
	rm -rf bin


