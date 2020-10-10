# Name : Abhishek Shaw
# Roll : 19CS60R48
#(D-B-C+A) × E
import sys
matrixSize = 50
currentI = None
currentJ = None
currentValue = 0
for line in sys.stdin:
    line = line.strip()
    i, j, value, operand = line.split('\t')
    i = int(i)
    j = int(j)
    if operand == "*":
        print(i, "\t", j, "\t", value, "\t*")
        continue
    value = float(value)
    if currentI == i and currentJ == j:
        currentValue += value
    else:
        if currentI:
            print(currentI, "\t", currentJ, "\t", currentValue, "\t+")
        currentValue = value
        currentI = i
        currentJ = j
if currentI and operand == "+":
    print(currentI, "\t", currentJ, "\t", currentValue, "\t", operand)
