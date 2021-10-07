#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BILLION 1000000000L

int count = 0;
int max_profit;
int can_place(int *col, int **board, int col_val, int row_val, int n){
    int i,j;
    for(i=0;i<n;i++) //check column
        if(col_val == col[i]) return 0;
    

    //check diagonals
    int k = 1;
    while(col_val - k >= 0 && row_val - k >= 0){
        if(board[col_val-k][row_val - k]) return 0;
        k++;
    }
    k = 1;
    while(col_val + k < n && row_val + k < n ){
        if(board[col_val + k][row_val + k]) return 0;
        k++;
    }
    return 1;
}
int profit(int *col, int n) {
    int sum = 0;
    int i;
    for(i = 0; i<n; i++) {
        sum += abs(col[i]-i);
    }
    return sum;
}

void nqueens(int **board, int *col, int *max_col, int row_num, int n) {
    int i, j;
    int profit_val;
    if(row_num == n) return;
    
    for(i = 0; i < n; i++) {
        if(can_place(col, board, i, row_num, n)){
            board[i][row_num] = 1;
            col[row_num] = i;
            if(row_num == n-1){
                count++;
                profit_val = profit(col, n);
                if(profit_val > max_profit){
                    for(j = 0; j < n; j++){
                        max_col[j] = col[j];
                    }
                    max_profit = profit_val;
                }
            }else nqueens(board, col,max_col, row_num+1,n);
        }
    }

} 

int main(int argc, char **argv) {
    struct timespec start, end;
    double time;
    int n, i, j;
    int *col, *row, **board, *max_col;
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

    col = (int *) malloc(n*sizeof(int));
    max_col = (int *) malloc(n*sizeof(int));    
    row = (int *) malloc(n*sizeof(int));
    for(i=0;i<n;i++) {
        col[i] = -1;
        row[i] = -1;
    }
    int count = 0;
    max_profit = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    nqueens(board,col,row,0, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed: %lf seconds\n", time);
    
    printf("\n");
    printf("number of solutions: %d\n",count);
    printf("maximum profit: %d\n",max_profit);
    printf("optimal solution:\n");
    for(i = 0; i < n; i++){
        printf("( %d, %d)\n",max_col[i],i);
    }
    return 0;
}