/* reverse_array_test.c
 *
 * Practice harness: reverse an array in place using pointers.
 *
 * Build:  gcc -Wall -Wextra -std=c11 -o test reverse_array_test.c && ./test
 *
 * Fill in the stubs below. Start with reverse_array; the rest are follow-ups.
 * Leave a follow-up unimplemented and its tests will just report as skipped.
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

/* ===================================================================
 * YOUR CODE HERE
 * =================================================================== */

void reverse_array(int *arr, size_t n)
{
    (void)arr;
    (void)n;
    /* TODO */
    int *left = arr; // start cursor
    int *right = arr + (n-1); // end cursor
    if(n == 0 || arr == NULL){ // cover edge case to avoid segmentation fault or poor access
        // do nothing
        return;
    }
    else{
        while(left < right){
            int temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
    
}

/* Follow-up 1: no length given. */
void reverse_string(char *s)
{
    (void)s;
    /* TODO */
    // int size = strlen(s); OR
    int size = 0;
    for(int i = 0; *(s+i) != '\0'; i++){
        size++;
    }
    
    char *left = s;
    char *right = s + (size - 1);

    if(size != 0 && s != NULL){
        while(left < right){
            char temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
}

/* Follow-up 2: reverse the half-open range [first, last). */
void reverse_range(int *first, int *last)
{
    (void)first;
    (void)last;
    /* TODO */

    int *left = first;
    int *right = last-1;

    if(left != right && left != NULL){
        while(left < right){
            int temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
}

/* Follow-up 3: reverse `count` elements of `size` bytes each. */
void reverse_generic(void *base, size_t count, size_t size)
{
    (void)base;
    (void)count;
    (void)size;
    /* TODO */

    // num_of_elements * sizeof(each_element_in_bytes) == count * size = total byte count of array

    // Note cannot do ptr arithmetic with void, ou need a pointer type with a known, 1-byte size:
    // so we must use char as a generic start as 1 byte that way p + 1 = 1 byte forward
    unsigned char *p = base; // ptr value is 1 byte since it is a char (which is a good generic start). 

    unsigned char *left = p;
    unsigned char *right = p + ((count-1) * size);

    if(left != right && left != NULL){
        while(left < right){
            for(int i = 0; i < size; i++){
                // *(tmp + i) = *(left + i);
                unsigned char tmp = *(left + i);
                *(left+i) = *(right+i);
                *(right+i) = tmp;
            }
            left += size;
            right -= size;
        }
    }

}

/* AI CLEANER SOLUTION */
// void reverse_generic(void *base, size_t count, size_t size)
// {
//     if (base == NULL || count < 2 || size == 0) return;

//     unsigned char *left  = base;
//     unsigned char *right = left + (count - 1) * size;

//     while (left < right) {
//         for (size_t i = 0; i < size; i++) {
//             unsigned char tmp = left[i];
//             left[i]  = right[i];
//             right[i] = tmp;
//         }
//         left  += size;
//         right -= size;
//     }
// }

/* ===================================================================
 * TEST HARNESS -- don't edit below this line
 * =================================================================== */

static int passed = 0;
static int failed = 0;

static void print_ints(const int *a, size_t n)
{
    printf("[");
    for (size_t i = 0; i < n; i++)
        printf("%s%d", i ? ", " : "", a[i]);
    printf("]");
}

static void check_ints(const char *name, int *got, const int *want, size_t n)
{
    if (memcmp(got, want, n * sizeof *got) == 0) {
        printf("  PASS  %s\n", name);
        passed++;
    } else {
        printf("  FAIL  %s\n        got  ", name);
        print_ints(got, n);
        printf("\n        want ");
        print_ints(want, n);
        printf("\n");
        failed++;
    }
}

static void check_str(const char *name, const char *got, const char *want)
{
    if (strcmp(got, want) == 0) {
        printf("  PASS  %s\n", name);
        passed++;
    } else {
        printf("  FAIL  %s\n        got  \"%s\"\n        want \"%s\"\n",
               name, got, want);
        failed++;
    }
}

static void test_reverse_array(void)
{
    printf("reverse_array\n");

    /* 1. Empty array. Must not crash, must not touch the canary. */
    {
        int a[2] = { 99, 99 };
        int want[2] = { 99, 99 };
        reverse_array(a, 0);
        check_ints("n = 0 leaves memory untouched", a, want, 2);
    }

    /* 2. NULL with n == 0. The classic "did you guard it" case. */
    {
        reverse_array(NULL, 0);
        printf("  PASS  NULL with n = 0 does not crash\n");
        passed++;
    }

    /* 3. Single element. */
    {
        int a[] = { 42 };
        int want[] = { 42 };
        reverse_array(a, 1);
        check_ints("single element", a, want, 1);
    }

    /* 4. Two elements -- smallest case that actually swaps. */
    {
        int a[] = { 1, 2 };
        int want[] = { 2, 1 };
        reverse_array(a, 2);
        check_ints("two elements", a, want, 2);
    }

    /* 5. Even length. */
    {
        int a[] = { 1, 2, 3, 4 };
        int want[] = { 4, 3, 2, 1 };
        reverse_array(a, 4);
        check_ints("even length", a, want, 4);
    }

    /* 6. Odd length -- middle element must stay put. */
    {
        int a[] = { 1, 2, 3, 4, 5 };
        int want[] = { 5, 4, 3, 2, 1 };
        reverse_array(a, 5);
        check_ints("odd length", a, want, 5);
    }

    /* 7. Duplicates and negatives -- no value-based logic should sneak in. */
    {
        int a[] = { -3, 0, 7, 7, -3, 100 };
        int want[] = { 100, -3, 7, 7, 0, -3 };
        reverse_array(a, 6);
        check_ints("duplicates and negatives", a, want, 6);
    }

    /* 8. Reversing twice is the identity. */
    {
        int a[] = { 5, 1, 4, 2, 3, 9, 8 };
        int want[] = { 5, 1, 4, 2, 3, 9, 8 };
        reverse_array(a, 7);
        reverse_array(a, 7);
        check_ints("double reverse is identity", a, want, 7);
    }

    /* 9. Partial reverse -- only the first k elements move. */
    {
        int a[] = { 1, 2, 3, 4, 5, 6 };
        int want[] = { 3, 2, 1, 4, 5, 6 };
        reverse_array(a, 3);
        check_ints("prefix of a longer array", a, want, 6);
    }

    /* 10. Larger array, checked structurally rather than by eye. */
    {
        enum { N = 1000 };
        static int a[N], want[N];
        for (int i = 0; i < N; i++) {
            a[i] = i;
            want[i] = N - 1 - i;
        }
        reverse_array(a, N);
        check_ints("1000 elements", a, want, N);
    }

    printf("\n");
}

static void test_reverse_string(void)
{
    printf("reverse_string (follow-up 1)\n");

    { char s[] = "";       reverse_string(s); check_str("empty string", s, ""); }
    { char s[] = "a";      reverse_string(s); check_str("one char", s, "a"); }
    { char s[] = "ab";     reverse_string(s); check_str("two chars", s, "ba"); }
    { char s[] = "hello";  reverse_string(s); check_str("odd length", s, "olleh"); }
    { char s[] = "abcd";   reverse_string(s); check_str("even length", s, "dcba"); }
    { char s[] = "racecar";reverse_string(s); check_str("palindrome", s, "racecar"); }
    { char s[] = "a b\tc"; reverse_string(s); check_str("whitespace preserved", s, "c\tb a"); }

    printf("\n");
}

static void test_reverse_range(void)
{
    printf("reverse_range (follow-up 2)\n");

    /* Empty range: first == last. */
    {
        int a[] = { 1, 2, 3 };
        int want[] = { 1, 2, 3 };
        reverse_range(a + 1, a + 1);
        check_ints("empty range", a, want, 3);
    }

    /* Interior slice -- ends must not move. */
    {
        int a[] = { 0, 1, 2, 3, 4, 5 };
        int want[] = { 0, 4, 3, 2, 1, 5 };
        reverse_range(a + 1, a + 5);
        check_ints("interior slice [1,5)", a, want, 6);
    }

    /* Whole array via the range API. */
    {
        int a[] = { 1, 2, 3, 4 };
        int want[] = { 4, 3, 2, 1 };
        reverse_range(a, a + 4);
        check_ints("whole array", a, want, 4);
    }

    /* Suffix. */
    {
        int a[] = { 9, 8, 1, 2, 3 };
        int want[] = { 9, 8, 3, 2, 1 };
        reverse_range(a + 2, a + 5);
        check_ints("suffix", a, want, 5);
    }

    printf("\n");
}

static void test_reverse_generic(void)
{
    printf("reverse_generic (follow-up 3)\n");

    /* Same ints, routed through the byte-swapping version. */
    {
        int a[] = { 1, 2, 3, 4, 5 };
        int want[] = { 5, 4, 3, 2, 1 };
        reverse_generic(a, 5, sizeof(int));
        check_ints("ints", a, want, 5);
    }

    /* Single-byte elements. */
    {
        char a[] = { 'a', 'b', 'c', 'd' };
        char want[] = { 'd', 'c', 'b', 'a' };
        reverse_generic(a, 4, sizeof(char));
        if (memcmp(a, want, 4) == 0) { printf("  PASS  chars\n"); passed++; }
        else { printf("  FAIL  chars\n"); failed++; }
    }

    /* Wide elements -- catches a swap loop that assumes 4 or 8 bytes. */
    {
        typedef struct { double x, y, z; } Vec;
        Vec a[3] = { {1,1,1}, {2,2,2}, {3,3,3} };
        reverse_generic(a, 3, sizeof(Vec));
        if (a[0].x == 3 && a[1].x == 2 && a[2].x == 1) {
            printf("  PASS  24-byte structs\n"); passed++;
        } else {
            printf("  FAIL  24-byte structs\n"); failed++;
        }
    }

    /* Degenerate counts. */
    {
        int a[] = { 7 };
        int want[] = { 7 };
        reverse_generic(a, 1, sizeof(int));
        reverse_generic(a, 0, sizeof(int));
        check_ints("count 0 and 1", a, want, 1);
    }

    printf("\n");
}

int main(void)
{
    test_reverse_array();
    test_reverse_string();
    test_reverse_range();
    test_reverse_generic();

    printf("=====================================\n");
    printf("passed: %d   failed: %d\n", passed, failed);
    return failed != 0;
}