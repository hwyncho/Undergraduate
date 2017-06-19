import random
import time

def generate_chromosomes():
    n = test_set_len
    chromosomes = []
    for i in range(n):
        chromosomes.append(
            str('{0:b}'.format(pow(2, i)).zfill(n))
        )

    return chromosomes


def calculate_fitness(chromosome):
    temp = 0
    for i in range(len(chromosome)):
        if chromosome[i] == '1':
            temp += test_set[i]

    fitness = round(float(temp / test_sum), 2)

    return {
        'chromosome':chromosome,
        'fitness':fitness
    }


def selection(population, num=2):
    fitness = [population[i]['fitness'] for i in range(len(population))]
    sum_fitness = sum(fitness)

    relative_fitness = [(f / sum_fitness) for f in fitness]
    probability = [sum(relative_fitness[:i+1]) for i in range(len(relative_fitness))]

    selected = []
    for _ in range(num):
        r = random.random()
        for (i, indivisual) in enumerate(population):
            if r <= probability[i]:
                selected.append(indivisual)
                break

    return selected


def crossover(chromosome_1, chromosome_2):
    n = len(chromosome_1)
    new_chromosome = chromosome_1[int(n / 2):] + chromosome_2[:int(n / 2)]

    return new_chromosome


def mutation(chromosome):
    r = random.randrange(1, 101)
    if r == 1:
        i = random.randrange(0, len(chromosome))
        if chromosome[i] == '0':
            new_chromosome = chromosome[:i] + '1' + chromosome[(i + 1):]
        else:
            new_chromosome = chromosome[:i] + '0' + chromosome[(i + 1):]

        return calculate_fitness(new_chromosome)

    return calculate_fitness(chromosome)


def replace(population, new_indivisual):
    max_distance = 0.0
    idx = 0
    for i in range(len(population)):
        fitness = population[i]['fitness']
        distance = abs(1.0 - fitness)
        if max_distance < distance:
            max_distance = distance
            idx = i

    population.pop(idx)
    population.append(new_indivisual)

    return population


def get_best(population):
    min_distance = 1.0
    idx = 0
    for i in range(len(population)):
        fitness = population[i]['fitness']
        distance = abs(1.0 - fitness)
        if min_distance > distance:
            min_distance = distance
            idx = i

    return population[idx]


def run(set_, set_len, sum_, run_time):
    global test_set
    test_set = set_

    global test_set_len
    test_set_len = set_len

    global test_sum
    test_sum = sum_

    time_start = time.time()
    time_end = time.time()

    chromosomes = generate_chromosomes()

    population = []
    for chromosome in chromosomes:
        population.append(calculate_fitness(chromosome))

    while((time_end - time_start) < run_time):
        selected = selection(population, 2)
        new_indivisual = mutation(
            crossover(selected[0]['chromosome'], selected[1]['chromosome'])
        )
        population = replace(population, new_indivisual)

        if get_best(population)['fitness'] == 1.0:
            time_end = time.time()
            run_time = time_end - time_start
            break

        time_end = time.time()

    return run_time, get_best(population)
