#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BILLION 1000000000L

static int count;
static int max_profit;

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
        count++;
        profit_val = profit(row, n);
        if(profit_val > max_profit){
            for(j = 0; j < n; j++){
                max_row[j] = row[j];
            }
            max_profit = profit_val;
            printf("max_prof = %d \n",max_profit);
        }
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

int main(int argc, char **argv) {
    struct timespec start, end;
    double time;
    int n, i, j;
    int *row, **board, *max_row;
    int max_profit;
    if(argc != 2) {
        printf("Usage: bksb n\nAborting...\n");
        exit(0);
    }
    n = atoi(argv[1]);
    board = (int **) malloc(n * sizeof(int *));
    for(i = 0; i < n; i++) {
        board[i] = (int *) malloc(n*sizeof(int));
        for(j = i; j < n; j++) {
            board[i][j] = 0;
        }
    }

    
    max_row = (int *) malloc(n*sizeof(int));    
    row = (int *) malloc(n*sizeof(int));
    for(i=0;i<n;i++) {
        row[i] = -1;
    }
    count = 0;
    max_profit = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    nqueens(board, row, max_row, 0, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed: %lf seconds\n", time);
    
    printf("\n");
    printf("number of solutions: %d\n",count);
    printf("optimal solution:\n");
    for(i = 0; i < n; i++){
        printf("( %d, %d)\n",max_row[i],i);
    }
    return 0;
}