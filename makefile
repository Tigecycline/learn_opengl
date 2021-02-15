main.out: main.o shader.o stb_image.o
	g++ -Wall -pedantic -lglfw -lGLEW -lGL main.o shader.o stb_image.o -o main.out

main.o: shader.h matrix.h camera.h main.cpp 
	g++ -Wall -pedantic -c main.cpp

shader.o: shader.h shader.cpp
	g++ -Wall -pedantic -c shader.cpp

stb_image.o: stb_image.h stb_image.cpp
	g++ -Wall -pedantic -c stb_image.cpp

clean: 
	@rm *.o main.out