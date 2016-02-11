

argobots:
	gcc -Wall -c -fPIC acoilt.c -I. -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS
	gcc -shared -o libacoilt.so acoilt.o -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt

massive:
	gcc -Wall -c -fPIC acoilt.c -I. -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -DMASSIVETHREADS
	gcc -shared -o libacoilt.so acoilt.o -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth

qthreads:
	gcc -Wall -c -fPIC acoilt.c -I. -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread -DQTHREADS
	gcc -shared -o libacoilt.so acoilt.o -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread