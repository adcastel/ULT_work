

OBJ=tasks


all: $(OBJ) 


tasks:
	gcc $(OBJ).c -o $(OBJ)_mth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/massivethreads/ -laccalt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth  -DMASSIVETHREADS -lm
	gcc $(OBJ).c -o $(OBJ)_qth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/qthreads/ -laccalt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -DQTHREADS -lm
	gcc $(OBJ).c -o $(OBJ)_arg -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/argobots/ -laccalt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm

clean: 
	rm *.o $(OBJ)_mth $(OBJ)_qth $(OBJ)_arg *~
