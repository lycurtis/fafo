#include <stdio.h>
#include <stdlib.h>

/*
 * This stores the total number of books in each shelf.
 */
int* total_number_of_books;

/*
 * This stores the total number of pages in each book of each shelf.
 * The rows represent the shelves and the columns represent the books.
 */
int** total_number_of_pages;

/*
5 = 5 shevles
5 = 5 queries
1 0 15
1 0 20
1 2 78
2 2 0
3 0

total number of books = [2, 0, 1, 0, 0] index is the shelf, value is the total number of books
total number of pages = [[15, 20],[0],[78],[0],[0]] | [row] index = shelf | [col] index = book | [row][col] value = pages in a book
*/
int main()
{
    int total_number_of_shelves;
    scanf("%d", &total_number_of_shelves);
    
    int total_number_of_queries;
    scanf("%d", &total_number_of_queries);
    
    total_number_of_books = calloc(total_number_of_shelves, sizeof(int));
    total_number_of_pages = calloc(total_number_of_shelves, sizeof(int *));
    
    // Allocate memory for each individual row (is this necessary?)
    // for (int i = 0; i < total_number_of_shelves; i++) {
    //     total_number_of_pages[i] = (int *)calloc(1, sizeof(int));
    //     if (total_number_of_pages[i] == NULL) {
    //     }
    // }
    
    while (total_number_of_queries--) {
        int type_of_query;
        scanf("%d", &type_of_query);
        
        if (type_of_query == 1) {
            /*
             * Process the query of first type here.
             */
            int x, y;
            scanf("%d %d", &x, &y); // x = shelf index and y = page count
            
            total_number_of_books[x] += 1; // *(total_number_of_books + x) += 1;
            
            total_number_of_pages[x] = (int *)realloc(total_number_of_pages[x], total_number_of_books[x] * sizeof(int));
            
            if(total_number_of_pages[x][0] == 0){
                total_number_of_pages[x][0] = y;
            }
            else{
                total_number_of_pages[x][total_number_of_books[x]-1] = y;
            }
            
        } else if (type_of_query == 2) {
            int x, y;
            scanf("%d %d", &x, &y);
            printf("%d\n", *(*(total_number_of_pages + x) + y));
        } else {
            int x;
            scanf("%d", &x);
            printf("%d\n", *(total_number_of_books + x));
        }
    }

    if (total_number_of_books) {
        free(total_number_of_books);
    }
    
    for (int i = 0; i < total_number_of_shelves; i++) {
        if (*(total_number_of_pages + i)) {
            free(*(total_number_of_pages + i));
        }
    }
    
    if (total_number_of_pages) {
        free(total_number_of_pages);
    }
    
    return 0;
}
