#/bin/bash


rm *_total.dat

for i in 1 2 4 8 16 24 32 36 72
do
	cat arg_main_$i.dat | awk '{ sum += $1; count++} END { print sum/count}' >> arg_main_total.dat
	cat myth_main_$i.dat | awk '{ sum += $1; count++} END { print sum/count}' >> myth_main_total.dat
	cat qth_main_$i.dat | awk '{ sum += $1; count++} END { print sum/count}' >> qth_main_total.dat
	
	cat glt_a_$i.dat | awk '{ sum += $1; count++} END { print (sum/count)*3}' >> glt_a_total.dat
	cat glt_m_$i.dat | awk '{ sum += $1; count++} END { print (sum/count)*3}' >> glt_m_total.dat
	cat glt_q_$i.dat | awk '{ sum += $1; count++} END { print (sum/count)*3}' >> glt_q_total.dat
	
	cat glt_a_f_$i.dat | awk '{ sum += $1; count++} END { print sum/count}' >> glt_a_f_total.dat
	cat glt_m_f_$i.dat | awk '{ sum += $1; count++} END { print sum/count}' >> glt_m_f_total.dat
	cat glt_q_f_$i.dat | awk '{ sum += $1; count++} END { print sum/count}' >> glt_q_f_total.dat
done




