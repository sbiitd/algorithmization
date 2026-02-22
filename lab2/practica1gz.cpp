#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

bool isInside(double x, double y) {
    bool below_upper = (y <= 2 * x - 4 + 0.001) && (y <= -2 * x + 12 + 0.001);
    bool above_lower = (y >= 2 * x - 12 - 0.001) && (y >= -2 * x + 4 - 0.001);

    return below_upper && above_lower;
}

double monteCarloArea(double x_min, double x_max, double y_min, double y_max, long long N) {
    long long M = 0;

    for (long long i = 0; i < N; i++) {
        double x = x_min + (double)rand() / RAND_MAX * (x_max - x_min);
        double y = y_min + (double)rand() / RAND_MAX * (y_max - y_min);

        if (isInside(x, y)) {
            M++;
        }
    }

    double rectArea = (x_max - x_min) * (y_max - y_min);
    return ((double)M / N) * rectArea;
}

int main() {
    setlocale(LC_ALL, "RU");
    srand(time(NULL));

    double x_min = 0, x_max = 8;
    double y_min = -4, y_max = 8;
    double true_area = 16.0;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Точное значение площади: " << true_area << "\n";
    std::cout << "Эксперимент по расчету площади методом Монте-Карло:\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << "| " << std::setw(10) << "N (точек)"
        << " | " << std::setw(12) << "Площадь"
        << " | " << std::setw(12) << "Погрешность" << " |\n";
    std::cout << "--------------------------------------------------------\n";

    long long N_vals[] = { 100, 1000, 10000, 100000, 1000000, 10000000 };

    for (long long N : N_vals) {
        double area = monteCarloArea(x_min, x_max, y_min, y_max, N);
        double error = fabs(area - true_area);
        double error_percent = (error / true_area) * 100;

        std::cout << "| " << std::setw(10) << N
            << " | " << std::setw(12) << area
            << " | " << std::setw(11) << error_percent << "% |\n";
    }
    std::cout << "--------------------------------------------------------\n";

    return 0;
}