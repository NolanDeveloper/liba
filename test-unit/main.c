#include <liba.h>
#include <float.h>
#include <math.h>
#include <acutest.h>

// Generate a random number in the interval [left, right).
static int random_between(int left, int right) {
    return left + rand() % (right - left);
}

static void test_integer_square_root(void) {
    TEST_ASSERT(0 == liba_integer_square_root(0));
    TEST_ASSERT(1 == liba_integer_square_root(1));
    for (int64_t n = 2; n < INT64_MAX / n; n = n * random_between(101, 199) / 100) {
        TEST_ASSERT(n - 1  == liba_integer_square_root(n * n - 1));
        TEST_ASSERT(n - 0 == liba_integer_square_root(n * n + 0));
        TEST_ASSERT(n - 0 == liba_integer_square_root(n * n + 1));
    }
}

static void test_least_common_multiple(void) {
    TEST_ASSERT(1 == liba_least_common_multiple(1, 1));
    TEST_ASSERT(2 == liba_least_common_multiple(2, 1));
    TEST_ASSERT(2 == liba_least_common_multiple(1, 2));
    TEST_ASSERT(300 == liba_least_common_multiple(100, 3));
    TEST_ASSERT(300 == liba_least_common_multiple(3, 100));
    TEST_ASSERT(2 * 2 * 3 * 7 * 11 == liba_least_common_multiple(2 * 2 * 3 * 7, 2 * 2 * 7 * 11));
}

static void test_case_greatest_common_divisor(int64_t a, int64_t b, int64_t gcd) {
    int64_t x, y;
    TEST_ASSERT(gcd == liba_greatest_common_divisor_extended(a, b, &x, &y));
    TEST_ASSERT(gcd == a * x + b * y);
}

static void test_greatest_common_divisor(void) {
    test_case_greatest_common_divisor(1, 1, 1);
    for (int64_t n = 2; n < INT64_MAX / 200; n = n * random_between(101, 199) / 100) {
        test_case_greatest_common_divisor(n, n, n);
        test_case_greatest_common_divisor(n + 1, n, 1);
        test_case_greatest_common_divisor(2 * n, n, n);
        test_case_greatest_common_divisor(3 * n, 2 * n, n);
        test_case_greatest_common_divisor(7 * n, 4 * n, n);
    }
    test_case_greatest_common_divisor(INT64_MAX - 1, INT64_MAX, 1);
}

static void test_case_inverse_modulo(int64_t a, int64_t n) {
    int64_t b = liba_inverse_modulo(a, n);
    TEST_ASSERT(1 == (a * b) % n);
}

static void test_inverse_modulo(void) {
    int64_t primes[] = { 2, 3, 5, 7, 11, 13, 17, 23, 1000000007 };
    for (int64_t n = 2; n < INT64_MAX / 200; n = n * random_between(101, 199) / 100) {
        for (size_t i = 0; i < sizeof(primes) / sizeof(*primes); ++i) {
            if (n % primes[i] != 0 && n < INT32_MAX / primes[i]) {
                test_case_inverse_modulo(primes[i], n);
                test_case_inverse_modulo(n, primes[i]);
            }
        }
    }
    test_case_inverse_modulo(INT32_MAX - 1, INT32_MAX );
    test_case_inverse_modulo(INT32_MAX, INT32_MAX - 1 );
}

static void test_power_modulo(void) {
    int64_t a = 2;
    int64_t b = 3;
    int64_t c = 5;
    for (int i = 1; i < 1000; ++i) {
        TEST_ASSERT(a == liba_power_modulo(2, i, 107));
        TEST_ASSERT(b == liba_power_modulo(3, i, 107));
        TEST_ASSERT(c == liba_power_modulo(5, i, 107));
        a = (a * 2) % 107;
        b = (b * 3) % 107;
        c = (c * 5) % 107;
    }
}

static int64_t zero_begin;
static int64_t zero_end;

static bool growing_function(int64_t x) {
    return zero_begin <= x;
}

static int64_t growing_function_i64(int64_t x) {
    if (x < zero_begin) {
        return x - zero_begin;
    }
    if (zero_end <= x) {
        return x - zero_end + 1;
    }
    return 0;
}

static double growing_function_real(double x) {
    return x - zero_begin;
}

static bool essentially_equal(double a, double b, double epsilon) {
    return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

static void test_binary_search(void) {
    for (int64_t n = 1; n < INT64_MAX / 1000; n = n * random_between(1001, 9999) / 10) {
        int64_t low, high;
        zero_begin = n;
        TEST_ASSERT(n == liba_binary_search(INT64_MIN, INT64_MAX, growing_function));

        zero_begin = -n;
        TEST_ASSERT(-n == liba_binary_search(INT64_MIN, INT64_MAX, growing_function));

        zero_begin = n;
        zero_end = n + 100;
        liba_binary_search_i64(INT64_MIN, INT64_MAX, growing_function_i64, &low, &high);
        TEST_ASSERT(zero_begin == low);
        TEST_ASSERT(zero_end == high);

        zero_begin = -n;
        zero_end = -n + 100;
        liba_binary_search_i64(INT64_MIN, INT64_MAX, growing_function_i64, &low, &high);
        TEST_ASSERT(zero_begin == low);
        TEST_ASSERT(zero_end == high);

        double x = liba_binary_search_real(-DBL_MAX, DBL_MAX, growing_function_real);
        TEST_ASSERT(essentially_equal(zero_begin, x, 0.00001));
    }
}

static void test_order_statistic(void) {
    int64_t array[100];
    for (int n = 0; n < 100; ++n) {
        int offset = random_between(1, 10000);
        for (int64_t i = 0; i < sizeof(array) / sizeof(*array); ++i) {
            array[i] = ((107 * (i + offset)) % 100) / 2;
        }
        // at this point array consists of numbers from 0 to 50
        // in pseudo random order each repeated twice
        liba_order_statistic(array, sizeof(array) / sizeof(*array), n);
        TEST_ASSERT(array[n] == n / 2);
    }
}

TEST_LIST = {
{ "integer-square-root", test_integer_square_root },
{ "least-common-multiple", test_least_common_multiple },
{ "greatest-common-divisor", test_greatest_common_divisor },
{ "inverse-modulo", test_inverse_modulo },
{ "power-modulo", test_power_modulo },
{ "binary-search", test_binary_search },
{ "order-statistic", test_order_statistic },
{ NULL, NULL }     /* zeroed record marking the end of the list */
};
