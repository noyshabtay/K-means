/*
 ============================================================================
 Name        : kmeans.c
 Author      : Noy Shabtay
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Kmeans clustering algorithm implementation.
 ============================================================================
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

int k;
int n;
int d;
int max_iter;

static PyObject* kmeans_runner(PyObject*, PyObject*);
static void kmeans(double**, double**);
static void copy_centroids(double**, double**);
static double squared_distance(int, int, double**, double**);
static void update_centroids(double**, int*, double**);
static int compare_centroids(double**, double**);
static void print_centroids(double**);


static PyObject* kmeans_runner(PyObject* self, PyObject* args) {
	int i;
	int j;
	int index;
	double **observations;
	double **centroids;
	long *indices;
	PyObject* py_observations;
	PyObject* py_indices;
	PyObject* py_item;
	PyObject* py_dim;

	if (!PyArg_ParseTuple(args, "iiiiOO:kmeans_runner", &k, &n, &d, &max_iter, &py_indices, &py_observations)) {
		return NULL;
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

	for (i = 0; i < n; i++) {
		py_item = PyList_GetItem(py_observations, i);
		for (j = 0; j < d; j++) {
			py_dim = PyList_GetItem(py_item, j);
			observations[i][j] = PyFloat_AsDouble(py_dim);
		}
	}

	indices = (long*)malloc(k * sizeof(long));
	for (i = 0; i < k; i++) {
		py_item = PyList_GetItem(py_indices, i);
		indices[i] = PyLong_AsLong(py_item);
	}

	for (j = 0; j < k; j++) {
		index = indices[j];
		for (i = 0; i < d; i++) {
			centroids[j][i] = observations[index][i];
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

	Py_RETURN_NONE;
}

static void kmeans(double** observations, double** centroids) {
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

static void copy_centroids(double** centroids ,double** prev_centroids) {
	int i;
	int j;
	for(j=0; j<k; j++) {
		for(i=0; i<d; i++) {
			prev_centroids[j][i] = centroids[j][i];
		}
	}
}

static double squared_distance(int j, int obs, double** observations, double** centroids) {
	int i;
	double sum_of_squares = 0;
	double tmp;
	for(i=0; i<d; i++) {
		tmp = observations[obs][i]-centroids[j][i];
		sum_of_squares += tmp*tmp;
	}
	return sum_of_squares;
}

static void update_centroids(double** centroids, int* clusters_size, double** clusters_sum) {
	int i;
	int j;
	for(j=0; j<k; j++) {
		for(i=0; i<d; i++) {
			centroids[j][i] = (clusters_sum[j][i] / clusters_size[j]);
		}
	}
}

static int compare_centroids(double** prev_centroids, double** centroids) {
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

static void print_centroids(double** centroids) {
	int i;
	int j;
	for (j=0; j<k; j++)
		{
			for (i=0; i<d; i++)
			{
				if (i < d-1)
					printf("%lf,", centroids[j][i]);
				else
					printf("%lf", centroids[j][i]);
			}
			printf("\n");
		}
}


static PyMethodDef kmeansMethods[] = {
	{"kmeans_runner",
	  (PyCFunction)kmeans_runner,
	  METH_VARARGS,
	  PyDoc_STR("A C Kmeans implementation")},
	{NULL, NULL, 0, NULL}
};


static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	"mykmeanssp", 
	NULL, 
	-1,  
	kmeansMethods 
};


PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
	return PyModule_Create(&moduledef);
}
