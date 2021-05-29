#include <stdio.h>
#include <liba.h>
#include <float.h>
#include <math.h>
#define MUNIT_ENABLE_ASSERT_ALIASES
#include <munit.h>

static MunitResult test_integer_square_root(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    assert_int64(0, ==, liba_integer_square_root(0));
    assert_int64(1, ==, liba_integer_square_root(1));
    for (int64_t n = 2; n < INT64_MAX / n; n = n * munit_rand_int_range(101, 199) / 100) {
        assert_int64(n - 1, ==, liba_integer_square_root(n * n - 1));
        assert_int64(n - 0, ==, liba_integer_square_root(n * n + 0));
        assert_int64(n - 0, ==, liba_integer_square_root(n * n + 1));
    }
    return MUNIT_OK;
}

static MunitResult test_least_common_multiple(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    assert_int64(1, ==, liba_least_common_multiple(1, 1));
    assert_int64(2, ==, liba_least_common_multiple(2, 1));
    assert_int64(2, ==, liba_least_common_multiple(1, 2));
    assert_int64(300, ==, liba_least_common_multiple(100, 3));
    assert_int64(300, ==, liba_least_common_multiple(3, 100));
    assert_int64(2 * 2 * 3 * 7 * 11, ==, liba_least_common_multiple(2 * 2 * 3 * 7, 2 * 2 * 7 * 11));
    return MUNIT_OK;
}

static void test_case_greatest_common_divisor(int64_t a, int64_t b, int64_t gcd) {
    int64_t x, y;
    assert_int64(gcd, ==, liba_greatest_common_divisor_extended(a, b, &x, &y));
    assert_int64(gcd, ==, a * x + b * y);
}

static MunitResult test_greatest_common_divisor(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    test_case_greatest_common_divisor(1, 1, 1);
    for (int64_t n = 2; n < INT64_MAX / 200; n = n * munit_rand_int_range(101, 199) / 100) {
        test_case_greatest_common_divisor(n, n, n);
        test_case_greatest_common_divisor(n + 1, n, 1);
        test_case_greatest_common_divisor(2 * n, n, n);
        test_case_greatest_common_divisor(3 * n, 2 * n, n);
        test_case_greatest_common_divisor(7 * n, 4 * n, n);
    }
    test_case_greatest_common_divisor(INT64_MAX - 1, INT64_MAX, 1);
    return MUNIT_OK;
}

static void test_case_inverse_modulo(int64_t a, int64_t n) {
    int64_t b = liba_inverse_modulo(a, n);
    if (1 != (a * b) % n) {
        munit_errorf("liba_inverse_modulo(%"PRIi64", %"PRIi64") = %"PRIi64"\n"
                     "%"PRIi64" * %"PRIi64" %% %"PRIi64" = %"PRIi64" but must be 1\n",
                     a, n, b, a, b, n, b);
    }
}

static MunitResult test_inverse_modulo(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    int64_t primes[] = { 2, 3, 5, 7, 11, 13, 17, 23, 1000000007 };
    for (int64_t n = 2; n < INT64_MAX / 200; n = n * munit_rand_int_range(101, 199) / 100) {
        for (size_t i = 0; i < sizeof(primes) / sizeof(*primes); ++i) {
            if (n % primes[i] != 0 && n < INT32_MAX / primes[i]) {
                test_case_inverse_modulo(primes[i], n);
                test_case_inverse_modulo(n, primes[i]);
            }
        }
    }
    test_case_inverse_modulo(INT32_MAX - 1, INT32_MAX );
    test_case_inverse_modulo(INT32_MAX, INT32_MAX - 1 );
    return MUNIT_OK;
}

static MunitResult test_power_modulo(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    int64_t a = 2;
    int64_t b = 3;
    int64_t c = 5;
    for (int i = 1; i < 1000; ++i) {
        assert_int64(a, ==, liba_power_modulo(2, i, 107));
        assert_int64(b, ==, liba_power_modulo(3, i, 107));
        assert_int64(c, ==, liba_power_modulo(5, i, 107));
        a = (a * 2) % 107;
        b = (b * 3) % 107;
        c = (c * 5) % 107;
    }
    return MUNIT_OK;
}

static __thread int64_t zero_begin;
static __thread int64_t zero_end;

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

static MunitResult test_binary_search(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    for (int64_t n = 1; n < INT64_MAX / 1000; n = n * munit_rand_int_range(1001, 9999) / 10) {
        int64_t low, high;
        zero_begin = n;
        assert_int64(n, ==, liba_binary_search(INT64_MIN, INT64_MAX, growing_function));

        zero_begin = -n;
        assert_int64(-n, ==, liba_binary_search(INT64_MIN, INT64_MAX, growing_function));

        zero_begin = n;
        zero_end = n + 100;
        liba_binary_search_i64(INT64_MIN, INT64_MAX, growing_function_i64, &low, &high);
        assert_int64(zero_begin, ==, low);
        assert_int64(zero_end, ==, high);

        zero_begin = -n;
        zero_end = -n + 100;
        liba_binary_search_i64(INT64_MIN, INT64_MAX, growing_function_i64, &low, &high);
        assert_int64(zero_begin, ==, low);
        assert_int64(zero_end, ==, high);

        double x = liba_binary_search_real(-DBL_MAX, DBL_MAX, growing_function_real);
        assert_true(essentially_equal(zero_begin, x, 0.00001));
    }
    return MUNIT_OK;
}

static MunitResult test_order_statistic(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;
    int64_t array[100];
    for (int n = 0; n < 100; ++n) {
        int offset = munit_rand_int_range(1, 10000);
        for (size_t i = 0; i < sizeof(array) / sizeof(*array); ++i) {
            array[i] = ((107 * (i + offset)) % 100) / 2;
        }
        // at this point array consists of numbers from 0 to 50
        // in pseudo random order each repeated twice
        liba_order_statistic(array, sizeof(array) / sizeof(*array), n);
        assert_int64(array[n], ==, n / 2);
    }
    return MUNIT_OK;
}

static MunitTest tests[] = {
        { "/integer-square-root", test_integer_square_root, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { "/least-common-multiple", test_least_common_multiple, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { "/greatest-common-divisor", test_greatest_common_divisor, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { "/inverse-modulo", test_inverse_modulo, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { "/power-modulo", test_power_modulo, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { "/binary-search", test_binary_search, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { "/order-statistic", test_order_statistic, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL  },
        { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};

static const MunitSuite suite = {
        "/my-tests",
        tests,
        NULL,
        1,
        MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}
