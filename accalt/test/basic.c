#include <stdio.h>
#include <accalt.h>

#define N 100
void func1(void * arg){

printf("Hola soy el thread %d/%d\n",accalt_get_thread_num(),accalt_get_num_threads());
}

int main(int argc, char *argv[]){

accalt_init(argc,argv);

ACCALT_ult * ults;

ults = accalt_ult_malloc(N);

for (int i = 0; i < N; i++){
	accalt_ult_creation_to(func1,NULL,&ults[i],i%accalt_get_num_threads());
}

accalt_yield();

for (int i = 0; i < N; i++){
	accalt_ult_join(&ults[i]);
}
accalt_finalize();


}
