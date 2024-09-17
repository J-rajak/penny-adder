#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

char *messages[3] = {NULL, NULL, NULL};
int lock = 0; // 0 means unlocked, 1 means locked

void *messenger(void *p)
{
    long tid = (long)p;
    char tmpbuf[100];
    for (int i = 0; i < 10; i++)
    {
        /* Sending a message */
        long int dest = (tid + 1) % 3;
        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        char *msg = strdup(tmpbuf);

        // Busy-waiting for the lock to become available
        while (__sync_lock_test_and_set(&lock, 1) == 1)
        {
        } // Spinlock

        messages[dest] = msg;
        printf("Thread %ld sent the message to Thread %ld\n", tid, dest);

        // Release the lock
        __sync_lock_release(&lock);

        /* Receiving a message */
        // Busy-waiting for the lock to become available
        while (__sync_lock_test_and_set(&lock, 1) == 1)
        {
        } // Spinlock

        printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
        free(messages[tid]);
        messages[tid] = NULL;

        // Release the lock
        __sync_lock_release(&lock);
    }
    return NULL;
}

int main()
{
    pthread_t thrID1, thrID2, thrID3;
    pthread_create(&thrID1, NULL, messenger, (void *)0);
    pthread_create(&thrID2, NULL, messenger, (void *)1);
    pthread_create(&thrID3, NULL, messenger, (void *)2);

    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);
    pthread_join(thrID3, NULL);

    return 0;
}
