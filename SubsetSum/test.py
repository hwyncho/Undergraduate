import random
import dynamic_programming as dp
import genetic_algorithm as ga


SET_SIZE = 1000
RANGE_MIN = 10000
RANGE_MAX = 100000

test_set = []
for i in range(SET_SIZE):
    test_set.append(random.randrange(RANGE_MIN, RANGE_MAX))

test_set_len = SET_SIZE

test_sum = 129348

dp_run_time, dp_result = dp.run(test_set, test_set_len, test_sum)
print('[DP] Time:{0}, Result:{1} \n'.format(dp_run_time, dp_result))

ga_run_time, ga_result = ga.run(test_set, test_set_len, test_sum, dp_run_time)
print('[GA] Time:{0}, Result:{1} \n'.format(ga_run_time, ga_result))
