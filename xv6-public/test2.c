#include "types.h"
#include "user.h"
#include "x86.h"

#define NUM_THREADS 3
#define NUM_INCREMENTS 5
#define DELAY_MS 100

lock_t lock;
volatile int counter = 0;

void increment_counter(void *arg1, void *arg2) {
    int thread_id = *(int*)arg1;
    int i;
    for (i = 0; i < NUM_INCREMENTS; i++) {
        lock_acquire(&lock);
        counter++;
        printf(1, "Thread %d incremented counter to %d\n", thread_id, counter);
        lock_release(&lock);
        sleep(DELAY_MS); 
    }
    exit();
}

int main() {
    lock_init(&lock);
    int thread_ids[NUM_THREADS] = {1, 2, 3};
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        thread_create(increment_counter, (void *)&thread_ids[i], 0);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        thread_join();
    }

    printf(1, "Final counter value: %d\n", counter);

    exit();
}

	
