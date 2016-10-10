#include <iostream>
#include <random>
#include <fstream>
#include <functional>
using namespace std;

typedef pair<double, double> point;
typedef function<void(string)> func;

ostream& operator<<(ostream& o, const point& p) {
	o << showpoint << p.first << " " << showpoint << p.second;
	return o;
}

vector<point> generate(int n, const point& p, double sigma) {
	random_device rnd;
	mt19937 mt(rnd());
	normal_distribution<> x(p.first, sigma), y(p.second, sigma);

	vector<point> ret(n);

	for (int i = 0; i < n; i++) {
		ret[i] = make_pair(x(mt), y(mt));
	}
	return ret;
}

void show(string file) {
	string command = "./bin/plot " + file;
	system(command.c_str());
}

void normal(string file) {
	int n;
	cout << "count: n" << endl;
	cin >> n;

	double sigma;
	cout << "sigma" << endl;
	cin >> sigma;

	int x, y;
	cout << "x, y" << endl;
	cin >> x >> y;

	ofstream fs(file + ".dat", ios::app);
	auto a = generate(n, make_pair(x, y), sigma);
	for (int i = 0; i < a.size(); i++) {
		fs << a[i] << endl;
	}
}

void siki(string file) {
	double a;
	double x0, y0;
	cout << "x0, y0" << endl;
	cin >> x0 >> y0;
	cout << "a" << endl;
	cin >> a;
	auto f = [=](double x) {
		return a * (x - x0) * (x - x0) + y0;
	};

	pair<double, double> xrange, yrange;
	cout << "xrange" << endl;
	cin >> xrange.first >> xrange.second;

	ofstream fs(file + ".dat", ios::app);
	for (int i = (int) (xrange.first * 10); i < (int)(xrange.second * 10); i++) {
		double x = i / 10.0;
		double val = f(x);
		fs << make_pair(x, val) << endl;
	}
}

int main(int argc, char *argv[]) {
	string file;
	cout << "file" << endl;
	cin >> file;
	cout << "0: stop, 1: 表示, 2: 正規分布, 3: 二次関数" << endl;
	int num;
	func fs[] = {show, normal, siki};
	while(cin >> num, num) {
		fs[num - 1](file);
		cout << "0: stop, 1: 表示, 2: 正規分布, 3: 二次関数" << endl;
	}
	return 0;
}
