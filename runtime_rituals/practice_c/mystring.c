/*
 * mystring.c — libc string/memory reimplementation practice
 *
 * Build:  gcc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined mystring.c -o mystring
 * Run:    ./mystring
 *
 * Fill in the five functions below. Everything under "TEST HARNESS" is
 * already written — don't touch it, just make it pass.
 *
 * Rules of the exercise (same as the screening):
 *   - No calling the real strlen/memcpy/memmove/memcmp/strncpy/etc.
 *   - <string.h> is included ONLY so the harness can print things; your
 *     implementations must not use it.
 *   - Assume a hosted C environment, but no compiler builtins as shortcuts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>   /* harness only */

/* ------------------------------------------------------------------ */
/* IMPLEMENT THESE                                                     */
/* ------------------------------------------------------------------ */

/*
 * my_strlen: number of chars before the terminating '\0'.
 * Precondition: s is non-NULL and NUL-terminated.
 */
size_t my_strlen(const char *s)
{
    (void)s;
    /* TODO */

    int char_count = 0;
    for(int i = 0; s[i] != '\0'; i++){
        char_count++;
    }
    return char_count; 
}

/*
 * my_strnlen: like my_strlen, but examines at most maxlen bytes and
 * returns maxlen if no terminator is found within them.
 * Must not read past s[maxlen-1].
 */
size_t my_strnlen(const char *s, size_t maxlen)
{
    (void)s; (void)maxlen;
    /* TODO */

    size_t count = 0;

    for(size_t i = 0; i < maxlen; i++){
        if(s[i] != '\0'){
            count++;
        }
        else{
            break;
        }
    }
    return count; 
}

/*
 * my_memcpy: copy n bytes from src to dst. Regions must not overlap.
 * Returns dst.
 */
void *my_memcpy(void *dst, const void *src, size_t n)
{
    // (void)src; (void)n;

    // NOTE: The parameters are void *, but you can't dereference or index a void *. What do you need to do before you can read and write individual bytes?
    // char * is a good generic replacement since char is 1 byte
    /**
     * BUT we use unsigned char instead of char because 
     *
    * Why unsigned char * and not char * or void *:
    *
    *   void *        - can't be dereferenced or indexed; no size to step by.
    *
    *   aliasing      - the standard lets an object be accessed through a
    *                   character-type lvalue regardless of its real type
    *                   (C11 6.5p7). Any other pointer type would be strict-
    *                   aliasing UB. This exemption is what makes mem* functions
    *                   expressible in C: the caller may pass a struct, a double,
    *                   a packet buffer.
    *
    *   signedness    - plain char's signedness is implementation-defined:
    *                   signed on x86, unsigned on most ARM ABIs. unsigned char
    *                   is defined everywhere.
    *
    *   representation- unsigned char is guaranteed to have no padding bits and
    *                   no trap representations, so every bit pattern is a valid
    *                   value and a round-trip can't corrupt data.
    *
    * Signedness only matters for memcmp (bytes compare AS unsigned char, so
    * 0x80 > 0x01; a signed load would sign-extend to -128 and flip the result).
    * For memcpy/memmove the copy is bit-identical either way -- but use
    * unsigned char anyway, for consistency and for the representation guarantee.
    */
    
    // unsigned char *dst_cursor = dst;
    // const unsigned char *src_cursor = src;

    // for(size_t i = 0; i < n; i++){
    //     dst_cursor[i] = src_cursor[i];
    // }

    // return dst;

    /* Cleaner solution */
    if(dst == NULL) return NULL;

    unsigned char *char_dst = (unsigned char*) dst;
    unsigned char *char_src = (unsigned const char*) src;

    for(size_t i = 0; i < n; i++){
        char_dst[i] = char_src[i];
    }
    return dst;

    /**
     * Look at what happens if you run this exact loop with dst = src + 1 and n = 10, tracing byte by byte. 
     * Then try dst = src - 1. One of those two is fine, the other corrupts. That asymmetry is the whole problem.
     * Solution: memmove
     */
}

/*
 * my_memmove: copy n bytes from src to dst. Regions MAY overlap; the
 * result must be as if src were first copied to a temporary buffer.
 * Returns dst.
 */
