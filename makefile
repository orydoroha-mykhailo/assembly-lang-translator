.PHONY: build clean

build: run

rebuild:
	make clean
	make build

clean:
	rm -f *.o run

run: main.o lexer.o translator.o analysis.o
	g++ -std=gnu++17 main.o lexer.o translator.o analysis.o -o run

main.o: main.cpp translator.h analysis.h lexer.h dictionary.h
	g++ -std=gnu++17 -c main.cpp

lexer.o: lexer.cpp lexer.h dictionary.h
	g++ -std=gnu++17 -c lexer.cpp

translator.o: translator.cpp translator.h analysis.h lexer.h dictionary.h
	g++ -std=gnu++17 -c translator.cpp

analysis.o: analysis.cpp analysis.h lexer.h dictionary.h
	g++ -std=gnu++17 -c analysis.cpp
