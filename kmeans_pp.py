import argparse
import numpy as np
import pandas as pd
import mykmeanssp

def squared_euclidian_dist(observations, last_Centroid):
    return np.power(observations-last_Centroid, 2).sum(axis=1)

def k_means_pp(k, n, d, observations):
    np.random.seed(0)
    first_index = np.random.choice(np.arange(n))
    indices = [first_index]
    if k == 1:
        return indices
    current_distances = squared_euclidian_dist(observations, observations.iloc[indices[0]])
    for j in np.arange(k-1):
        if j > 0:
            next_distances = squared_euclidian_dist(observations, observations.iloc[indices[j]])
            current_distances = np.minimum(current_distances, next_distances)
        probabilities = np.divide(current_distances, np.sum(current_distances))
        next_index = np.random.choice(np.arange(n), p=probabilities)
        indices.append(next_index)
    return indices

if __name__ == '__main__':
    #Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("K", type=int, help="Number of clusters")
    parser.add_argument("N", type=int, help="Number of observations")
    parser.add_argument("d", type=int, help="Number of dimensions")
    parser.add_argument("MAX_ITER", type=int, help="Maximum number of iterations")
    parser.add_argument("filename", type=str, help="Path to the observations' file")
    args = parser.parse_args()
    k, n, d, MAX_ITER, filename = args.K, args.N, args.d, args.MAX_ITER, args.filename

    if (d < 1) or (MAX_ITER < 1) or (n < 1) or (k < 1) or (k >= n):
        raise Exception("invalid command line arguments")

    # input handling
    observations = pd.read_csv(filename, dtype = np.float64, header = None)

    # k_means++ init
    indices = k_means_pp(k, n, d, observations)

    #Indices print
    print(','.join(str(i) for i in indices))

    #DataFrames to 2-dimensional lists for C-API
    observations_list = observations.to_numpy().tolist()

    # k_means clustring
    mykmeanssp.kmeans_runner(k, n, d, MAX_ITER, indices, observations_list)
