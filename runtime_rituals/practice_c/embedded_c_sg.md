Based on the recruiter’s wording, **do not spend your remaining time grinding graph algorithms or dynamic programming**. This sounds like a practical C/embedded coding screen: implement small components from scratch, manipulate memory safely, and diagnose imperfect code.

Recent Anduril candidate reports also describe a mixture of embedded coding, general coding, and debugging, including working with existing circular-buffer-style code and questions involving memory management or concurrency. Treat those reports as anecdotal rather than guaranteed interview content. ([Glassdoor][1])

## Highest-priority topics

### 1. Pointers, arrays, and pointer arithmetic

Be completely comfortable with:

```c
int *p;
const int *p;
int *const p;
const int *const p;

*p
p + 1
&p[i]
*(p + i)
```

Know:

* Arrays decay to pointers in most expressions.
* `sizeof(array)` differs from `sizeof(pointer)`.
* Pointer arithmetic advances by the pointed-to type’s size.
* Subtracting two pointers is valid only within the same array object.
* One-past-the-end pointers may be formed but not dereferenced.
* Dangling, null, uninitialized, and out-of-bounds pointers.
* Overlapping memory and when `memmove` is required instead of `memcpy`.

**Likely problems:**

* Reverse an array in place using pointers.
* Implement `strlen`, `strnlen`, `memcpy`, `memmove`, or `memcmp`.
* Find an element without using array-index syntax.
* Debug an off-by-one buffer overrun.
* Parse bytes from a buffer while advancing a cursor pointer.

Practice implementing:

```c
size_t my_strlen(const char *s);
void *my_memcpy(void *dst, const void *src, size_t n);
void *my_memmove(void *dst, const void *src, size_t n);
```

For `memmove`, be ready to explain why copying backward is necessary when the destination overlaps the source at a higher address.

---

### 2. Bit manipulation

This was explicitly called out, so expect it.

Memorize these operations:

```c
value |=  (1u << bit);   // set
value &= ~(1u << bit);   // clear
value ^=  (1u << bit);   // toggle
(value >> bit) & 1u;     // test
```

Be able to:

* Set, clear, toggle, and test a bit.
* Extract a bit field.
* Insert a bit field without damaging neighboring bits.
* Count set bits.
* Detect whether a number is a power of two.
* Reverse bits or bytes.
* Build and interpret register masks.
* Avoid undefined behavior from invalid shifts.

Practice:

```c
uint32_t extract_field(uint32_t value,
                       unsigned offset,
                       unsigned width);

uint32_t insert_field(uint32_t reg,
                      uint32_t field,
                      unsigned offset,
                      unsigned width);
```

Also implement:

```c
unsigned count_set_bits(uint32_t x);
bool is_power_of_two(uint32_t x);
```

Expected patterns:

```c
x &= x - 1;             // clears lowest set bit
x != 0 && (x & (x - 1)) == 0
```

Be careful with:

```c
1 << 31
```

Prefer:

```c
UINT32_C(1) << 31
```

or:

```c
1u << bit
```

after validating the shift count.

---

### 3. Circular/ring buffers

This is probably the single most valuable embedded structure to practice. An Anduril interview report specifically mentions debugging a circular queue with a reader and writer, though that report involved C++. ([Glassdoor][1])

Be able to implement a fixed-capacity byte ring buffer:

```c
typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} ring_buffer_t;

bool ring_push(ring_buffer_t *rb, uint8_t value);
bool ring_pop(ring_buffer_t *rb, uint8_t *value);
bool ring_empty(const ring_buffer_t *rb);
bool ring_full(const ring_buffer_t *rb);
```

Know the two common representations:

1. Store a `count`.
2. Leave one slot unused to distinguish full from empty.

Common bugs:

* Incorrect modulo arithmetic.
* Confusing `head` and `tail`.
* Failing to handle capacity zero.
* Overwriting unread data.
* Off-by-one errors around wraparound.
* Race conditions between producer and consumer.

Practice manually tracing this:

```text
capacity = 4
push A, B, C
pop twice
push D, E, F
```

You should be able to state the indices and contents after every operation.

---

### 4. Strings and byte-buffer parsing

Embedded interviews often use arrays and strings because they expose pointer, bounds, and memory-management ability without requiring sophisticated algorithms. General embedded interview guidance likewise emphasizes arrays, strings, and bit manipulation. ([Embedded Related][2])

Practice:

* Reverse a string in place.
* Determine whether a buffer contains a valid null-terminated string.
* Tokenize without modifying the input.
* Parse a simple binary packet.
* Search for a byte pattern.
* Remove duplicates from a sorted array.
* Compact valid elements in place.