// void *my_memmove(void *dst, const void *src, size_t n)
// {
//     // (void)src; (void)n;

//     /* TODO */
//     if(dst == NULL) return NULL;

//     unsigned char *char_dst = (unsigned char*) dst;
//     unsigned char *char_src = (unsigned const char*) src;

//     unsigned char* tmp = calloc(n, sizeof(*tmp));

//     for(size_t i = 0; i < n; i++){
//         tmp[i] = char_src[i];
        
//     }
//     for(size_t i = 0; i < n; i++){
//         char_dst[i] = tmp[i];
        
//     }
//     free(tmp);
//     return dst;

// }

/*
Problem with this implementation:
1. Stack overflow. unsigned char tmp[n] allocates n bytes on the stack. 
   Real memmove gets called with large n (kilobytes, megabytes). 
   A VLA that big blows the stack. memcpy/memmove must handle arbitrary sizes.
2. It defeats the point. You can correctly handle overlap by copying through a temp buffer 
   the logic is sound — but the standard, allocation-free way to handle overlapping regions 
   is to pick the copy direction based on the pointers.
*/
// void *my_memmove(void *dst, const void *src, size_t n)
// {
//     // (void)src; (void)n;

//     /* TODO */
//     if(dst == NULL || n == 0) return NULL;

//     unsigned char *char_dst = (unsigned char*) dst;
//     unsigned char *char_src = (unsigned const char*) src;

//     unsigned char tmp[n];
//     unsigned char *p = tmp;

//     for(size_t i = 0; i < n; i++){
//         p[i] = char_src[i]; // (*(p+i) = *(char_src+i)); Copy from source to temporary
        
//     }

//     p = tmp; //reset pointer to beginning of tmp
//     while(n--) {
//         *(char_dst++) = *(p++);
//     }
//     return dst;
// }

void *my_memmove(void *dst, const void *src, size_t n)
{
    // (void)src; (void)n;

    /* TODO */
    if(dst == NULL || n == 0) return NULL;

    unsigned char *char_dst = (unsigned char*) dst;
    unsigned char *char_src = (unsigned const char*) src;

    if(dst < src){
        for(size_t i = 0; i < n; i++){
            char_dst[i] = char_src[i];
        }
    }
    else{
        for(size_t i = n; i > 0; i--){
            char_dst[i-1] = char_src[i-1];
        }
    }
    return dst;
}

/*
 * my_memcmp: compare the first n bytes of a and b.
 * Returns <0, 0, or >0 per the standard. Bytes compare as unsigned char.
 */
int my_memcmp(const void *a, const void *b, size_t n)
{
    (void)a; (void)b; (void)n;
    return 0; /* TODO */
}

/* ------------------------------------------------------------------ */
/* TEST HARNESS — no edits needed below this line                      */
/* ------------------------------------------------------------------ */

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, ...)                                                   \
    do {                                                                   \
        if (cond) { g_pass++; }                                            \
        else {                                                             \
            g_fail++;                                                      \
            printf("  FAIL %s:%d: ", __func__, __LINE__);                  \
            printf(__VA_ARGS__);                                           \
            printf("\n");                                                  \
        }                                                                  \
    } while (0)

/* Sign-normalizing comparator: the standard only promises the sign. */
static int sgn(int x) { return (x > 0) - (x < 0); }

/* A buffer with poison bytes on both sides, so we catch over-copies. */
#define PAD 16
typedef struct { unsigned char raw[PAD + 256 + PAD]; } guarded_t;

static unsigned char *gbuf(guarded_t *g)
{
    for (size_t i = 0; i < sizeof g->raw; i++) g->raw[i] = 0xA5;
    return g->raw + PAD;
}

static int guards_intact(const guarded_t *g, size_t used)
{
    for (size_t i = 0; i < PAD; i++)
        if (g->raw[i] != 0xA5) return 0;
    for (size_t i = PAD + used; i < sizeof g->raw; i++)
        if (g->raw[i] != 0xA5) return 0;
    return 1;
}

