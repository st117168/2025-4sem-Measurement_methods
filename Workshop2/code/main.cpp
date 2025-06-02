#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <clocale>

std::vector<double> readData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("ошибка открытия файла " + filename);
    }

    std::vector<double> data;
    double value;
    while (file >> value) {
        data.push_back(value);
    }

    return data;
}

std::vector<double> calculateSensitivity(const std::vector<double>& L, const std::vector<double>& Ueff) {
    std::vector<double> sensitivity;

    for (size_t i = 0; i < L.size(); ++i) {
        double S = L[i] / (2 * std::sqrt(2) * Ueff[i]);
        sensitivity.push_back(S);
    }

    return sensitivity;
}

// Функция для вычисления среднего значения
double mean(const std::vector<double>& data) {
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

// Функция для вычисления стандартного отклонения (деление на n(n-1))
double standard_deviation(const std::vector<double>& data) {
    double avg = mean(data);
    double sum_squared_diff = 0.0;

    // Суммируем квадраты отклонений
    for (double value : data) {
        sum_squared_diff += pow(value - avg, 2);
    }

    // Стандартное отклонение (деление на n(n-1))
    return sqrt(sum_squared_diff / (data.size() * (data.size() - 1)));
}

// Функция для выполнения расчетов стандартного отклонения и вывода результатов
void calculateAndPrintResults(std::vector<double> S_y, std::vector<double> S_x) {

    // Вычисляем среднее и стандартное отклонение для каждой группы
    double mean_group1 = mean(S_y);
    double std_dev_group1 = standard_deviation(S_y);

    double mean_group2 = mean(S_x);
    double std_dev_group2 = standard_deviation(S_x);

    // Выводим результаты
    std::cout << "Первая группа:" << std::endl;
    std::cout << "Среднее значение S_y: " << mean_group1 << std::endl;
    std::cout << "Стандартное отклонение S_y: " << std_dev_group1 << std::endl;

    std::cout << "\nВторая группа:" << std::endl;
    std::cout << "Среднее значение S_x: " << mean_group2 << std::endl;
    std::cout << "Стандартное отклонение S_x: " << std_dev_group2 << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<double> L_vertical_horizontal = { 10, 20, 30, 40, 50 };
    std::vector<double> L_max = { 10, 20, 30, 40, 50 };

    std::vector<double> Ueff_vertical;
    std::vector<double> Ueff_horizontal;
    std::vector<double> Ueff_max;

    try {
        Ueff_vertical = readData("Ueff_vertical.txt");
        Ueff_horizontal = readData("Ueff_horizontal.txt");
        Ueff_max = readData("Ueff_max.txt");

        if (Ueff_vertical.size() != L_vertical_horizontal.size() ||
            Ueff_horizontal.size() != L_vertical_horizontal.size()) {
            throw std::invalid_argument("размерности Ueff_vertical.txt и Ueff_horizontal.txt не совпадают");
        }

        if (Ueff_max.size() != L_max.size()) {
            throw std::invalid_argument("размерности Ueff_max.txt и L_max не совпадают");
        }

        std::vector<double> S_vertical = calculateSensitivity(L_vertical_horizontal, Ueff_vertical);
        std::vector<double> S_horizontal = calculateSensitivity(L_vertical_horizontal, Ueff_horizontal);

        std::vector<double> S_max = calculateSensitivity(L_max, Ueff_max);

        std::cout << "длина линии и чувствительность для пластины вертикального отклонения:" << std::endl;
        for (size_t i = 0; i < S_vertical.size(); ++i) {
            std::cout << "L = " << L_vertical_horizontal[i] << ", S = " << S_vertical[i] << std::endl;
        }

        std::cout << "\nдлина линии и чувствительность для пластины горизонтального отклонения:" << std::endl;
        for (size_t i = 0; i < S_horizontal.size(); ++i) {
            std::cout << "L = " << L_vertical_horizontal[i] << ", S = " << S_horizontal[i] << std::endl;
        }

        std::cout << "\nМаксимальная чувствительность:" << std::endl;
        for (size_t i = 0; i < S_max.size(); ++i) {
            std::cout << "L = " << L_max[i] << ", S = " << S_max[i] << std::endl;
        }

        std::ofstream outputFile("output.txt");
        if (!outputFile.is_open()) {
            throw std::runtime_error("ошибка записи в файл");
        }

        outputFile << "длина линии и чувствительность для пластины вертикального отклонения:" << std::endl;
        for (size_t i = 0; i < S_vertical.size(); ++i) {
            outputFile << "L = " << L_vertical_horizontal[i] << ", S = " << S_vertical[i] << std::endl;
        }

        outputFile << "\nдлина линии и чувствительность для пластины горизонтального отклонения:" << std::endl;
        for (size_t i = 0; i < S_horizontal.size(); ++i) {
            outputFile << "L = " << L_vertical_horizontal[i] << ", S = " << S_horizontal[i] << std::endl;
        }

        outputFile << "\nЧувствительность для данных из Ueff_max.txt:" << std::endl;
        for (size_t i = 0; i < S_max.size(); ++i) {
            outputFile << "L = " << L_max[i] << ", S = " << S_max[i] << std::endl;
        }

        outputFile.close();
        std::cout << "данные сохранены в output.txt" << std::endl;

        S_vertical.erase(S_vertical.begin(), S_vertical.begin() + 1);
        calculateAndPrintResults(S_horizontal, S_vertical);
    }
    catch (const std::exception& e) {
        std::cerr << "ошибка: " << e.what() << std::endl;
    }

    return 0;
}