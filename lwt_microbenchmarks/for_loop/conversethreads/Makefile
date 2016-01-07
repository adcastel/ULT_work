


all:
	/home/adcastel/src/charm/multicore-linux64/bin/charmc -c ct_self_dispatch.c -lm
	/home/adcastel/src/charm/multicore-linux64/bin/charmc -o ct_self ct_self_dispatch.o -language converse++
	/home/adcastel/src/charm/multicore-linux64/bin/charmc -c ct_normal_mode.c -lm
	/home/adcastel/src/charm/multicore-linux64/bin/charmc -o ct_normal ct_normal_mode.o -language converse++
	/home/adcastel/src/charm/multicore-linux64/bin/charmc -c ct_return_mode.c -lm
	/home/adcastel/src/charm/multicore-linux64/bin/charmc -o ct_return ct_return_mode.o -language converse++
	



