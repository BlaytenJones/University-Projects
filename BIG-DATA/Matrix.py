import sys
from pyspark import SparkContext, SparkConf

def extract_info_M(line):
    elements = line.split(",")
    return int(elements[1]), ['M', int(elements[0]), float(elements[2])]

def extract_info_N(line):
    elements = line.split(",")
    return int(elements[0]), ['N', int(elements[1]), float(elements[2])]

def multiply_elements(pair):
    M_info = pair[1][0]
    N_info = pair[1][1]
    key = tuple([M_info[1], N_info[1]])
    value = M_info[2] * N_info[2]
    return key, value

if __name__ == "__main__":
    # create Spark context with necessary configuration
    sc = SparkContext("local", "Big Data HW One Matrix Multiplication")

    # obtain text files
    M = sc.textFile("./M.txt")
    N = sc.textFile("./N.txt")

    # creates MPair and NPair tuples; key is j which gives M, i, mij pair
    MPair = M.map(extract_info_M)
    NPair = N.map(extract_info_N)

    # create P matrix with multiple pik entries
    P = MPair.join(NPair)

    # aggregate non-zero elements of P matrix
    NewMatrix = P.map(multiply_elements).reduceByKey(lambda part1, part2: part1 + part2)

    # save result
    NewMatrix.saveAsTextFile("./result/")
    sc.stop()