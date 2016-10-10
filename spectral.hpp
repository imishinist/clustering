#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "Eigen/Dense"
#include "k_means.hpp"

typedef std::pair<double, double> point;
typedef std::function<double(const point&, const point&)> similarity_function;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix;
typedef std::pair<matrix, matrix> decomposition;

matrix diagonal(const matrix& S) {
	int n = S.rows();
	matrix D = matrix::Zero(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			D(i, i) += S(i, j);
		}
	}
	return D;
}

double similar_euclidean(const point& p, const point& q) {
	return pow(p.first - q.first, 2) + pow(p.second - q.second, 2);
}

similarity_function build_euclidean_metric(double sigma2) {
	return [=](const point& p, const point& q) {
		return exp(-similar_euclidean(p, q)/sigma2);
	};
}

matrix similarity_matrix(const std::vector<point>& points, similarity_function f) {
	int n = points.size();
	matrix s(n, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			s(i, j) = f(points[i], points[j]);
			if (i == j) {
				s(i, j) = 0;
			}
		}
	}
	return s;
}

std::vector<int> spectral(const decomposition& D, int k) {
	int n = D.first.rows();
	std::vector<std::vector<double>> eigenvectors(n, std::vector<double>(k));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			eigenvectors[i][j] = (D.second)(i, j);
		}
	}

	return k_means(eigenvectors, k);
}

std::vector<int> ncut(const std::vector<point>& points, int k, double sigma2) {
	matrix W = similarity_matrix(points, build_euclidean_metric(sigma2));
	matrix D = diagonal(W);
	Eigen::GeneralizedSelfAdjointEigenSolver<matrix> solve(D-W, D);

	return spectral(decomposition(solve.eigenvalues(), solve.eigenvectors()), k);
}

std::vector<int> mcut(const std::vector<point>& points, int k, double sigma2) {
	int n = points.size();
	matrix W = similarity_matrix(points, build_euclidean_metric(sigma2));
	matrix D = diagonal(W);
	for (int i = 0; i < n; i++) {
		D(i, i) = 1 / sqrt(D(i, i));
	}
	matrix L = matrix::Identity(n, n) - D * W * D;
	Eigen::SelfAdjointEigenSolver<matrix> solve(L);

	return spectral(decomposition(solve.eigenvalues(), solve.eigenvectors()), k);
}

std::vector<int> just_k_means(const std::vector<point>& points, int k, double sigma2) {
	int n = points.size();
	std::vector<std::vector<double>> values(n, std::vector<double>(2));
	for (int i = 0; i < n; i++) {
		values[i][0] = points[i].first;
		values[i][1] = points[i].second;
	}

	return k_means(values, k);
}
