import argparse

def squared_distance (d, p1, p2):
    sum_of_squares = 0.0
    for i in range(d):
        sum_of_squares += (p1[i]-p2[i])**2
    return sum_of_squares

def update_centroid(d, k, centroids, clusters_size, clusters_sum):
    for j in range(k):
        for i in range(d):
            centroids[j][i] = clusters_sum[j][i] / clusters_size[j]

def print_centroid(centroid):
    for i in range(len(centroid)-1):
        print("%.2f" % centroid[i], end=",")
    print("%.2f" %centroid[-1], end="")

def compare_centroids(k, d, prev, curr):
    for j in range(k):
        for i in range(d):
            if prev[j][i] != curr[j][i]:
                return False
    return True

if __name__ == '__main__':
    #Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("K", type=int, help="Number of clusters")
    parser.add_argument("N", type=int, help="Number of observations")
    parser.add_argument("d", type=int, help="Number of dimensions")
    parser.add_argument("MAX_ITER", type=int, help="Maximum number of iterations")
    args = parser.parse_args()
    k, n, d, MAX_ITER = args.K, args.N, args.d, args.MAX_ITER

    if (d < 1) or (MAX_ITER < 1) or (n < 1) or (k < 1) or (k >= n):
        raise Exception("invalid command line arguments")

    #Input handling
    observations = []
    while True:
        try:
            for line in input().split('\n'):
                observations.append([float(num) for num in line.split(',')])
        except EOFError:
            break
    
    centroids = [[num for num in obs] for obs in observations[:k]]

    #Clustering
    for iter in range(MAX_ITER):
        clusters_size = [0 for j in range(k)]
        clusters_sum = [[0.0 for i in range(d)] for j in range(k)]
        prev_centroids = [[num for num in centroid] for centroid in centroids]
        for observation in observations: 
            min_d, min_cluster = float('inf'), -1
            for j in range(k):
                distance = squared_distance(d, centroids[j], observation)
                if distance < min_d:
                    min_d = distance
                    min_cluster = j
            clusters_size[min_cluster] += 1
            for i in range(d):
                clusters_sum[min_cluster][i] += observation[i]
        update_centroid(d, k, centroids, clusters_size, clusters_sum)
        if compare_centroids(k, d, prev_centroids, centroids):
            break

    #Output
    for j in range(k):
        print_centroid(centroids[j])
        print()