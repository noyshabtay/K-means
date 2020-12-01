/*
 ============================================================================
 Name        : kmeans.c
 Author      : Noy Shabtay & Roy Naftali
 Version     :
 Copyright   : Your copyright notice
 Description : Kmeans clustering algorithm implementation.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int k;
int n;
int d;
int max_iter;

void kmeans(double**, double**);
void copy_centroids(double**, double**);
double squared_distance(int, int, double**, double**);
void update_centroids(double**, int*, double**);
int compare_centroids(double**, double**);
void print_centroids(double**);


int main(int argc, char* argv[]) {
	int i;
	int j;
	double **observations;
	double **centroids;
	double n1;
	char c;

	if (argc != 5) {
		printf("invalid command line arguments");
		return 0;
	}
	k = atoi(argv[1]);
	n = atoi(argv[2]);
	d = atoi(argv[3]);
	max_iter = atoi(argv[4]);
	if ((d < 1) || (max_iter < 1) || (n < 1) || (k < 1) || (k >= n)) {
		printf("invalid command line arguments");
		return 0;
	}

	observations = (double**)malloc(n*sizeof(double*));
	assert(observations != NULL);
	for(i=0; i<n; i++){
		observations[i] = (double*)malloc(d*sizeof(double));
		assert(observations[i] != NULL);
	}
	centroids = (double**)malloc(k*sizeof(double*));
	assert(centroids != NULL);
	for(j=0; j<k; j++){
		centroids[j] = (double*)malloc(d*sizeof(double));
		assert(centroids[j] != NULL);
	}

	j = 0;
	i = 0;
	while (scanf("%lf%c", &n1, &c) == 2) {
		observations[j][i] = n1;
		i++;
		if (i == d) {
			j++;
			i = 0;
		}
	}

	for(j=0; j<k; j++) {
		for(i=0; i<d; i++) {
			centroids[j][i] = observations[j][i];
		}
	}

	kmeans(observations, centroids);

	for(j=0; j<k; j++){
		free(centroids[j]);
	}
	free(centroids);
	for(i=0; i<n; i++){
		free(observations[i]);
	}
	free(observations);

	return 0;
}

void kmeans(double** observations, double** centroids) {
	int i;
	int j;
	int obs;
	int iter;
	int* clusters_size;
	double min_d;
	int min_cluster;
	double **prev_centroids;
	double **clusters_sum;
	double distance;

	prev_centroids = (double**)malloc(k*sizeof(double*));
	assert(prev_centroids != NULL);
	for(j=0; j<k; j++){
		prev_centroids[j] = (double*)malloc(d*sizeof(double));
		assert(prev_centroids[j] != NULL);
	}

	for(iter=0; iter<max_iter; iter++) {
		clusters_size = calloc(k, sizeof(int));
		assert(clusters_size != NULL);
		clusters_sum = (double**)calloc(k, sizeof(double*));
		assert(clusters_sum != NULL);
		for(j=0; j<k; j++){
			clusters_sum[j] = (double*)calloc(d, sizeof(double));
			assert(clusters_sum[j] != NULL);
		}

		copy_centroids(centroids ,prev_centroids);

		for(obs=0; obs<n; obs++){
			min_d = 1.0 / 0.0;
			min_cluster = -1;
			for(j=0; j<k; j++) {
				distance = squared_distance(j, obs, observations, centroids);
				if (distance < min_d) {
					min_d = distance;
					min_cluster = j;
				}
			}
			clusters_size[min_cluster]++;
			for (i=0; i<d; i++) {
				clusters_sum[min_cluster][i] += observations[obs][i];
			}
		}
		update_centroids(centroids, clusters_size, clusters_sum);
		for(j=0; j<k; j++){
			free(clusters_sum[j]);
		}
		free(clusters_sum);
		free(clusters_size);
		if (compare_centroids(prev_centroids, centroids)) {
			break;
			}
	}
	print_centroids(centroids);

	for(j=0; j<k; j++){
		free(prev_centroids[j]);
	}
	free(prev_centroids);
}

void copy_centroids(double** centroids ,double** prev_centroids) {
	int i;
	int j;
	for(j=0; j<k; j++) {
		for(i=0; i<d; i++) {
			prev_centroids[j][i] = centroids[j][i];
		}
	}
}

double squared_distance(int j, int obs, double** observations, double** centroids) {
	int i;
	double sum_of_squares = 0;
	double tmp;
	for(i=0; i<d; i++) {
		tmp = observations[obs][i]-centroids[j][i];
		sum_of_squares += tmp*tmp;
	}
	return sum_of_squares;
}

void update_centroids(double** centroids, int* clusters_size, double** clusters_sum) {
	int i;
	int j;
	for(j=0; j<k; j++) {
		for(i=0; i<d; i++) {
			centroids[j][i] = (clusters_sum[j][i] / clusters_size[j]);
		}
	}
}

int compare_centroids(double** prev_centroids, double** centroids) {
	int i;
	int j;

	for(j=0; j<k; j++) {
		for(i=0; i<d; i++) {
			if (prev_centroids[j][i] != centroids[j][i]) {
				return 0;
			}
		}
	}
	return 1;
}

void print_centroids(double** centroids) {
	int i;
	int j;
	for (j=0; j<k; j++)
		{
			for (i=0; i<d; i++)
			{
				if (i < d-1)
					printf("%.2f,", centroids[j][i]);
				else
					printf("%.2f", centroids[j][i]);
			}
			printf("\n");
		}
}
