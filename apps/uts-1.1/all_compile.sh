

gcc -O3 -lm -DBRG_RNG  -o uts-arg uts_shm.c rng/brg_sha1.c uts.c -DARGOBOTS -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt
gcc -O3 -lm -DBRG_RNG  -o uts-arg_t uts_shm.c rng/brg_sha1.c uts.c -DARGOBOTS -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DTASKLET

gcc -O3 -lm -DBRG_RNG  -o uts-qth uts_shm.c rng/brg_sha1.c uts.c -DQTHREADS -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread

 gcc -O3 -lm -DBRG_RNG  -o uts-mth uts_shm.c rng/brg_sha1.c uts.c -DMASSIVETHREADS -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth

#for i in {1..72}; do export GLT_NUM_THREADS=$i; export GLT_NUM_POOLS=$i; ./uts-arg $T1L >> original_times/uts_arg.dat; done

#for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; export QTHREAD_SHEPHERD_BOUNDARY=core; export QTHREAD_WORKER_UNIT=core; export QTHREAD_AFFINITY=yes; ./uts-qth $T1L >> original_times/uts_qth.dat; done

#for i in {1..72}; do export MYTH_WORKER_NUM=$i; export MYTH_BIND_WORKERS=1; ./uts-mth $T1L ; done
