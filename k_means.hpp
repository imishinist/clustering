#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include "stream.cpp"

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
void initialize_kpp(std::vector<int>& belongs_to, std::vector<std::vector<double>>& means, const std::vector<std::vector<T>>& points, int k) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> r(0, belongs_to.size() - 1);
    int f_i = r(mt);
    for (int i = 0; i < points[f_i].size(); i++) {
        means[0][i] = points[f_i][i];
    }

    int selected = 1;

    while(selected < k) {
        std::vector<double> d(points.size(), 0);
        double D = 0.0;

        for (int i = 0; i < points.size(); i++) {
            double dist = double_inf;
            double disti = 0;
            for (int j = 0; j < selected; j++) {
                double tmp = distance(points[i], means[j]);
                if (tmp < dist) {
                    dist = tmp;
                    disti = j;
                }
            }
            D += dist;
            d[i] = dist;
            belongs_to[i] = disti;
        }

        double sum = 0.0;
        std::vector<double> tables;
        for (int i = 0; i < d.size(); i++) {
            sum += d[i] / D;
            tables.push_back(sum);
        }
        std::uniform_real_distribution<> a(0, 1);
        auto pos = std::upper_bound(tables.begin(), tables.end(), a(mt));
        int index = std::distance(tables.begin(), pos);
        means[selected] = points[index];
        selected++;
    }
}

template<typename T>
std::vector<int> k_means(const std::vector<std::vector<T>>& points, int k) {
	std::vector<int> belongs_to(points.size(), 0);
	std::vector<std::vector<double>> means(k, std::vector<double>(points[0].size(), 0));
	
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> r(0, k - 1);
	//for (auto& x : belongs_to) x = r(mt);
    initialize_kpp(belongs_to, means, points, k);

    while (assign_means(belongs_to, means, points)) {
		calc_means(belongs_to, means, points);
    }

	return belongs_to;
}
