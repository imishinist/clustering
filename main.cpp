#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include "spectral.hpp"
using namespace std;

namespace clustering {
    double none = -1.0;
}

typedef std::function<std::vector<int>(const std::vector<point>&, int, double)> cut;

template<typename T>
ostream& operator<<(ostream& o, const pair<T, T>& p) {
	o << showpoint << p.first << " " << p.second << " ";
	return o;
}

template<typename T>
ostream& operator<<(ostream& o, const vector<T>& p) {
	for (int i = 0; i < p.size(); i++) {
		o << showpoint << p[i] << " ";
	}
	return o;
}

template<typename T>
ostream& operator<<(ostream& o, const vector<vector<pair<T,T>>>& k_points) {
    int mx = 0;
    pair<T, T> none_p = make_pair(clustering::none, clustering::none);
    for (auto& points : k_points) {
        if (mx < points.size()) {
            mx = points.size();
        }
    }
    for (int i = 0; i < mx; i++) {
        for (auto& points : k_points) {
            if (i < points.size()) {
                o << points[i];
            } else {
                o << none_p;
            }
        }
        o << endl;
    }
    return o;
}

string labels[] = {"ncut", "mcut", "kmeans"};
int n;

template<typename T>
void print(string file, int k, vector<int>& tgv, const vector<pair<T, T>>& a) {
	ofstream of(file + "_" + labels[n - 1] + ".dat");

    vector<vector<pair<T, T>>> k_points(k);
    for (int i = 0; i < a.size(); i++) {
        k_points[tgv[i]].push_back(a[i]);
    }

    of << k_points;
}

int main(int argc, char *argv[]) {
	int k;
	double sigma2 = 1;
	string file;

	cout << "input data file" << endl;
	cin >> file;
	cout << "ncut: 1, mcut: 2, kmeans: 3" << endl;
	cin >> n;
	if (n != 3) {
		cout << "sigma2" << endl;
		cin >> sigma2;
	}
	cout << "cluster" << endl;
	cin >> k;

	string line;
	vector<pair<double, double>> a;

	ifstream fs(file + ".dat");
	while (getline(fs, line)) {
		stringstream ss(line);
		double x, y;
		ss >> x >> y;
		a.push_back(make_pair(x, y));
	}

	cut f[] = {ncut, mcut, just_k_means};

	auto ret = f[n - 1](a, k, sigma2 * 2);
	print(file, k, ret, a);
	string command = "./bin/plot " + file + "_" + labels[n - 1];
	system(command.c_str());
	return 0;
}
