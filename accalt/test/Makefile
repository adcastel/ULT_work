




all: basic task nested_for


basic:
	gcc basic.c -o basic_mth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/massivethreads/ -laccalt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth  -DMASSIVETHREADS -lm
	gcc basic.c -o basic_qth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/qthreads/ -laccalt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -DQTHREADS -lm
	gcc basic.c -o basic_arg -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/argobots/ -laccalt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm

task:
	gcc basic_tasks.c -o task_mth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/massivethreads/ -laccalt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth  -DMASSIVETHREADS -lm
	gcc basic_tasks.c -o task_qth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/qthreads/ -laccalt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -DQTHREADS -lm
	gcc basic_tasks.c -o task_arg -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/argobots/ -laccalt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm

nested_for:
	gcc basic_nested_for.c -o nested_for_mth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/massivethreads/ -laccalt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth  -DMASSIVETHREADS -lm
	gcc basic_nested_for.c -o nested_for_qth -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/qthreads/ -laccalt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread  -DQTHREADS -lm
	gcc basic_nested_for.c -o nested_for_arg -I/home/adcastel/ULT_work/accalt/src/ -L/home/adcastel/ULT_work/accalt/src/argobots/ -laccalt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS -lm


clean: 
	rm *.o basic_mth basic_qth basic_arg task_mth task_qth task_arg nested_for_mth nested_for_qth nested_for_arg *~
