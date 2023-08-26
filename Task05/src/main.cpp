#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

struct City {
    std::string name;
    double x, y;
};

double calculate_distance(const City& city1, const City& city2, int norm) {
    double distance = 0.0;
    if (norm == 0) {             // euclidian distance
        distance = std::sqrt((city1.x - city2.x) * (city1.x - city2.x) +
            (city1.y - city2.y) * (city1.y - city2.y));
    }
    else if (norm == 1) {       //chebyshevDistance
        distance = std::abs(city1.x - city2.x) + std::abs(city1.y - city2.y);
    }
    else if (norm == 2)   {    // manhattanDistance
        distance = std::max(std::abs(city1.x - city2.x), std::abs(city1.y - city2.y));
    }
    return distance;
}

int main() {
    std::ifstream input("txt.data");
    if (!input.is_open()) {
        std::cerr << "Error opening txt.data file." << std::endl;
        return 1;
    }

    std::vector<City> cities;
    std::string line;
    while (std::getline(input, line)) {
        City city;
        city.name = line;
        std::getline(input, line);
        std::istringstream coordinates(line);
        coordinates >> city.x >> city.y;
        cities.push_back(city);
    }

    while (true) {
        std::string city_name;
        double radius;
        int norm;

        std::cout << "Enter the city name (enter 0 to exit): ";
        std::getline(std::cin, city_name);
        if (city_name == "0") {
            break;
        }

        std::cout << "Enter the search radius: ";
        std::cin >> radius;

        std::cout << "Enter the norm: 0 for Euclidean, 1 for Manhattan, 2 for Chebyshev: ";
        std::cin >> norm;

        const auto& selected_city = std::find_if(cities.begin(), cities.end(),
            [&city_name](const City& c) { return c.name == city_name; });

        if (selected_city == cities.end()) {
            std::cout << "City not found." << std::endl;
            continue;
        }

        std::vector<std::pair<std::string, double>> nearby_cities;

        for (const City& city : cities) {
            if (city.name != city_name) {
                double distance = calculate_distance(*selected_city, city, norm);
                if (distance <= radius) {
                    nearby_cities.emplace_back(city.name, distance);
                }
            }
        }

        std::sort(nearby_cities.begin(), nearby_cities.end(),
            [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
                return a.second < b.second;
            });

        std::cout << "Total cities found in given radius: " << cities.size()
            << "Cities are to the north of " << city_name << ": "
            << std::count_if(cities.begin(), cities.end(),
                [&selected_city](const City& c) { return c.y < selected_city->y; })
            << std::endl;

        std::cout << "Nearby cities:" << std::endl;
        for (const auto& city : nearby_cities) {
            std::cout << city.first << ": " << city.second << std::endl;
        }
    }

    return 0;
}
