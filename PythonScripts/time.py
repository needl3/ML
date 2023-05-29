import timeit
import random
import numpy as np
import time

def prettyPrintMatrix(mat):
    for i in mat:
        for j in i:
            print(j, end='\t')
        print('\n')

def unvectorized(array1, array2):
    finalMat = [[] for _ in range(len(array2))]
    for a1r in range(len(array1)):
        for a2c in range(len(array2[0])):
            sum = 0
            for i in range(len(array2)):
                sum += array1[a1r][i] * array2[i][a2c]
            finalMat[a1r].append(sum)
    return finalMat

def vectorized(array1, array2):
    return np.dot(array1, array2)

if __name__ == '__main__':
    array1 = [[random.randint(1,100) for _ in range(500)] for _ in range(250)]
    array2 = [[random.randint(1,100) for _ in range(250)] for _ in range(500)]

    if len(array1[0]) != len(array2) and len(array1) != len(array2[0]):
        print("Invalid matrix structure")
        exit(1)

    elaspedTime = timeit.timeit(lambda: unvectorized(array1, array2), number=5)
    print("Unvectorized took: ", elaspedTime, " seconds")
    # prettyPrintMatrix(finalDotProduct1)

    array1 = np.array(array1)
    array2 = np.array(array2)
    elaspedTime = timeit.timeit(lambda: vectorized(array1, array2), number=5) 
    print("Vectorized took: ", elaspedTime, " seconds")
    # prettyPrintMatrix(finalDotProduct2)

