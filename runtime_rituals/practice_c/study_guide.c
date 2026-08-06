#include <stdio.h>

int main() {
    /**
     * PROTIP: Read from right to left when dealing with pointers
     * PROTIP: Treat/Interpret const as "Read-Only" variable
     */

    /**
     * 1. pointer to an integer value, can change the value it points at and where p points to
     */
    int *p; 
    /**
      int a = 1, b = 2;
      int *p = &a;
      *p = 10;    // OK — changes a
       p  = &b;    // OK — now points at b
     */


    /**
     * 2. pointer to an integer value that is a constant (the integer value is constant (read only), whereas the pointer itself can change)
     */
    const int *p;
    /*
        const int *p = &a;
        *p = 10;    // ERROR — can't write through p
        p  = &b;    // OK
    */

    /**
     * 3. same thing as const int *p since int and const are to the left of * (ptr). Read as ptr to a constant integer
     */
    int const *p;


    /**
     * 4. p is a constant pointer to an integer value (the pointer cannot be written (read only))
     * The pointer is frozen; the value it points at is fair game. Because it can never be reassigned, it must be initialized at declaration.
     */
    int *const p;
    /*
    int *const p = &a;
    *p = 10;    // OK — changes a
    p  = &b;    // ERROR — can't repoint p
    */


    /**
     * 5. constant pointer to a constant integer value (read only pointer and read only int value)
     */
    const int *const p; 
    /*
    Both locked. Can't repoint, can't write through
    const int *const p = &a;
    *p = 10;    // ERROR
    p  = &b;    // ERROR
    */


    // basic pointer operations
    *p; // dereference or output is the value of the ptr or value at that address
    p+1; // point to the next memory element memory address
    /**
     * address:  1000   1004   1008   1012   1016
       value:  |  10  |  20  |  30  |  40  |  50 |
                   ↑      ↑
                   p     p+1
        Gives you a new pointer one element further along — address 1004, not 1001. 
        The compiler scales by sizeof(*p) automatically. This is why the pointer's type matters: a char * would advance 1 byte, a double * 8.
     */

    &p[i]; // memory address at p[i] in other words &(*(p+i)) ==> p + i because & * cancel each other out
    *(p+i); // equivalent to p[i]

    *p++;      // *(p++) — read *p, then advance p
    (*p)++;    // increment the value at p, p unchanged
    *++p;       // advance p, then read
    
    return 0;
}
