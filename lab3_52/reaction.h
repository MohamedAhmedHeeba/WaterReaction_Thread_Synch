#include <stdio.h>
#include <pthread.h>

struct reaction {
    pthread_mutex_t lock_mutex;
    pthread_cond_t getting_new_H;
    pthread_cond_t ready_to_react;
    int number_of_H;
};

void reaction_init(struct reaction *reaction);

void reaction_h(struct reaction *reaction);

void reaction_o(struct reaction *reaction);
