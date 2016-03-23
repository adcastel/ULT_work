#|/bin/bash

rm uts-glt_* 

gcc -O3 -lm -DBRG_RNG  -o uts-glt_fast_q uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/qthreads -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread -DFASTGLT

gcc -O3 -lm -DBRG_RNG  -o uts-glt_fast_a_t uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/argobots -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DTASKLET -DFASTGLT

gcc -O3 -lm -DBRG_RNG  -o uts-glt_fast_a uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/argobots -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DFASTGLT

gcc -O3 -lm -DBRG_RNG  -o uts-glt_fast_m uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/massivethreads -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -DFASTGLT

gcc -O3 -lm -DBRG_RNG  -o uts-glt_q uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/qthreads -L/home/adcastel/GLT/src/qthreads -lglt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread

gcc -O3 -lm -DBRG_RNG  -o uts-glt_a_t uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/argobots -I/home/adcastel/argobots/include -L/home/adcastel/GLT/src/argobots -lglt -L/home/adcastel/argobots/lib -labt -DTASKLET

gcc -O3 -lm -DBRG_RNG  -o uts-glt_a uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/argobots -I/home/adcastel/argobots/include -L/home/adcastel/GLT/src/argobots -lglt -L/home/adcastel/argobots/lib -labt

gcc -O3 -lm -DBRG_RNG  -o uts-glt_m uts_shm.c rng/brg_sha1.c uts.c -DGLT -I/home/adcastel/GLT/src/massivethreads -L/home/adcastel/GLT/src/massivethreads -lglt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth


