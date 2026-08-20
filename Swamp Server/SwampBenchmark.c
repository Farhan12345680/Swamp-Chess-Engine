#include "./1. core/engine.h"





int main(){
        GAME_STATE_STRUCT state={0};

        initializer(&state);
        uint64_t perft1=divideBulk(7 , &state);

        // divide(3);

}


// int main(){
//     pthread_t thread1;
//     pthread_create(&thread1,NULL , EngineHandler ,(void*)retValueOfComputation);
//     pthread_t thread2 ;
//     pthread_create(&thread2,NULL , UCI_SERVER , (void*) retValueOfComputation);


//     pthread_join(thread2 , NULL);
// }
