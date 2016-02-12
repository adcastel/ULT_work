#include <stdio.h>
#include <acoilt.h>

#define N 100
void * func1(void * arg){

printf("Hola soy el thread %d/%d\n",acoilt_get_thread_num(),acoilt_get_num_threads());
}

int main(int argc, char *argv[]){

acoilt_init(argc,argv);

ACOILT_ult * ults;

ults = acoilt_ult_malloc(N);

for (int i = 0; i < N; i++){
	acoilt_ult_creation_to(func1,NULL,&ults[i],i%acoilt_get_num_threads());
}

acoilt_yield();

for (int i = 0; i < N; i++){
	acoilt_ult_join(&ults[i]);
}
acoilt_finalize();


}
