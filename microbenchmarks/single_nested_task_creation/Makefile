

OBJ=tasks


all: $(OBJ) 


tasks:
	gcc $(OBJ).c -o $(OBJ)_mth -I/home/adcastel/ULT_work/glt/src/ -L/home/adcastel/ULT_work/glt/src/massivethreads/ -lglt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth  -DMASSIVETHREADS -lm
	gcc $(OBJ).c -o $(OBJ)_qth -I/home/adcastel/ULT_work/glt/src/ -L/home/adcastel/ULT_work/glt/src/qthreads/ -lglt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -DQTHREADS -lm
	gcc $(OBJ).c -o $(OBJ)_arg -I/home/adcastel/ULT_work/glt/src/ -L/home/adcastel/ULT_work/glt/src/argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm
	gcc $(OBJ).c -o $(OBJ)_arg_task -I/home/adcastel/ULT_work/glt/src/ -L/home/adcastel/ULT_work/glt/src/argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm -DTASK

clean: 
	rm *.o $(OBJ)_mth $(OBJ)_qth $(OBJ)_arg* *~
