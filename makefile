all:	compile

compile:	ebf.cpp ebf.h
		g++ ebf.cpp -o ebf

run:	compile
		./ebf

clean:
		rm ebf 
