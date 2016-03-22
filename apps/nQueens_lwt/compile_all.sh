#!/bin/bash


gcc ori_main.c -o arg_main -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DARGOBOTS

gcc ori_main.c -o qth_main -DQTHREADS -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread

gcc ori_main.c -o myth_main -DMASSIVETHREADS -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth


#FAST GLT

gcc glt_main.c -o glt_m_f -I/home/adcastel/GLT/src/massivethreads -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -DFASTGLT

gcc glt_main.c -o glt_q_f -I/home/adcastel/GLT/src/qthreads -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread -DFASTGLT

gcc glt_main.c -o glt_a_f -I/home/adcastel/GLT/src/argobots -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DFASTGLT

#GLT

gcc glt_main.c -o glt_a -I/home/adcastel/GLT/src/argobots -L/home/adcastel/GLT/src/argobots/ -lglt -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt

gcc glt_main.c -o glt_m -I/home/adcastel/GLT/src/massivethreads -L/home/adcastel/GLT/src/massivethreads -lglt -I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth

gcc glt_main.c -o glt_q -I/home/adcastel/GLT/src/qthreads -L/home/adcastel/GLT/src/qthreads -lglt -I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread



