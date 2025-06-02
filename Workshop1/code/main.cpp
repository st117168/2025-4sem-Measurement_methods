#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <clocale>

// Функции для вычислений
double avarage(const std::vector<double>& data)
{
    double sum = 0.0;
    for (double u : data)
    {
        sum += u;
    }
    return sum / data.size();
}

std::vector<double> standartDeviation(std::vector<double>& randDevAr)
{
    std::vector<double> standAr;
    for (int i = 0; i < randDevAr.size(); i++)
    {
        double t = randDevAr[i];
        standAr.push_back(t * t);
    }
    return standAr;
}

std::vector<double> randomDeviation(const std::vector<double>& data, double avarage)
{
    std::vector<double> randDevAr;
    for (int i = 0; i < data.size(); i++)
    {
        randDevAr.push_back(data[i] - avarage);
    }

    return randDevAr;
}

std::vector<double> calculateDeltaT(const std::vector<double>& Tx_values, double gamma0, double T0, double T_avg)
{
    std::vector<double> deltaT_results;
    for (double Tx : Tx_values)
    {
        double gamma_T = gamma0 + (T0 / Tx);
        deltaT_results.push_back(gamma_T * T_avg);
    }
    return deltaT_results;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    const double gamma0 = 5e-7;
    const double T0 = 0.1;

    std::ifstream file("rude.txt");
    std::string line;
    std::vector<double> values;

    if (!file.is_open())
    {
        std::cerr << "ошибка открытия файла" << std::endl;
        return 1;
    }

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        values.push_back(stod(line));
    }

    file.close();

    // Вычисление среднего значения
    double mean = avarage(values);
    std::cout << "Среднее значение (грубые): " << mean << std::endl;

    // Вычисление отклонений от среднего для каждого значения
    std::cout << "Отклонения от среднего: " << std::endl;

    std::vector<double> ar1 = randomDeviation(values, mean);

    for (int i = 0; i < ar1.size(); i++)
    {
        std::cout << ar1[i] << "\n";
    }
    std::cout << std::endl;

    // Вычисление стандартного отклонения

    std::cout << "Стандартное отклонение: " << std::endl;

    std::vector<double> ar2 = standartDeviation(ar1);
    for (int i = 0; i < ar2.size(); i++)
    {
        std::cout << ar2[i] << "\n";
    }
    std::cout << std::endl;

    // Вычисление средней погрешности прибора
    std::cout << "Средняя погрешность прибора: " << std::endl;

    std::cout << (gamma0 + (T0 / mean)) * mean << "\n";
    std::cout << std::endl;

    // Вычисление погрешностей
    std::cout << "Погрешности (грубые): " << std::endl;

    std::vector<double> delta_T1 = calculateDeltaT(values, gamma0, T0, mean);
    for (int i = 0; i < delta_T1.size(); i++)
    {
        std::cout << delta_T1[i] << "\n";
    }
    std::cout << std::endl;

    std::ifstream coarse_file("accurate.txt");
    std::string linee;
    std::vector<double> coarse_values;

    if (!coarse_file.is_open())
    {
        std::cerr << "ошибка открытия файла" << std::endl;
        return 1;
    }

    while (getline(coarse_file, linee))
    {
        if (linee.empty())
        {
            continue;
        }

        coarse_values.push_back(stod(linee));
    }
    coarse_file.close();

    double coarse_avg = avarage(coarse_values);
    std::cout << "Среднее значение (точные): " << coarse_avg << std::endl;

    double gamma_T;
    std::vector<double> delta_T = calculateDeltaT(coarse_values, gamma0, T0, coarse_avg);

    std::cout << "Погрешности (точные): " << std::endl;

    for (int i = 0; i < delta_T.size(); i++)
    {
        std::cout << delta_T[i] << "\n";
    }
    std::cout << std::endl;

    // Вычисление отклонений от среднего для каждого значения
    std::cout << "Отклонения от среднего: " << std::endl;

    std::vector<double> rd1 = randomDeviation(coarse_values, coarse_avg);

    for (int i = 0; i < rd1.size(); i++)
    {
        std::cout << rd1[i] << "\n";
    }
    std::cout << std::endl;

    std::cout << "Квадрат отклонения от среднего: " << std::endl;

    for (int i = 0; i < rd1.size(); i++)
    {
        std::cout << rd1[i] * rd1[i] << "\n";
    }
    std::cout << std::endl;

    std::cout << "Среднеквадратичное отклонение: " << std::endl;

    double sum = 0;
    for (int i = 0; i < rd1.size(); i++)
    {
        sum += rd1[i] * rd1[i];
    }
    if (rd1.size() != 1)
    {
        std::cout << sqrt(sum / (rd1.size() - 1)) << std::endl;
    }
    else
    {
        std::cout << "n = 0!" << std::endl;
    }

    return EXIT_SUCCESS;
}