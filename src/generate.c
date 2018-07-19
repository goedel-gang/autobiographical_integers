/*

Code to determine the autobiographic-ness of an integer: see
(https://oeis.org/A138480)

It also then scrapes through integers to print out the matching ones. It hopes
to be somewhat fast, at least compared to eg a Python script.

*/

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAX_DIGITS 10
#define NOTIFY_INTERVAL 100000

// macro used for debugging statements, while still exposing them to the
// compiler. be careful if using `else` nearby
#ifdef DEBUG
 #define D if(1)
#else
 #define D if(0)
#endif


// Integer exponentiation - used to generate 10^digits
unsigned long int_pow(int base, int exp) {
    unsigned long result = 1;
    while (exp) {
        if (exp & 1) {
           result *= base;
        }
        exp /= 2;
        base *= base;
    }
    return result;
}


// Get timestamp in ns - used to display processing rate
long long ns_timestamp(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return 1000000000 * spec.tv_sec + spec.tv_nsec;
}

// Determine if integer is autobiographic
bool is_autobiographical(unsigned long n) {
    int i = 0;
    unsigned int total = 0;
    unsigned int counts[MAX_DIGITS], digits[MAX_DIGITS];

    D printf("examining %lu\n", n);

    // Calculate the digits of the integer. Short circuits the function if the
    // digital total exceeds the possible number of digits (eg 9999999 has room
    // for way too many digits)
    while (n) {
        // while we're at it, initialise the array of digital counts
        counts[i] = 0;
        total += n % 10;
        if (total > MAX_DIGITS) {
            D printf("total too high: %d\n", total);
            return false;
        }
        digits[i++] = n % 10;
        n /= 10;

        D printf("digit found: %lu\n", n % 10);
        D printf("n reduced to: %lu\n", n);
    }

    // found the total number of digits
    total = i;
    D printf("total digits = %d\n", total);

    // build histogram of digital counts
    for (i--; i >= 0; i--) {
        D printf("incrementing count for %d (currently %d)\n",
                  digits[i], counts[digits[i]]);
        counts[digits[i]]++;
    }

    // check histogram against actual digits
    for (i = 0; i < total; i++) {
        if (counts[i] != digits[total - i - 1]) {
            D printf("failed count for digit: %d (%d != %d)\n",
                        i, counts[i], digits[total - i - 1]);
            return false;
        }
        D printf("succeeded count for digit: %d\n", i);
    }

    return true;
}

// loop, finding qualifying integers and print some stuff every now and then
int main() {
    unsigned long i;
    unsigned long top = int_pow(10, MAX_DIGITS);
    long long start = ns_timestamp();
    for (i = 1; i < top; i++) {
        if (i % NOTIFY_INTERVAL == 0) {
            // Use stderr to notify of progress
            fprintf(stderr, "\r%5.1f%% (%7.1e/%7.1e @ %7.1eguess/s)",
                             100.0 * i / top, (double)i, (double)top,
                             (double)1000000000 * i / (ns_timestamp() - start));
        }
        if (is_autobiographical(i)) {
            // "Clear" the stderr part before printing matched integer
            fprintf(stderr, "\r%41s\r", " ");
            printf("%lu\n", i);
        }
    }

    return 0;
}
