#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std;

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

int main() {
    // Данные из таблицы (время падения в миллисекундах)
    vector<vector<double>> fall_times_ms = {
    {153.436, 153.052, 152.456, 154.468, 154.086, 152.791},
    {153.831, 153.113, 152.778, 154.850, 153.325, 153.689},
    {153.034, 152.855, 152.692, 153.694, 153.551, 153.828},
    {153.313, 152.929, 152.425, 153.770, 153.518, 153.889},
    {153.314, 152.977, 152.718, 153.743, 154.381, 153.851},
    {154.201, 153.264, 152.348, 153.880, 153.964, 153.526},
    {153.285, 153.116, 152.593, 154.230, 153.808, 153.119},
    {154.201, 153.480, 152.404, 154.082, 153.531, 153.554},
    {153.795, 153.357, 152.735, 154.450, 153.125, 153.051},
    {153.388, 153.694, 152.904, 153.615, 153.274, 153.908},
    {153.203, 153.024, 152.724, 154.191, 153.884, 153.391},
    {152.888, 153.135, 152.977, 153.674, 153.727, 153.312},
    {153.638, 152.988, 153.429, 153.764, 153.273, 153.498},
    {153.557, 152.905, 152.847, 154.471, 153.364, 153.383},
    {153.863, 153.828, 152.730, 153.563, 153.707, 153.045},
    {153.628, 153.209, 152.780, 153.951, 153.732, 152.810},
    {155.240, 153.222, 152.630, 153.650, 153.556, 153.059},
    {153.053, 153.518, 152.745, 154.353, 153.594, 153.082},
    {153.266, 152.999, 152.605, 153.636, 153.285, 153.261},
    {153.707, 153.563, 152.814, 154.212, 154.599, 153.221},
    {153.660, 152.876, 152.669, 153.456, 153.416, 153.325},
    {152.930, 153.277, 152.686, 153.995, 153.628, 153.967},
    {153.187, 152.961, 153.692, 153.998, 153.422, 152.880},
    {153.507, 153.085, 152.607, 153.792, 154.061, 153.113},
    {153.452, 153.293, 152.837, 153.651, 153.276, 153.111},
    {153.212, 153.273, 153.137, 153.856, 153.151, 152.801},
    {153.276, 153.012, 152.723, 153.746, 153.369, 153.204},
    {153.427, 153.429, 153.280, 153.727, 153.217, 153.798},
    {154.273, 153.225, 152.688, 155.352, 153.939, 152.798},
    {153.493, 153.527, 152.691, 153.895, 153.939, 153.384}
    };

    vector<string> substance_names = { "Алюминий", "Латунь", "Сталь", "Дерево", "Плексиглас", "Свинец" };
    vector<double> g_values(substance_names.size());
    vector<double> g_errors(substance_names.size()); // Add this line
    vector<double> t_values(substance_names.size());

    double h = 0.272; // м

    double h_error = 0.001; // м
    double v0 = 1.050; // м/с (Changed v to v0)
    double v0_error = 0.005; // м/с  (Changed v_error to v0_error)

    // Вычисляем g и погрешность для каждого вещества
    for (size_t i = 0; i < substance_names.size(); ++i) {
        // Вычисляем среднее время падения
        double sum = 0.0;
        for (size_t j = 0; j < fall_times_ms.size(); ++j) {
            sum += fall_times_ms[j][i];
        }
        double t_ms = sum / fall_times_ms.size(); // Среднее время в мс
        double t = t_ms / 1000.0;               // Среднее время в секундах
        t_values[i] = t_ms;

        // Вычисляем ускорение свободного падения
        g_values[i] = (2 * (h - v0 * t)) / (t * t); // Use v0 instead of v

        // Вычисляем погрешность g по формуле
        double dt = 0.000001; // 1 мкс в секундах, оценка погрешности времени
        double term1 = (1.0 / 9.0) * pow(1.0 / (t * t), 2) * pow(h_error, 2);
        double term2 = (1.0 / 9.0) * pow(1.0 / t, 2) * pow(v0_error, 2); // Use v0_error instead of v_error
        double term3 = pow(((v0 * t - 2 * h) / (t * t * t)), 2) * pow(dt, 2); // Use v0 instead of v

        g_errors[i] = 2 * sqrt(term1 + term2 + term3); // Save error in g_errors
    }

    // Выводим результаты в формате таблицы
    cout << "-----------------------------------------------------------------" << endl;
    cout << "| № | Вещество   |   g, м/с²   |  Погрешность Δg, м/с² |" << endl;
    cout << "-----------------------------------------------------------------" << endl;

    for (size_t i = 0; i < substance_names.size(); ++i) {
        cout << "| " << setw(1) << i + 1 << " | "
            << setw(10) << substance_names[i] << " | "
            << setw(11) << fixed << setprecision(4) << g_values[i] << " | "
            << setw(21) << fixed << setprecision(4) << g_errors[i] << " |" << endl;
    }

    cout << "-----------------------------------------------------------------" << endl;

    //Средние значения времени пролета шариков
    cout << "Средние значения времени пролета шариков" << endl;
    for (int i = 0; i < t_values.size(); ++i)
    {
        cout << "| " << setw(10) << t_values[i] << " | ";
    }
    cout << endl;
    for (size_t i = 0; i < substance_names.size(); ++i) {
        cout << "| " << setw(10) << substance_names[i] << " | ";
    }
    cout << endl;

    // Вычисляем погрешность t
    cout << "Погрешности t" << endl;
    for (int j = 0; j < fall_times_ms[0].size(); ++j)
    {
        vector<double> times(fall_times_ms.size());
        for (int i = 0; i < fall_times_ms.size(); ++i)
        {
            times[i] = fall_times_ms[i][j];
        }
        cout << "| " << setw(10) << standard_deviation(times) << " | ";
    }
    cout << endl;
    for (size_t i = 0; i < substance_names.size(); ++i) {
        cout << "| " << setw(10) << substance_names[i] << " | ";
    }
    cout << endl;

    return 0;
}
