#include <stdio.h>
/*
Add `int max_of_four(int a, int b, int c, int d)` here.
*/
int max_of_four(int a, int b, int c, int d){
    int max = a;
    int input[4] = {a, b, c, d};
    size_t length = sizeof(input) / sizeof(input[0]);
    /*
    Note: sizeof(input) returns the total size in bytes of the array
    int arr[] = {10, 20, 30, 40, 50};
    Address
    1000   10   (4 bytes)
    1004   20   (4 bytes)
    1008   30   (4 bytes)
    1012   40   (4 bytes)
    1016   50   (4 bytes)
    5 ints * 4 bytes = 20 bytes
    sizeof(arr[0])   // 4
    
    so 
    
    size_t length = sizeof(arr) / sizeof(arr[0]);
    20 / 4 = 5
    */
    for(int i = 0; i < length - 1; i++){
        if(input[i+1] > max){
            max = input[i+1];
        }
    }
    return max;
}

int main() {
    int a, b, c, d;
    scanf("%d %d %d %d", &a, &b, &c, &d);
    int ans = max_of_four(a, b, c, d);
    printf("%d", ans);
    
    return 0;
}
