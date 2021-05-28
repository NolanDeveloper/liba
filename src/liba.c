#include <liba.h>

#include <assert.h>

int64_t liba_integer_square_root(int64_t x) {
    assert(x >= 0);
    uint64_t t = x / 2;
    if (!t) {
        return x;
    }
    uint64_t next = (t + x / t) / 2;
    while (next < t) {
        t = next;
        next = (t + x / t) / 2;
    }
    return t;
}

int64_t liba_least_common_multiple(int64_t a, int64_t b) {
    return a * b / liba_greatest_common_divisor(a, b);
}

int64_t liba_greatest_common_divisor(int64_t a, int64_t b) {
    int64_t x, y;
    return liba_greatest_common_divisor_extended(a, b, &x, &y);
}

int64_t liba_greatest_common_divisor_extended(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    assert(a >= 0);
    assert(b >= 0);
    assert(x);
    assert(y);
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    lldiv_t d = lldiv(a, b);
    int64_t result = liba_greatest_common_divisor_extended(b, d.rem, x, y);
    int64_t tx = *x;
    int64_t ty = *y;
    *x = ty;
    *y = tx - ty * d.quot;
    return result;
}

int64_t liba_inverse_modulo(int64_t a, int64_t n) {
    int64_t gcd = liba_greatest_common_divisor(a, n);
    assert(1 == gcd);
    int64_t result = a % n;
    if (result < 0) {
        result += n;
    }
    return result;
}

int64_t liba_power_modulo(int64_t a, int64_t b, int64_t n) {
    assert(b >= 0);
    assert(n > 0);
    if (a < 0) {
        a = a % n;
        if (a < 0) {
            a += n;
        }
    }
    int64_t result = 1;
    while (b) {
        if (b % 2 == 1) {
            result = (a * result) % n;
        }
        a = (a * a) % n;
        b /= 2;
    }
    return result;
}

int64_t liba_binary_search(int64_t left, int64_t right, bool (*func)(int64_t)) {
    assert(left < right);
    assert(func);
    while (right - left > 1) {
        int64_t m = left + (right - left) / 2;
        if (func(m)) {
            right = m;
        } else {
            left = m;
        }
    }
    return func(left) ? left : right; // func(left) is False means for all x in [left, right) func(x) = False
}

static _Thread_local int64_t (*bs_func)(int64_t);

static bool low_helper(int64_t n) { return 0 <= bs_func(n); }

static bool high_helper(int64_t n) { return 0 < bs_func(n); }

void liba_binary_search_i64(int64_t left, int64_t right, int64_t (*func)(int64_t), int64_t *low, int64_t *high) {
    bs_func = func;
    *low = liba_binary_search(left, right, low_helper);
    *high = liba_binary_search(left, right, high_helper);
    bs_func = NULL;
}

double liba_binary_search_real(double left, double right, double (*func)(double), double eps) {
    assert(left < right);
    assert(func(left) <= 0 && 0 <= func(right));
    while (right - left > eps) {
        double m = left + (right - left) / 2;
        if (func(m) < 0) {
            left = m;
        } else {
            right = m;
        }
    }
    return left;
}

static void swap_i64(int64_t *a, int64_t *b) {
    int64_t t = *a;
    *a = *b;
    *b = t;
}

void liba_partition(int64_t *array, size_t size, int64_t k, size_t *low, size_t *high) {
    assert(array);
    assert(low);
    assert(high);
    if (!size) {
        *low = *high = 0;
        return;
    }
    size_t next_less = 0;
    size_t equal = size;
    size_t greater = size;
    while (next_less < equal) {
        if (array[next_less] < k) {
            ++next_less;
        } else if (array[next_less] == k) {
            --equal;
            swap_i64(&array[next_less], &array[equal]);
        } else {
            --greater;
            --equal;
            swap_i64(&array[next_less], &array[greater]);
            if (equal != greater) {
                swap_i64(&array[next_less], &array[equal]);
            }
        }
    }
    *low = next_less;
    *high = greater;
}

void liba_order_statistics(int64_t *array, size_t size, size_t k) {
    assert(array);
    while (size) {
        size_t low, high;
        liba_partition(array, size, array[rand() % size], &low, &high);
        if (low <= k && k < high) {
            break;
        }
        if (k < low) {
            size = low;
        } else {
            array = &array[high];
            size -= high;
            k -= high;
        }
    }
}
