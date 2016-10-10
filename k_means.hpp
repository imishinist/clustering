#include <vector>
#include <random>
#include <cmath>
#include <chrono>

#define double_inf (std::numeric_limits<double>::infinity())

template<typename T>
double distance(const std::vector<T>& point, const std::vector<double>& mean) {
	double sum = 0;
	for (int i = 0; i < point.size(); i++) {
		sum += pow(point[i] - mean[i], 2);
	}
	return sum;
}

template<typename T>
void calc_means(const std::vector<int>& belongs_to, std::vector<std::vector<double>>& means, const std::vector<std::vector<T>>& points) {
	int k = means.size();
	std::vector<int> point_count(k, 0);

	for (int i = 0; i < k; i++) {
		for(auto &x : means[i]) x = 0.0;
	}
	for (int i = 0; i < points.size(); i++) {
		point_count[belongs_to[i]]++;
		for (int j = 0; j < means[belongs_to[i]].size(); j++) {
			means[belongs_to[i]][j] += points[i][j];
		}
	}

	// TODO: point_countが0の場合について
	for (int i = 0; i < k; i++) {
		for (auto &x : means[i]) x /= point_count[i];
	}
}

template<typename T>
bool assign_means(std::vector<int>& belongs_to, const std::vector<std::vector<double>>& means, const std::vector<std::vector<T>>& points) {
	int n = belongs_to.size(), cnt = 0;

	for (int i = 0; i < n; i++) {
		double dist = double_inf;
		double disti = 0;
		for (int j = 0; j < means.size(); j++) {
			double tmp = distance(points[i], means[j]);
			if (tmp < dist) {
				dist = tmp;
				disti = j;
			}
		}
		if (belongs_to[i] != disti) cnt++;
		belongs_to[i] = disti;
	}
	return cnt != 0;
}

template<typename T>
std::vector<int> k_means(const std::vector<std::vector<T>>& points, int k) {
	std::vector<int> belongs_to(points.size(), 0);
	std::vector<std::vector<double>> means(k, std::vector<double>(points[0].size(), 0));
	
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> r(0, k - 1);
	for (auto& x : belongs_to) x = r(mt);

	do {
		calc_means(belongs_to, means, points);
	} while(assign_means(belongs_to, means, points));

	return belongs_to;
}
