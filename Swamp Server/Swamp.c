#include "./1. core/core.h"
#include "./1. core/uciserver.h"
#include "./1. core/engine.h"




// int main(){





//     for(int i=7;i<=7; i++){
//         initializer();
//         uint64_t perft1=perftBULK(i);
//         // divide(3);
//     }
// }


int main(){
    pthread_t thread1;
    pthread_create(&thread1,NULL , EngineHandler ,(void*)retValueOfComputation);
    pthread_t thread2 ;
    pthread_create(&thread2,NULL , UCI_SERVER , (void*) retValueOfComputation);


    pthread_join(thread2 , NULL);
}
