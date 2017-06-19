import numpy as np
import time

def is_subset_sum():
    x = test_set_len + 1
    y = test_sum + 1
    # subset : (sum + 1) x (set_len + 1)
    subset = np.zeros([y, x], dtype=np.bool)

    # if sum == 0
    for i in range(0, x):
        subset[0][i] = True

    # if sum != 0 and set_len == 0
    for i in range(1, y):
        subset[i][0] = False

    for i in range(1, y):
        for j in range(1, x):
            subset[i][j] = subset[i][j - 1]
            if i >= test_set[j - 1]:
                subset[i][j] = subset[i][j] or subset[i - test_set[j -1]][j - 1]

    return subset[test_sum][test_set_len]


def run(set_, set_len, sum_):
    global test_set
    test_set = set_

    global test_set_len
    test_set_len = set_len

    global test_sum
    test_sum = sum_

    time_start = time.time()
    result = is_subset_sum()
    time_end = time.time()
    run_time = time_end - time_start

    return run_time, result
