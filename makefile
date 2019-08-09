main.out: main.o shader.o
	g++ -Wall -pedantic -lglfw -lGLEW -lGL main.o shader.o -o main.out

main.o: shader.h main.cpp 
	g++ -Wall -pedantic -c main.cpp

shader.o: shader.h shader.cpp triangle.vert triangle.frag
	g++ -Wall -pedantic -c shader.cpp

clean: 
	@rm *.o main.out