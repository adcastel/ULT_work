#!/bin/bash

cd GLT_times
rm *_break_*
cd ../original_times
rm *_break_*
cd ..

for j in {1..1000}
do
export LD_LIBRARY_PATH=/home/adcastel/qthreads/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/adcastel/GLT/src/qthreads/:$LD_LIBRARY_PATH
for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_WORKERS_PER_THREAD=1; ./uts-glt_q $T1L | grep "breakdown" >> GLT_times/glt_q_break_$i.dat; done

export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/adcastel/GLT/src/massivethreads/:$LD_LIBRARY_PATH
for i in  1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i;  ./uts-glt_m $T1L | grep "breakdown" >> GLT_times/glt_m_break_$i.dat; done

export LD_LIBRARY_PATH=/home/adcastel/argobots/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/adcastel/GLT/src/argobots/:$LD_LIBRARY_PATH
for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-glt_a_t $T1L |  grep "breakdown" >> GLT_times/glt_a_t_break_$i.dat; done

for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-glt_a $T1L | grep "breakdown" >> GLT_times/glt_a_break_$i.dat; done

#FAST

export LD_LIBRARY_PATH=/home/adcastel/qthreads/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/adcastel/GLT/src/qthreads/:$LD_LIBRARY_PATH
for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_WORKERS_PER_THREAD=1; ./uts-glt_fast_q $T1L | grep "breakdown" >> GLT_times/glt_q_f_break_$i.dat; done

export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/adcastel/GLT/src/massivethreads/:$LD_LIBRARY_PATH
for i in  1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i;  ./uts-glt_fast_m $T1L | grep "breakdown" >> GLT_times/glt_m_f_break_$i.dat; done

export LD_LIBRARY_PATH=/home/adcastel/argobots/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/adcastel/GLT/src/argobots/:$LD_LIBRARY_PATH
for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-glt_fast_a_t $T1L |  grep "breakdown" >> GLT_times/glt_a_t_f_break_$i.dat; done

for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-glt_fast_a $T1L | grep "breakdown" >> GLT_times/glt_a_f_break_$i.dat; done


#ORI



for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-arg $T1L | grep "breakdown" >> original_times/uts_arg_break_$i.dat; done

for i in 1 2 4 8 16 24 32 36 72; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-arg_t $T1L | grep "breakdown" >> original_times/uts_arg_t_break_$i.dat; done

for i in 1 2 4 8 16 24 32 36 72; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; export QTHREAD_SHEPHERD_BOUNDARY=core; export QTHREAD_WORKER_UNIT=core; export QTHREAD_AFFINITY=yes; ./uts-qth $T1L | grep "breakdown" >> original_times/uts_qth_break_$i.dat; done

for i in 1 2 4 8 16 24 32 36 72; do export MYTH_WORKER_NUM=$i; export MYTH_BIND_WORKERS=1; ./uts-mth $T1L | grep "breakdown" >> original_times/uts_mth_break_$i.dat; done
done
