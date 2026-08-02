#include <stdio.h>
#include <stdlib.h>

int *total_number_of_books;
int **total_number_of_pages;

int main(void)
{
    int total_number_of_shelves;
    scanf("%d", &total_number_of_shelves);

    int total_number_of_queries;
    scanf("%d", &total_number_of_queries);

    total_number_of_books =
        calloc(total_number_of_shelves, sizeof *total_number_of_books);

    total_number_of_pages =
        calloc(total_number_of_shelves, sizeof *total_number_of_pages);

    if (total_number_of_books == NULL || total_number_of_pages == NULL) {
        free(total_number_of_books);
        free(total_number_of_pages);
        return 1;
    }

    while (total_number_of_queries--) {
        int type_of_query;
        scanf("%d", &type_of_query);

        if (type_of_query == 1) {
            int shelf_index;
            int page_count;

            scanf("%d %d", &shelf_index, &page_count);

            int old_book_count = total_number_of_books[shelf_index];
            int new_book_count = old_book_count + 1;

            int *resized_row = realloc(
                total_number_of_pages[shelf_index],
                new_book_count * sizeof *resized_row
            );

            if (resized_row == NULL) {
                for (int i = 0; i < total_number_of_shelves; i++) {
                    free(total_number_of_pages[i]);
                }

                free(total_number_of_pages);
                free(total_number_of_books);
                return 1;
            }

            total_number_of_pages[shelf_index] = resized_row;
            total_number_of_pages[shelf_index][old_book_count] = page_count;
            total_number_of_books[shelf_index] = new_book_count;

        } else if (type_of_query == 2) {
            int shelf_index;
            int book_index;

            scanf("%d %d", &shelf_index, &book_index);

            printf(
                "%d\n",
                total_number_of_pages[shelf_index][book_index]
            );

        } else if (type_of_query == 3) {
            int shelf_index;
            scanf("%d", &shelf_index);

            printf("%d\n", total_number_of_books[shelf_index]);
        }
    }

    for (int i = 0; i < total_number_of_shelves; i++) {
        free(total_number_of_pages[i]);
    }

    free(total_number_of_pages);
    free(total_number_of_books);

    return 0;
}

/* Main improvements are:
- Empty shelf rows remain NULL. realloc(NULL, size) works like malloc(size)
- The new book is always placed at old_book_count, so there is no need to check whether the first page count is 0
- A page count of 0 can be valid, so using total_number_of_pages[x][0] == 0 to detect an empty shelf is unreliable
- realloc uses a temporary pointer, preventing the original allocation from being lost if allocation fails
- The book count is updated only after memory allocation succeeds
- sizeof *pointer is used instead of repeating the data type.
*/
