import numpy as np

f = open("wave1.txt", "r")
data = f.read()
splitted = data.split("\n")
splitted.pop()
list_data = list(map(int, splitted))
data = np.array(list_data)

max_value = max(data)
min_value = min(data)

real_max = 1.30
real_min = -1.22

print(max_value, min_value)
coeff1 = real_max / max_value
coeff2 = real_min / min_value

avg_coeff = (coeff1 + coeff2)/2
print(avg_coeff)
