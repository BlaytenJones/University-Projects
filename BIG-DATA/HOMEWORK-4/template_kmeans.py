from pyspark import SparkContext
import sys
import numpy as np
import math

K = 10
DIM = 20
convergence = 20
centroid = []

def main():
    sc = SparkContext("local[*]", "Big Data KMeans")

    # Read the data points into a RDD
    data1 = sc.textFile(sys.argv[1])
    points = data1.map(lambda d: get_points(d))
    
    # Read the centroids. There are only 10 centroids, so they can be stored in a local list
    data2 = sc.textFile(sys.argv[2]).collect()
    global centroid     # Declares that centroid is the global variable
    for i in range(K):
        centroid.append(get_points(data2[i]))

    update_centroid = list(centroid)
    
    # To store the cost of each iteration
    result = []

    for k in range(convergence):
        centroid = list(update_centroid)

        #TODO: Assign points to centroids and compute the cost
        assigned = points.map(nearest_c)
        cost = assigned.map(lambda x: dist(x[1], centroid[x[0]])).sum()
        result.append(cost)

        #TODO: Update centroids as the average of all data points assigned
        update_centroid = assigned.map(lambda x: (x[0], (x[1], 1))) \
                         .reduceByKey(lambda x, y: (np.add(x[0], y[0]), x[1] + y[1])) \
                         .map(lambda x: (x[0], np.divide(x[1][0], x[1][1]))) \
                         .sortByKey() \
                         .map(lambda x: x[1]) \
                         .collect()

        # Check if any centroid has no points assigned to it
        for i in range(len(centroid)):
            if i >= len(update_centroid):
                update_centroid.append(centroid[i])
                
        # update centroid list
        centroid = update_centroid

    # Print the K centroids
    for i in range(K):
        print(update_centroid[i])
    # Print the costs
    for i in result:
        print(i)

def nearest_c(p):
    min_dist = float('inf')
    nc = -1
    #TODO: Find the nearest centroid and generate a key-value pair
    for i in range(len(centroid)):
        d = dist(p, centroid[i])
        if d < min_dist:
            min_dist = d
            nc = i
    return nc, p

# Compute the Euclidean distance between two points
def dist(p, q):
    return math.sqrt(sum((p_i - q_i) ** 2 for p_i, q_i in zip(p, q)))

# Parse the input file and get the coordinates of each point
def get_points(d):
    d_point = [float(x) for x in d.split('\t')]
    return d_point


if __name__ == "__main__":
    main()