Example packet:

```text
Byte 0: message type
Byte 1: payload length
Bytes 2..N: payload
Final 2 bytes: checksum
```

Write something like:

```c
typedef struct {
    uint8_t type;
    const uint8_t *payload;
    size_t payload_len;
    uint16_t checksum;
} packet_t;

bool parse_packet(const uint8_t *buffer,
                  size_t buffer_len,
                  packet_t *packet);
```

The interviewer will care about whether you validate the length **before every access**.

---

### 5. Integer types, overflow, and signedness

Study `<stdint.h>`, even though it was not named in the message.

Know:

```c
uint8_t
int16_t
uint32_t
size_t
ptrdiff_t
UINT32_MAX
```

Be ready to discuss:

* Signed versus unsigned comparisons.
* Integer promotion of `uint8_t` and `uint16_t`.
* Signed overflow being undefined behavior.
* Unsigned arithmetic wrapping modulo (2^N).
* Narrowing conversions.
* Safe bounds checks.

Bad:

```c
if (offset + length <= buffer_size)
```

Potentially safer:

```c
if (offset <= buffer_size &&
    length <= buffer_size - offset)
```

Likely debugging question:

```c
for (size_t i = count - 1; i >= 0; --i) {
    ...
}
```

This never terminates normally because `size_t` is unsigned.

Possible correction:

```c
for (size_t i = count; i-- > 0;) {
    ...
}
```

---

### 6. Structs, alignment, padding, and memory layout

Know how to reason about:

```c
typedef struct {
    uint8_t type;
    uint32_t timestamp;
    uint16_t length;
} header_t;
```

Do not assume its size is `7`.

Study:

* Alignment and padding.
* `offsetof`.
* Why casting raw packet bytes to a struct may be unsafe.
* Unaligned access.
* Endianness.
* Strict aliasing.
* Packed structs and their tradeoffs.

Be able to implement explicit serialization:

```c
uint16_t read_u16_le(const uint8_t *p)
{
    return (uint16_t)p[0]
         | ((uint16_t)p[1] << 8);
}
```

And explain why this can be preferable to:

```c
return *(const uint16_t *)p;
```

The cast may introduce alignment, aliasing, and endianness problems.

---

### 7. `const`, `volatile`, and memory-mapped registers

You are very likely to be asked what `volatile` does.

Know that it:

* Tells the compiler that accesses are observable and must not simply be optimized away.
* Is appropriate for some hardware registers and variables modified outside normal program flow.
* Does **not** make an operation atomic.
* Does **not** provide thread synchronization.
* Does **not** fix a data race.

Example:

```c
#define STATUS_REG (*(volatile uint32_t *)0x40001000u)

bool peripheral_ready(void)
{
    return (STATUS_REG & READY_MASK) != 0u;
}
```

Be ready to explain each qualifier:

```c
volatile const uint32_t *status;
```

The pointed-to object is read-only through this pointer and may change externally.

---

### 8. Debugging existing C

They specifically warned that some problems may contain existing code. Practice reading code before rewriting it.

Look for:

* Null dereferences.
* Buffer overruns.
* Use-after-free.
* Returning pointers to local variables.
* Incorrect `sizeof`.
* Missing initialization.
* Assignment instead of comparison.
* Unsigned underflow.
* Incorrect loop bounds.
* Double increment.
* Memory leaks, if allocation is used.
* Incorrect ownership assumptions.
* Missing synchronization.
* Failure paths that leave state inconsistent.

Example:

```c
void copy_name(char *dst, const char *src)
{
    memcpy(dst, src, sizeof(src));
}
```

Problem: `sizeof(src)` is the size of the pointer, not the string or destination.

Another:

```c
char *format_value(int value)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}
```

Problem: returns a pointer to an object whose lifetime ended.

When debugging, narrate:

1. What invariant the code should maintain.
2. The smallest failing input.
3. The exact line where the invariant breaks.
4. The fix.
5. The regression tests you would add.

---

## Coding problems I would do immediately

In priority order:

1. Implement `memmove`.
2. Implement a fixed-capacity ring buffer.
3. Extract and insert register bit fields.
4. Parse a length-delimited binary packet safely.
5. Reverse an array using only pointers.
6. Count set bits in a `uint32_t`.
7. Detect host endianness and read little-endian integers.
8. Implement a fixed-size stack with no dynamic allocation.
9. Debug five examples involving unsigned underflow and incorrect `sizeof`.
10. Implement a simple finite-state machine.
11. Implement a byte-wise checksum or CRC-like loop.
12. Merge or compact sorted arrays in place.

