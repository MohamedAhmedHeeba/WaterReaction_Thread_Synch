#include <pthread.h>
#include "reaction.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

void reaction_init(struct reaction *reaction)
{
    // initialization our lock
    pthread_mutex_init(&reaction->lock_mutex,NULL);
    // initialization condition of getting new H atom
    pthread_cond_init(&reaction->getting_new_H,NULL);
    // initialization condition of thr reaction is ready --> there are H2O
    pthread_cond_init(&reaction->ready_to_react,NULL);
    // start number of H atoms is Zero
    reaction->number_of_H = 0;

}

//*producer of H*/
void reaction_h(struct reaction *reaction)
{
    //start critical section in order to no comming atom during preparing the reaction
    pthread_mutex_lock(&reaction->lock_mutex);

    //once reached here --> we getting new H atom --> increase number of H
    reaction->number_of_H++;
    // number of H may be > 2 --> wake up thread O in reaction_o to check is there are enought H atoms to react or Not
    pthread_cond_signal(&reaction->getting_new_H);
    //once reached here --> there is less than 2 H atoms --> this thread must wait until getting new H and O and ready to reacte
    pthread_cond_wait(&reaction->ready_to_react,&reaction->lock_mutex);

    //end critical section
    pthread_mutex_unlock(&reaction->lock_mutex);
    /**after exit critical section h atom  will return**/
}

//*consumer of H*/
void reaction_o(struct reaction *reaction)
{
    //start critical section in order to no coming atom during preparing the reaction
    pthread_mutex_lock(&reaction->lock_mutex);

    // wait for getting two H atom --> number of H = 2
    while(reaction->number_of_H < 2){
        // recheck after each getting H atom
        pthread_cond_wait(&reaction->getting_new_H,&reaction->lock_mutex);
    }
    // once reached here this mean that we get at least 2 H atom & the current O atom --> reaction is ready :D
    make_water();
    //we consume 2 H atoms --> must decrease number of H
    reaction->number_of_H -= 2;
    //wake up any two H atoms that ready to reacte
    pthread_cond_signal(&reaction->ready_to_react);
    pthread_cond_signal(&reaction->ready_to_react);

    //end critical section
    pthread_mutex_unlock(&reaction->lock_mutex);
    /**after exit critical section O atom  will return*/
}


