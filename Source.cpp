#include<iostream>
#include<omp.h>
#include<time.h>
using namespace std;

const int m = 10000, n = 10000;

int main()
{
	int** matrix = new int* [m];
	for (int i = 0; i < m; i++)
		matrix[i] = new int[n];

	int* vector = new int[n];
	int * ans1 = new int[m];
	int* ans2 = new int[m];
	int* ans3 = new int[m];

	omp_set_num_threads(8);
	omp_set_nested(true);

	clock_t start, stop;

	int i, j;

#pragma omp parallel for private(i) shared(ans1, ans2) schedule(static) 
	for (i = 0; i < m; i++)
	{
		ans1[i] = 0;
		ans2[i] = 0;
		ans3[i] = 0;
	}

	for (i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = rand() % 20 - 10;

	for (i = 0; i < n; i++)
		vector[i] = rand() % 20 - 10;

	start = clock();
#pragma omp parallel for private(i, j) shared(vector, ans3) schedule(static) 
	for (i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ans3[i] += matrix[i][j] * vector[j];
	stop = clock() - start;
	cout << "nonsharedmatrixtime = " << double(stop) / CLK_TCK << endl;

	start = clock();
#pragma omp parallel for private(i, j) shared(vector, ans1, matrix) schedule(static) 
	for (i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ans1[i] += matrix[i][j] * vector[j];
	stop = clock() - start;
	cout << "paralleltime = " << double(stop)/CLK_TCK << endl;

	start = clock();
	for (i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ans2[i] += matrix[i][j] * vector[j];
	stop = clock() - start;
	cout << "time = " << double(stop) / CLK_TCK << endl;

	bool f = true;
	for (int i = 0; i < m; i++)
		if (ans1[i] != ans2[i])
			f = false;
	cout << f;

	return 0;
}