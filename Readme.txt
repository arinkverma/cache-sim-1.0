Readme-Project211
==================

installation
***************
open terminal

1.$cd <present directory>
2.$make

expected output
---------------
	g++    -c -o class.o class.cpp
	class.cpp:24: warning: this decimal constant is unsigned only in ISO C90
	g++ -Wall -c -g main.cpp
	g++ -Wall -g main.o class.o -o cache-sim

3.$make clean

expected output
---------------
	\rm *.o


To make backup copy
4.$make tar
expected output
---------------
	tar cfv cache-sim-1.0.tar class.h class.cpp utility.h typedef.h main.cpp Makefile Readme.txt
	class.h
	class.cpp
	utility.h
	typedef.htar cfv cache-sim-1.0.tar class.h class.cpp utility.h typedef.h main.cpp Makefile Readme.txt
	class.h
	class.cpp
	utility.h
	typedef.h
	main.cpp
	Makefile
	Readme.txt

Done!



