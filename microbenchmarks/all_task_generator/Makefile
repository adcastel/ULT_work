

OBJ=tasks


all: $(OBJ) 


tasks:
	gcc tasks.c -o task_f_a -I/home/adcastel/GLT/src/argobots -DFASTGLT -I/home/adcastel/argobots_opt/include/ -lm -L/home/adcastel/argobots_opt/lib/ -labt
	gcc tasks.c -o task_f_m -I/home/adcastel/GLT/src/massivethreads -DFASTGLT -I/home/adcastel/massivethreads/include/ -lm -L/home/adcastel/massivethreads/lib/ -lmyth
	gcc tasks.c -o task_f_q -I/home/adcastel/GLT/src/qthreads -DFASTGLT -I/home/adcastel/qthreads/include/ -lm -L/home/adcastel/qthreads/lib/ -lqthread
clean: 
	rm *.o $(OBJ)_mth $(OBJ)_qth $(OBJ)_arg  $(OBJ)_arg_task *~
