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
void print(int k, vector<int> tgv, const vector<pair<T, T>>& a) {
	ofstream of("tmp/kmeans.dat");
	vector<vector<int>> idx(k);
	for (int i = 0; i < tgv.size(); i++) {
		idx[tgv[i]].push_back(i);
	}
	int mx = 0;
	for (int i = 0; i < idx.size(); i++) {
		if (mx < idx[i].size()) {
			mx = idx[i].size();
		}
	}
	for (int i = 0; i < mx; i++) {
		for (int j = 0; j < k; j++) {
			if (i < idx[j].size()) {
				int index = idx[j][i];
				of << a[index];
			} else {
				of << make_pair<double, double>(0.0, 0.0);
			}
		}
		of << endl;
	}
}

int main(int argc, char *argv[]) {
	int n, k;
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
	print(k, ret, a);
	string command = "./bin/plot tmp/kmeans";
	system(command.c_str());
	return 0;
}
