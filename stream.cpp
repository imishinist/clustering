#include <iostream>
#include <vector>

namespace clustering {
    double none = -1.0;
}

template<typename T>
std::ostream& operator<<(std::ostream& o, const std::pair<T, T>& p) {
	o << std::showpoint << p.first << " " << p.second << " ";
	return o;
}

template<typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& p) {
	for (int i = 0; i < p.size(); i++) {
		o << std::showpoint << p[i] << " ";
	}
	return o;
}

template<typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<std::pair<T,T>>>& k_points) {
    int mx = 0;
    std::pair<T, T> none_p = std::make_pair(clustering::none, clustering::none);
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
        o << std::endl;
    }
    return o;
}