You should also do one or two ordinary algorithm questions involving arrays, hash-style lookup, or two pointers. A recent Anduril embedded interview report described one general coding question in addition to an embedded question and debugging question, so completely ignoring standard coding problems would be risky. ([Glassdoor][3])

Reasonable general problems:

* Two Sum, including a simple (O(n^2)) C version if no hash table is available.
* Remove duplicates from sorted array.
* Merge two sorted arrays.
* Valid parentheses using a fixed-size stack.
* Binary search.
* Find the missing integer.
* Detect a cycle in a linked list.
* Reverse a singly linked list.

Do **not** prioritize advanced trees, graph traversal, tries, or dynamic programming unless you have already mastered the embedded material above.

## One realistic mock problem

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t storage[16];
    size_t read_index;
    size_t write_index;
    size_t count;
} byte_queue_t;

/*
 * Add all bytes to the queue.
 *
 * Return true on success.
 * Return false if queue is NULL, data is NULL when length is nonzero,
 * or insufficient capacity exists.
 *
 * If the operation fails, the queue must remain unchanged.
 */
bool queue_write(byte_queue_t *queue,
                 const uint8_t *data,
                 size_t length);
```

A good solution should:

* Validate pointers.
* Check available capacity without overflow.
* Preserve the queue on failure.
* Handle wraparound.
* Handle zero-length writes.
* Avoid dynamic allocation.
* Maintain `count <= 16`.
* Use clear variable names.
* Include tests for wraparound and exact capacity.

Possible implementation:

```c
bool queue_write(byte_queue_t *queue,
                 const uint8_t *data,
                 size_t length)
{
    if (queue == NULL) {
        return false;
    }

    if (length != 0u && data == NULL) {
        return false;
    }

    if (queue->count > sizeof(queue->storage)) {
        return false;
    }

    const size_t available =
        sizeof(queue->storage) - queue->count;

    if (length > available) {
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        queue->storage[queue->write_index] = data[i];
        queue->write_index =
            (queue->write_index + 1u) % sizeof(queue->storage);
    }

    queue->count += length;
    return true;
}
```

Then expect follow-ups:

* Avoid `%` if the capacity is a power of two.
* Make it safe for a producer and consumer.
* Support partial writes.
* Generalize the storage capacity.
* Explain what happens if an interrupt calls `queue_write`.
* Add a bulk read operation.

## What to say while coding

Interviewers frequently evaluate your process as much as the final code. Use this sequence:

> “First I’ll clarify the input constraints and required failure behavior. Then I’ll state the invariant. I’ll implement the simplest correct version, test boundary cases, and optimize afterward.”

Mention edge cases before coding:

* Null pointers.
* Empty inputs.
* Capacity zero.
* Maximum length.
* Integer overflow.
* Aliasing or overlapping buffers.
* Wraparound.
* Malformed input.
* Whether partial modification on failure is acceptable.

Compile mentally with warnings such as:

```bash
-Wall -Wextra -Wconversion -Wshadow -Werror
```

You do not need to write perfect production firmware immediately, but your code should show that you naturally think about bounds, ownership, types, and failure behavior.

## Best use of the next few hours

Spend roughly:

* **40%:** pointers, memory functions, arrays, and strings.
* **25%:** bit operations, integer behavior, and register manipulation.
* **20%:** ring buffers, packet parsing, and debugging.
* **10%:** linked lists, stacks, binary search, and simple array algorithms.
* **5%:** `volatile`, alignment, endianness, interrupts, and concurrency explanations.

The strongest bet is that you will be asked to write a modest C function whose algorithm is not especially difficult, but whose correctness depends on careful pointer use, bounds checking, bit operations, wraparound, or integer behavior.

[1]: https://www.glassdoor.com/Interview/1-Interviewer-first-asked-several-C-short-answer-questions-relating-to-memory-management-and-concurrency-concepts-2-Gi-QTN_6820510.htm?utm_source=chatgpt.com "Anduril Interview Question: 1. Interviewer ..."
[2]: https://www.embeddedrelated.com/showarticle/1503.php?utm_source=chatgpt.com "Cracking the (embedded) Coding Interview - Manasi Rajan"
[3]: https://www.glassdoor.com/Interview/Anduril-Senior-Embedded-Software-Engineer-Interview-Questions-EI_IE3546800.0%2C7_KO8%2C41.htm?utm_source=chatgpt.com "Anduril Senior Embedded Software Engineer interview ..."