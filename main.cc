#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int array[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int sorted[SIZE];

void* sort(void* arg);
void* merge(void* arg);
int cmpfunc(const void* a, const void* b);

struct sort_args {
    int start;
    int end;
};

int main() {
    pthread_t t1, t2, t3;
    struct sort_args a1 = {0, SIZE/2};
    struct sort_args a2 = {SIZE/2, SIZE};

    pthread_create(&t1, NULL, sort, &a1);
    pthread_create(&t2, NULL, sort, &a2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_create(&t3, NULL, merge, NULL);
    pthread_join(t3, NULL);

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");
    return 0;
}

void* sort(void* arg) {
    struct sort_args* args = (struct sort_args*) arg;
    int start = args->start;
    int end = args->end;
    qsort(array + start, end - start, sizeof(int), cmpfunc);
    pthread_exit(NULL);
}

void* merge(void* arg) {
    int i = 0, j = SIZE/2, k = 0;
    while (i < SIZE/2 && j < SIZE) {
        if (array[i] < array[j]) {
            sorted[k++] = array[i++];
        } else {
            sorted[k++] = array[j++];
        }
    }
    while (i < SIZE/2) {
        sorted[k++] = array[i++];
    }
    while (j < SIZE) {
        sorted[k++] = array[j++];
    }
    pthread_exit(NULL);
}

int cmpfunc(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}
