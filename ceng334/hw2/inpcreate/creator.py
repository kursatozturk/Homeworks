from random import random, choice
import sys
import numpy as np


def rand_generator(constr, threshold):
    def rand():
        return int(random() * constr + threshold)
    return rand

total_worker_count = 150
total_ore_count = 6000

CAPACITY_CONSTRAINT = 60
INTERVAL_CONSTRAINT = 400

#interval_list = np.array(np.random.rand(4) * np.array([0.1, 0.05, 0.2, 0.2]) * INTERVAL_CONSTRAINT + 100, dtype= np.uint16)
interval_list = [100, 100, 300, 300]
print(interval_list)
rand = rand_generator(CAPACITY_CONSTRAINT, 2)

file_name = sys.argv[1]
input_file = open(file_name, 'w')


ore_probabilities = np.array([.31/.62, .20/.62, .11/.62])
count_iron, count_copper, count_coal = \
    np.random.multinomial(total_ore_count, ore_probabilities, size=1)[0]
worker_probabilities = np.array([0.13, 0.13, 0.13, 0.3, 0.1, 0.1, 0.11])

iron_miner, copper_miner, coal_miner, transporter,\
    iron_smelter, copper_smelter, foundry = \
    np.random.multinomial(total_worker_count, worker_probabilities, size=1)[0] + 1

iron_miner_tots = np.random.multinomial(count_iron, np.ones(iron_miner)/iron_miner, size=1)[0]
copper_miner_tots = np.random.multinomial(count_copper, np.ones(copper_miner)/copper_miner, size=1)[0]
coal_miner_tots = np.random.multinomial(count_coal, np.ones(coal_miner)/coal_miner, size=1)[0]

miner_count = iron_miner_tots[iron_miner_tots > 0].shape[0] + \
            copper_miner_tots[copper_miner_tots > 0].shape[0] + \
            coal_miner_tots[coal_miner_tots > 0].shape[0]


# begin writing
input_file.write(f'{miner_count}\n')
intervals = interval_list[0]
for tot in iron_miner_tots:
    if tot == 0:
        continue
    cap = rand()
    m_line = f'{intervals} {cap} 0 {tot}\n'
    input_file.write(m_line)

for tot in copper_miner_tots:
    if tot == 0:
        continue
    cap = rand()
    m_line = f'{intervals} {cap} 1 {tot}\n'
    input_file.write(m_line)

for tot in coal_miner_tots:
    if tot == 0:
        continue
    cap = rand()
    m_line = f'{intervals} {cap} 2 {tot}\n'
    input_file.write(m_line)

# transporter
input_file.write(f'{transporter}\n')
intervals = max(interval_list[1] // 5, 100)
for i in range(transporter):
    input_file.write(f'{intervals}\n')


# smelters
input_file.write(f'{iron_smelter + copper_smelter}\n')
intervals = interval_list[2]
for i in range(iron_smelter):
    cap = rand()
    s_line = f'{intervals} {cap} 0\n'
    input_file.write(s_line)

for i in range(copper_smelter):
    cap = rand()
    s_line = f'{intervals} {cap} 1\n'
    input_file.write(s_line)

# foundry
input_file.write(f'{foundry}\n')
intervals = interval_list[3]
for i in range(foundry):
    cap = rand()
    f_line = f'{intervals} {cap}\n'
    input_file.write(f_line)

print(f'\
IRON MINER COUNT : {iron_miner} \n\
COPPER MINER COUNT : {copper_miner} \n\
COAL MINER COUNT : {coal_miner} \n\
IRON SMELTER COUNT : {iron_smelter}\n\
COPPER SMELTER COUNT : {copper_smelter}\n\
FOUNDRY COUNT : {foundry}\n\
TRANSPORTER COUNT : {transporter}\n')


print(f'\
IRON COUNT : {count_iron}\n\
COPPER COUNT : {count_copper} \n\
COAL COUNT : {count_coal}\n')


exp_max_steel_ingot = min(count_iron, count_coal)
exp_max_iron_ingot = count_iron // 2
exp_max_copper_ingot = count_copper // 2

exp_min_iron_ingot = max(count_iron - count_coal, 0) // 2
exp_min_steel_ingot = 0
exp_min_copper_ingot = count_copper // 2

print(f'\
STEEL max: {exp_max_steel_ingot}, min: {exp_min_steel_ingot}\n\
IRON max: {exp_max_iron_ingot}, min: {exp_min_iron_ingot}\n\
COPPER max: {exp_max_copper_ingot}, min: {exp_min_copper_ingot}\n\
')
