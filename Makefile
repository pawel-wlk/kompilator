all: before kompilator

before:
	mkdir bin

kompilator: bin/parser.o bin/lexer.o
	g++ -o $@ $^

bin/parser.o: bin/parser.tab.cpp
	g++ -c $^ -o $@

bin/lexer.o: bin/lexer.c
	g++ -c $^ -o $@

bin/parser.tab.cpp: src/parser.ypp
	bison -d $^ -o $@
    
bin/lexer.c: src/lexer.l
	flex -o $@ $^

cleanall: clean
	rm -f kompilator

clean:
	rm -rf bin


