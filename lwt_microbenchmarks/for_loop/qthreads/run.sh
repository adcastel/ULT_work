echo "1 shepherd node"
for i in {1..36}
do
	export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_SHEPHERD_BOUNDARY=node export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; export QTHREAD_WORKER_UNIT=core; ./qthreads_fork_to 1000
done

echo "2 shepherd node"
for i in {1..18}; do export QTHREAD_NUM_SHEPHERDS=2; export QTHREAD_SHEPHERD_BOUNDARY=node export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; export QTHREAD_WORKER_UNIT=core; ./qthreads_fork_to 1000; done

echo "2 shepherd socket"
for i in {1..18}
do
export QTHREAD_NUM_SHEPHERDS=2; export QTHREAD_SHEPHERD_BOUNDARY=socket export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; export QTHREAD_WORKER_UNIT=core; ./qthreads_fork_to 1000
done


echo "4 shepherd socket"
for i in {1..9}
do
export QTHREAD_NUM_SHEPHERDS=4; export QTHREAD_SHEPHERD_BOUNDARY=socket export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; export QTHREAD_WORKER_UNIT=core; ./qthreads_fork_to 1000
done


echo "1 shepherd core 1 worker core"
for i in {1..36}; do         export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_SHEPHERD_BOUNDARY=core export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; export QTHREAD_WORKER_UNIT=core; ./qthreads_fork_to 1000; done


