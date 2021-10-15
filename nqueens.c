#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define BILLION 1000000000L

static int count;
static int max_profit;
pthread_mutex_t lock;

typedef struct {
    int *max_row, n, pid, p;
} GM;

int can_place(int **board, int row_val, int col_val, int n) {
    int i,j;
    for(i = 0; i<n;i++){
        if(board[row_val][i]) return 0;
    }

    for (i = row_val, j = col_val; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return 0;
    
    for (i = row_val, j = col_val; j >= 0 && i < n; i++, j--)
        if (board[i][j])
            return 0;

    return 1;
}

int profit(int *row, int n) {
    int sum = 0;
    int i;
    for(i = 0; i<n; i++) {
        sum += abs(row[i]-i);
    }
    return sum;
}
    
void nqueens(int **board, int *row, int *max_row, int col_num, int n) {
    int i,j,profit_val;
    if(col_num == n){
        pthread_mutex_lock(&lock);
        count++;
        profit_val = profit(row, n);
        if(profit_val > max_profit){
            for(j = 0; j < n; j++){
                max_row[j] = row[j];
            }
            max_profit = profit_val;
            printf("max_prof = %d \n",max_profit);    
        }
        pthread_mutex_unlock(&lock);
        return;
    }
    for(i = 0; i< n; i++){
        if(can_place(board,i,col_num,n)) {
            board[i][col_num] = 1;
            row[col_num] = i;
            nqueens(board,row,max_row,col_num+1,n);
            board[i][col_num] = 0;
        }
    }
    return;
}

void * nqueens_call(void *varg) {
    GM *arg = varg;
    int *max_row, n, pid, p;
    max_row = arg->max_row;
    n = arg->n;
    pid = arg->pid;
    p = arg->p;
    int i, j, **board, *row;
    if(pid<n){
        board = (int **) malloc(n * sizeof(int *));
        for(i = 0; i < n; i++) {
            board[i] = (int *) malloc(n*sizeof(int));
            for(j = i; j < n; j++) {
                board[i][j] = 0;
            }
        }
        row = (int *) malloc(n*sizeof(int));
        for(i=0;i<n;i++) {
            row[i] = -1;
        }

        for(i = pid; i<n; i+=p){
            board[i][0] = 1;
            row[0] = i;
            
            nqueens(board,row,max_row,1,n);
            board[i][0] = 0;
        }
    }
}

int main(int argc, char **argv) {
    struct timespec start, end, init_time;
    double time;
    int n, i, j, p;
    int *row, **board, *max_row;
    int max_profit;
    if(argc != 3) {
        printf("Usage: nqueens n p\nAborting...\n");
        exit(0);
    }
    n = atoi(argv[1]);
    p = atoi(argv[2]);
    

    max_row = (int *) malloc(n*sizeof(int));    
    
    count = 0;
    max_profit = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t *threads = malloc(p * sizeof(threads));
    for(i = 0; i < p; i++) {
        GM *arg = malloc(sizeof(*arg));
        arg->max_row = max_row;
        arg->n = n;
        arg->pid = i;
        arg->p = p;
        pthread_create(&threads[i], NULL, nqueens_call, arg);
    }
    // clock_gettime(CLOCK_MONOTONIC, &init_time);
    // double initialize_time = BILLION *(init_time.tv_sec - start.tv_sec) +(init_time.tv_nsec - start.tv_nsec);
    // printf("Initialization time: %lf seconds\n", initialize_time);
    for(i = 0; i < p; i++)
        pthread_join(threads[i], NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);
    free(threads);

    time = BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed time: %lf seconds\n", time);

    printf("\n");
    printf("number of solutions: %d\n",count);
    printf("optimal solution:\n");
    for(i = 0; i < n; i++){
        printf("( %d, %d)\n",max_row[i],i);
    }
    return 0;
}