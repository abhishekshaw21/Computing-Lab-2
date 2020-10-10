# Name : Abhishek Shaw
# Roll : 19CS60R48
#(D-B-C+A) × E
import sys
currentIJ = None
currentM = None
currentN = None
for line in sys.stdin:
    line = line.strip()
    mn, ij, InputValue, operand = line.split('\t')
    ij = int(ij)
    m = int(int(mn) / 100)
    n = int(mn) % 100
    InputValue = float(InputValue)
    # print(m,n,i,j,value,operand)
    if currentM == m and currentN == n:
        if currentIJ == ij:
            if operand == "*":
                prodValue = InputValue
                pass
            else:
                sumValue += InputValue
        else:
            value += sumValue * prodValue
            currentIJ = ij
            sumValue = 0
            prodValue = 0
            if operand == "*":
                prodValue = InputValue
            else:
                sumValue = InputValue
    else:
        if currentM:
            value += sumValue * prodValue
            print(currentM, ",", currentN, ",", value)
        value = 0
        sumValue = 0
        prodValue = 0
        currentIJ = ij
        currentM = m
        currentN = n
        if operand == "*":
            prodValue = InputValue
        else:
            sumValue = InputValue

value += sumValue * prodValue
print(currentM, ",", currentN, ",", value)
