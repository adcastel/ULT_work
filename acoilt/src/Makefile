

argobots:
	gcc -Wall -c -fPIC acoilt.c -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS
	gcc -shared -o libacoilt.so acoilt.o -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt

massive:
	gcc -Wall -c -fPIC acoilt.c -I/home/adcastel/massivethreads/include -L/home/adcastel/massivethreads/lib -labt -DMASSIVETHREADS
	gcc -shared -o libacoilt.so acoilt.o -I/home/adcastel/massivethreads/include -L/home/adcastel/massivethreads/lib -labt

