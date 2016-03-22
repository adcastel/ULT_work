#!/bin/bash


rm *.dat

for j in {1..200}
do
	echo "Iteration $j"

	export LD_LIBRARY_PATH=/home/adcastel/argobots/lib:$LD_LIBRARY_PATH
	for i in 1 2 4 8 16 24 32 36 72
	do 
		export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./arg_main 12 >> arg_main_$i.dat
	done
	
	export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH
	for i in 1 2 4 8 16 24 32 36 72
	do 
		export MYTH_WORKER_NUM=$i; export MYTH_BIND_WORKERS=1; ./myth_main 12 >> myth_main_$i.dat	
	done

	export LD_LIBRARY_PATH=/home/adcastel/qthreads/lib:$LD_LIBRARY_PATH
	for i in 1 2 4 8 16 24 32 36 72
	do 
		export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; export QTHREAD_SHEPHERD_BOUNDARY=core; export QTHREAD_WORKER_UNIT=core; export QTHREAD_AFFINITY=yes; ./qth_main 12 >> qth_main_$i.dat
	done

	export LD_LIBRARY_PATH=/home/adcastel/GLT/src/qthreads/:$LD_LIBRARY_PATH
	for i in 1 2 4 8 16 24 32 36 72
	do
		export GLT_NUM_THREADS=$i; export GLT_NUM_WORKERS_PER_SHEPHERD=1; ./glt_q 12 >> glt_q_$i.dat 
		export GLT_NUM_THREADS=$i; export GLT_NUM_WORKERS_PER_SHEPHERD=1; ./glt_q_f 12 >> glt_q_f_$i.dat 
	done

	export LD_LIBRARY_PATH=/home/adcastel/GLT/src/argobots/:$LD_LIBRARY_PATH
	for i in 1 2 4 8 16 24 32 36 72
	do 
		export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./glt_a 12 >> glt_a_$i.dat
		export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./glt_a_f 12 >> glt_a_f_$i.dat
	done

	export LD_LIBRARY_PATH=/home/adcastel/GLT/src/massivethreads/:$LD_LIBRARY_PATH
	for i in 1 2 4 8 16 24 32 36 72
	do 
		export GLT_NUM_THREADS=$i; ./glt_m 12 >> glt_m_$i.dat
		export GLT_NUM_THREADS=$i; ./glt_m_f 12 >> glt_m_f_$i.dat
	done

done


