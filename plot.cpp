#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

int column_count(string file) {
    string line;
    fstream fs(file);
    getline(fs, line);

    stringstream ss(line);
    int cnt = 0;
    string tmp;
    while (ss >> tmp) {
        cnt++;
    }
    return cnt;
}

void plot(string file) {
    int columns = 2;
    string data_file, plot_file;
    data_file = file + ".dat";
    plot_file = file + ".plt";

    columns = column_count(data_file) / 2;

    ofstream fs(plot_file);
    fs << "set xrange [0:100]" << endl;
    fs << "set yrange [0:100]" << endl;

    fs << "plot \"" << data_file << "\" using 1:2" << endl;

    for (int i = 1; i < columns; i++) {
        fs << "replot \"" << data_file << "\" using " << i * 2 + 1 << ":"
           << i * 2 + 2 << endl;
    }

    string command = "gnuplot " + plot_file;
    system(command.c_str());
}

int main(int argc, char *argv[]) {
    string file;
    for (int i = 1; i < argc; i++) {
        string file = argv[i];
        auto pos = file.find_last_of(".");
        plot(file.substr(0, pos));
    }

    return 0;
}
