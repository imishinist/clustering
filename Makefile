main:
	g++ main.cpp -o bin/main -std=c++11 -O2

gen:
	g++ gen.cpp -o bin/gen -std=c++11 -O2

plot:
	g++ plot.cpp -o bin/plot -std=c++11 -O2

clean:
	rm bin/*
