/* LIBrary of standard Algorithms */

#ifndef LIBA_H
#define LIBA_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root.
 * Preconditions: x >= 0 */
int64_t liba_integer_square_root(int64_t x);

/* Least common multiple.
 * Preconditions: a >= 0 && b >= 0
 * Time complexity: O(log(min(a, b))) */
int64_t liba_least_common_multiple(int64_t a, int64_t b);

/* Greatest common divisor.
 * Preconditions: a >= 0 && b >= 0
 * Time complexity: O(log min(a, b)) */
int64_t liba_greatest_common_divisor(int64_t a, int64_t b);

/* Greatest common divisor and Bezout coefficients
 * gcd(a, b) = a * x + b * y.
 * Preconditions: a >= 0 && b >= 0
 * Time complexity: O(log min(a, b)) */
int64_t liba_greatest_common_divisor_extended(int64_t a, int64_t b, int64_t *x, int64_t *y);

/* Compute multiplicative inverse modulo n. a and n must be co-prime.
 * '(liba_inverse_modulo(a, n) * a) % n = 1'.
 * Preconditions: 0 < n && n <= INT32_MAX (!!!NOT INT64_MAX!!!)
 * Time complexity: O(log n) */
int64_t liba_inverse_modulo(int64_t a, int64_t n);

/* 'pow(a, b) % n'
 * Preconditions: 0 <= b && 0 < n
 * Time complexity: O(log b) */
int64_t liba_power_modulo(int64_t a, int64_t b, int64_t n);

/* Find n in [left, right] such that
 * func [left, n)  = { False }
 * func [n, right) = { True }
 * n == left will be returned in case function is always True
 * n == right will be returned in case function is always False
 * Preconditions: left < right && func is non-decreasing.
 * Time complexity: O(log n) where n = right - left. */
int64_t liba_binary_search(int64_t left, int64_t right, bool (*func)(int64_t));

/* Find low and high [left, right] such that
 * for x in [left, low)   func(x) < 0
 * for x in [low, high)   func(x) = 0
 * for x in [high, right) func(x) > 0
 * low == high may be returned in case when function never goes to zero.
 * low == high == right will be returned in case function is always negative.
 * low == high == left will be returned in case function is always positive.
 * Preconditions: left < right && func is non-decreasing.
 * Time complexity: O(log n) where n = right - left. */
void liba_binary_search_i64(int64_t left, int64_t right, int64_t (*func)(int64_t), int64_t *low, int64_t *high);

/* Find x such that abs(x_0 - x) < eps where func(x_0) = 0
 * Preconditions: left < right && func is continuous or non-decreasing && func(left) <= 0 && func(right) >= 0
 * Time complexity: O(log n) where n = (right - left) / eps. */
double liba_binary_search_real(double left, double right, double (*func)(double));

/* Permute elements of array and find low and high such that
 * array[j] <  k for j < low
 * array[j] == k for     low <= j < high
 * array[j] >  k for                high <= j
 * Not stable.
 * Time complexity: O(n) */
void liba_partition(int64_t *array, size_t size, int64_t k, size_t *low, size_t *high);

/* Permute elements of array such that array[k] is the k-th smallest value.
 * Randomized.
 * Time complexity: O(n) on average, O(n^2) in the worst case. */
void liba_order_statistic(int64_t *array, size_t size, size_t k);

#endif