static void test_strlen(void)
{
    printf("my_strlen\n");
    CHECK(my_strlen("") == 0, "empty string");
    CHECK(my_strlen("a") == 1, "one char");
    CHECK(my_strlen("hello") == 5, "hello");
    CHECK(my_strlen("with\ttab and space") == 18, "whitespace counted");

    /* embedded high-bit bytes must not confuse the scan */
    const char hi[] = { (char)0x80, (char)0xFF, 'x', '\0' };
    CHECK(my_strlen(hi) == 3, "high-bit bytes");

    /* long string, and one that is not word-aligned at the start */
    char big[300];
    for (int i = 0; i < 299; i++) big[i] = 'z';
    big[299] = '\0';
    CHECK(my_strlen(big) == 299, "299-char string");
    CHECK(my_strlen(big + 1) == 298, "unaligned start");
    CHECK(my_strlen(big + 3) == 296, "unaligned start +3");
}

static void test_strnlen(void)
{
    printf("my_strnlen\n");
    CHECK(my_strnlen("hello", 10) == 5, "terminator before maxlen");
    CHECK(my_strnlen("hello", 5) == 5, "maxlen == length");
    CHECK(my_strnlen("hello", 3) == 3, "maxlen cuts short");
    CHECK(my_strnlen("hello", 0) == 0, "maxlen 0");
    CHECK(my_strnlen("", 4) == 0, "empty string");

    /* Unterminated buffer: reading past index 4 is a bug.
     * Under ASan this will trap outright if you overrun. */
    char *unterm = malloc(5);
    for (int i = 0; i < 5; i++) unterm[i] = 'q';
    CHECK(my_strnlen(unterm, 5) == 5, "no terminator within maxlen");
    free(unterm);
}

static void test_memcpy(void)
{
    printf("my_memcpy\n");
    guarded_t g;
    unsigned char *dst = gbuf(&g);
    unsigned char src[256];
    for (int i = 0; i < 256; i++) src[i] = (unsigned char)(i * 7 + 1);

    CHECK(my_memcpy(dst, src, 256) == dst, "returns dst");
    CHECK(memcmp(dst, src, 256) == 0, "256-byte copy contents");
    CHECK(guards_intact(&g, 256), "no write outside n bytes");

    /* n == 0 must touch nothing */
    dst = gbuf(&g);
    my_memcpy(dst, src, 0);
    CHECK(guards_intact(&g, 0), "n==0 writes nothing");

    /* odd sizes and misaligned src/dst */
    for (size_t n = 1; n <= 40; n++) {
        for (int off = 0; off < 4; off++) {
            dst = gbuf(&g);
            my_memcpy(dst + off, src + off, n);
            if (memcmp(dst + off, src + off, n) != 0) {
                CHECK(0, "size %zu offset %d contents", n, off);
                return;
            }
            if (!guards_intact(&g, (size_t)off + n)) {
                CHECK(0, "size %zu offset %d overran", n, off);
                return;
            }
        }
    }
    CHECK(1, "odd sizes x misalignment");
}

