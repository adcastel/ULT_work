

OBJ=task
GLT_HOME=/home/adcastel/GLT/src/

all: $(OBJ) fast_task 


task:
	gcc $(OBJ).c -o $(OBJ)_mth -I$(GLT_HOME)/massivethreads -L$(GLT_HOME)/massivethreads/ -lglt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm
	gcc $(OBJ).c -o $(OBJ)_qth -I$(GLT_HOME)/qthreads -L$(GLT_HOME)/qthreads/ -lglt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread -lm
	gcc $(OBJ).c -o $(OBJ)_arg -I$(GLT_HOME)/argobots -L$(GLT_HOME)argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm
	gcc $(OBJ).c -o $(OBJ)_arg_task -I$(GLT_HOME)/argobots -L$(GLT_HOME)/argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm -DTASK

fast_task:
	gcc $(OBJ).c -o $(OBJ)_fast_mth -I$(GLT_HOME)/massivethreads -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm -DFASTGLT
	gcc $(OBJ).c -o $(OBJ)_fast_qth -I$(GLT_HOME)/qthreads -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread -lm -DFASTGLT
	gcc $(OBJ).c -o $(OBJ)_fast_arg -I$(GLT_HOME)/argobots -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm -DFASTGLT
	gcc $(OBJ).c -o $(OBJ)_fast_arg_task -I$(GLT_HOME)/argobots -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm -DTASK -DFASTGLT
clean: 
	rm *.o $(OBJ)_mth $(OBJ)_qth $(OBJ)_arg* *~ *fast*
