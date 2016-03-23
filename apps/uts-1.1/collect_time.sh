#/bin/bash


cd GLT_times

rm* _total.dat

for i in 1 2 4 8 16 24 32 36 72
do
	cat glt_q_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_q_break_total.dat
	cat glt_a_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_a_break_total.dat
	cat glt_a_t_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_a_t_break_total.dat
	cat glt_m_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_m_break_total.dat
	
#FAST
	cat glt_q_f_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_q_f_break_total.dat
	cat glt_a_f_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_a_f_break_total.dat
	cat glt_a_t_f_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_a_t_f_break_total.dat
	cat glt_m_f_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> glt_m_f_break_total.dat
done

cd ../original_times


rm* _total.dat
for i in 1 2 4 8 16 24 32 36 72
do
	cat uts_qth_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> uts_qth_break_total.dat
	cat uts_mth_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> uts_mth_break_total.dat
	cat uts_arg_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> uts_arg_break_total.dat
	cat uts_arg_t_break_$i.dat | awk '{ sum += $6; create += $8; yield +=$10; join +=$12; count++} END { print sum/count" "create/count" "yield/count" "join/count}' >> uts_arg_t_break_total.dat

done