static void test_memmove(void)
{
    printf("my_memmove\n");
    unsigned char expect[128], actual[128];
    unsigned char seed[128];
    for (int i = 0; i < 128; i++) seed[i] = (unsigned char)(i + 1);

    CHECK(my_memmove(actual, seed, 16) == actual, "returns dst");

    /* forward overlap: dst > src */
    memcpy(expect, seed, 128); memmove(expect + 4, expect, 60);
    memcpy(actual, seed, 128); my_memmove(actual + 4, actual, 60);
    CHECK(memcmp(expect, actual, 128) == 0, "dst > src, overlapping");

    /* backward overlap: dst < src */
    memcpy(expect, seed, 128); memmove(expect, expect + 4, 60);
    memcpy(actual, seed, 128); my_memmove(actual, actual + 4, 60);
    CHECK(memcmp(expect, actual, 128) == 0, "dst < src, overlapping");

    /* one-byte shifts, the classic breaker */
    memcpy(expect, seed, 128); memmove(expect + 1, expect, 100);
    memcpy(actual, seed, 128); my_memmove(actual + 1, actual, 100);
    CHECK(memcmp(expect, actual, 128) == 0, "shift right by 1");

    memcpy(expect, seed, 128); memmove(expect, expect + 1, 100);
    memcpy(actual, seed, 128); my_memmove(actual, actual + 1, 100);
    CHECK(memcmp(expect, actual, 128) == 0, "shift left by 1");

    /* exactly-adjacent (no true overlap) and full self-copy */
    memcpy(expect, seed, 128); memmove(expect + 32, expect, 32);
    memcpy(actual, seed, 128); my_memmove(actual + 32, actual, 32);
    CHECK(memcmp(expect, actual, 128) == 0, "adjacent, non-overlapping");

    memcpy(actual, seed, 128); my_memmove(actual, actual, 128);
    CHECK(memcmp(actual, seed, 128) == 0, "dst == src");

    /* non-overlapping, plus n == 0 */
    memcpy(actual, seed, 128);
    my_memmove(actual + 64, seed, 0);
    CHECK(memcmp(actual, seed, 128) == 0, "n==0 changes nothing");

    /* exhaustive small shifts in both directions */
    for (size_t n = 1; n <= 40; n++) {
        for (int d = -8; d <= 8; d++) {
            unsigned char *e = expect + 40, *a = actual + 40;
            memcpy(expect, seed, 128); memmove(e + d, e, n);
            memcpy(actual, seed, 128); my_memmove(a + d, a, n);
            if (memcmp(expect, actual, 128) != 0) {
                CHECK(0, "n=%zu delta=%d", n, d);
                return;
            }
        }
    }
    CHECK(1, "exhaustive small shifts");
}

static void test_memcmp(void)
{
    printf("my_memcmp\n");
    CHECK(sgn(my_memcmp("abc", "abc", 3)) == 0, "equal");
    CHECK(sgn(my_memcmp("abc", "abd", 3)) < 0, "a < b");
    CHECK(sgn(my_memcmp("abd", "abc", 3)) > 0, "a > b");
    CHECK(sgn(my_memcmp("abc", "abd", 2)) == 0, "stops at n");
    CHECK(sgn(my_memcmp("abc", "xyz", 0)) == 0, "n==0 is equal");

    /* Bytes compare as UNSIGNED char: 0x80 must be greater than 0x01. */
    const unsigned char hi[] = { 0x80 };
    const unsigned char lo[] = { 0x01 };
    CHECK(sgn(my_memcmp(hi, lo, 1)) > 0, "0x80 > 0x01 (unsigned)");
    CHECK(sgn(my_memcmp(lo, hi, 1)) < 0, "0x01 < 0x80 (unsigned)");

    /* embedded NULs are just data here */
    const char a[] = { 'a', '\0', 'b' };
    const char b[] = { 'a', '\0', 'c' };
    CHECK(sgn(my_memcmp(a, b, 3)) < 0, "compares past embedded NUL");
    CHECK(sgn(my_memcmp(a, b, 2)) == 0, "equal up to embedded NUL");

    /* difference in the last byte of a long run */
    unsigned char x[200], y[200];
    for (int i = 0; i < 200; i++) x[i] = y[i] = (unsigned char)i;
    y[199] = 0xFF;
    CHECK(sgn(my_memcmp(x, y, 200)) < 0, "difference at final byte");
    CHECK(sgn(my_memcmp(x, y, 199)) == 0, "equal before final byte");

    /* agreement with libc on sign, over many random pairs */
    unsigned r = 12345;
    for (int t = 0; t < 500; t++) {
        unsigned char p[32], q[32];
        for (int i = 0; i < 32; i++) {
            r = r * 1103515245u + 12345u;
            p[i] = (unsigned char)(r >> 16);
            q[i] = (i % 3) ? p[i] : (unsigned char)(r >> 8);
        }
        if (sgn(my_memcmp(p, q, 32)) != sgn(memcmp(p, q, 32))) {
            CHECK(0, "random pair %d disagrees with libc", t);
            return;
        }
    }
    CHECK(1, "500 random pairs match libc sign");
}

int main(void)
{
    test_strlen();
    test_strnlen();
    test_memcpy();
    test_memmove();
    test_memcmp();

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail != 0;
}