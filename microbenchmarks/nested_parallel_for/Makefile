

OBJ=nested_for
DEB= -g -DCORE

all: nested_for fast_nested_for 


nested_for:
	gcc $(OBJ).c -o $(OBJ)_mth -I/home/adcastel/GLT/src/massivethreads/ -L/home/adcastel/GLT/src/massivethreads/ -lglt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm $(DEB)
	gcc $(OBJ).c -o $(OBJ)_qth -I/home/adcastel/GLT/src/qthreads/ -L/home/adcastel/GLT/src/qthreads/ -lglt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -lm $(DEB)
	gcc $(OBJ).c -o $(OBJ)_arg -I/home/adcastel/GLT/src/argobots/ -L/home/adcastel/GLT/src/argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -lm $(DEB)
	gcc $(OBJ).c -o $(OBJ)_arg_task -I/home/adcastel/GLT/src/argobots/ -L/home/adcastel/GLT/src/argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -lm -DTASK $(DEB)

fast_nested_for:
	gcc $(OBJ).c -o $(OBJ)_fast_mth -I/home/adcastel/GLT/src/massivethreads/ -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm -DFASTGLT $(DEB)
	gcc $(OBJ).c -o $(OBJ)_fast_qth -I/home/adcastel/GLT/src/qthreads/ -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -lm -DFASTGLT $(DEB)
	gcc $(OBJ).c -o $(OBJ)_fast_arg -I/home/adcastel/GLT/src/argobots/ -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -lm -DFASTGLT $(DEB)
	gcc $(OBJ).c -o $(OBJ)_fast_arg_task -I/home/adcastel/GLT/src/argobots/ -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -lm -DTASK -DFASTGLT $(DEB)

clean: 
	rm *.o $(OBJ)_mth $(OBJ)_qth $(OBJ)_arg* *~
