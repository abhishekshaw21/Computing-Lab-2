# Name : Abhishek Shaw
# Roll : 19CS60R48
# Question 1: (D-B-C+A) × E
# Mapper

import sys
matrixSize = 50
inputFileName = sys.argv[1]
with open(inputFileName) as f:
    content = f.readlines()
content = [x.strip() for x in content]
for line in content:
    line = line.strip()
    matrix, i, j, value = line.split(",")
    i = int(i)
    j = int(j)
    value = float(value)
    if(matrix == "D" or matrix == "A"):
        x = j  # Represent the row
        y = i * 100
        for _ in range(matrixSize):
            y = y + 1
            print(y, "\t", x, "\t", value, "\t+")
        #print(i, "\t", j, "\t", value, "\t+")
    elif(matrix == "B" or matrix == "C"):
        value = -value
        x = j  # Represent the row
        y = i * 100
        for _ in range(matrixSize):
            y = y + 1
            print(y, "\t", x, "\t", value, "\t+")
        #print(i, "\t", j, "\t", value, "\t+")
    elif(matrix == "E"):
        x = i  # Represent the column
        y = j
        for _ in range(matrixSize):
            y = y + 100
            print(y, "\t", x, "\t", value, "\t*")
