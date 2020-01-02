all: bin kompilator

bin:
	mkdir -p bin

kompilator: bin/parser.o bin/lexer.o bin/Memory.o bin/Variable.o bin/Constant.o bin/io.o
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

bin/io.o: src/io.cpp
	g++ -c $^ -o $@

cleanall: clean
	rm -f kompilator

clean:
	rm -rf bin


