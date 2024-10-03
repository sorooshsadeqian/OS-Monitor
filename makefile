all: main.o classes.o
	g++ -std=c++11 main.o classes.o -lpthread -lrt -o a.out 
	mkdir -p car_data
main.o: main.cpp classes.o
	g++ -std=c++11 -c main.cpp -o main.o

classes.o: classes.cpp
	g++ -std=c++11 -c classes.cpp -o classes.o

clean:
	rm -f *.o *.out
	rm -rf car_data
