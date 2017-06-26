import numpy as np
import time


def has_subset_sum():
    x = test_set_len + 1
    y = test_sum + 1

    # check : (sum + 1) x (set_len + 1)
    global check
    check = np.zeros([y, x], dtype=np.bool)

    # if sum == 0
    for i in range(0, x):
        check[0][i] = True

    # if sum != 0 and set_len == 0
    for i in range(1, y):
        check[i][0] = False

    for i in range(1, y):
        for j in range(1, x):
            check[i][j] = check[i][j - 1]
            if i >= test_set[j - 1]:
                check[i][j] = check[i][j] or check[i - test_set[j -1]][j - 1]

    return check[test_sum][test_set_len]


def get_subset(y, x, subset):
    if (y == 0) or (x == 0):
        result['subset'].append(subset.copy())
        subset.clear()
        return

    if check[y][x - 1]:
        get_subset(y, x - 1, subset)

    if (test_set[x - 1] <= y) and (check[y - test_set[x - 1]][x - 1]):
        subset.append(
            test_set[x - 1]
        )
        get_subset(
            y - test_set[x - 1],
            x - 1,
            subset
        )


def run(set_, set_len, sum_):
    global test_set
    test_set = set_

    global test_set_len
    test_set_len = set_len

    global test_sum
    test_sum = sum_

    global result
    result = {}

    time_start = time.time()
    result['has'] = has_subset_sum()
    time_end = time.time()

    result['subset'] = []
    if result['has']:
        get_subset(test_sum, test_set_len, [])

    result['count'] = len(result['subset'])

    run_time = time_end - time_start

    return run_time, result
