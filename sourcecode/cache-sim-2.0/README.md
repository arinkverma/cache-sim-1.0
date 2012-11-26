Readme-Project211
=========

installation
***************
open terminal

1.$cd <present directory>
2.$make

expected output
---------------
<pre>
	g++    -c -o class.o class.cpp
	class.cpp:24: warning: this decimal constant is unsigned only in ISO C90
	g++ -Wall -c -g main.cpp
	g++ -Wall -g main.o class.o -o cache-sim
</pre>
3.$make clean

expected output
---------------
<pre>
	\rm *.o
</pre>


To make backup copy
4.$make tar
expected output
---------------
<pre>
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
</pre>
Done!


Workload Generator
***************
Arguments:
-seq < length = % of ins * repeat ,:... >
-ins < total # of instruction >
-load < approx % of load instruction >
Example : ./workgen -seq 10:20*2:10:20*2 -ins 100000 -load 20
Address trace will have 10000 instructions then 20000 instructions in a loop 2 times. Again 10000 instructions then 20000 instructions in a loop 2 times. And load instructions approx 20 %.


Cache Simulator
***************
Arguments:
-cap < capacity in kbytes >
-block < size of block in (8,16,32,64) >
-assoc < associativity in (1,2,4,8) >
-out < standard output to file >
Example : ./cache-sim -cap 2 -block 32 -assoc 4 -out output tracefile



