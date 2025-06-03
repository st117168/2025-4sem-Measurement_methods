#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct Point {
    double x;
    double y;
};

// Функция для вычисления среднего значения
double mean(const std::vector<double>& data) {
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

int main() {

    ifstream file1("data1.txt");
    ifstream file2("data2.txt");
    ofstream outfile("results_table.csv");

    int N = 16;

    vector<Point> data1(N), data2(N);

    if (!file1 || !file2 || !outfile) {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }

    for (int i = 0; i < N; ++i) file1 >> data1[i].x >> data1[i].y;
    for (int i = 0; i < N; ++i) file2 >> data2[i].x >> data2[i].y;

    // Вычисляем Iк для грубых и точных измерений и переводим в мА
    for (int i = 0; i < 16; ++i)
        {
            data1[i].y = log((data1[i].y / 12.0));   // Iк = Uк / 12 и переводим в мА (грубые данные)
            data2[i].y = log((data2[i].y / 12.0)); // Iк = Uк / 12 и переводим в мА (точные данные)
        }

    // Перезаписываем файлы с новыми значениями y
    std::ofstream outfile1("data3.txt");
    std::ofstream outfile2("data4.txt");

    if (!outfile1 || !outfile2) {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
        return 1;
    }

    // Записываем новые данные (x остаётся прежним, y преобразуется)
    for (int i = 0; i < N; ++i) {
        outfile1 << data1[i].x << " " << data1[i].y << "\n";
    }
    for (int i = 0; i < N; ++i) {
        outfile1 << exp(data1[i].y) * 1000 << "\n";
    }

    for (int i = 0; i < N; ++i) {
        outfile2 << data2[i].x << " " << data2[i].y << "\n";
    }
    for (int i = 0; i < N; ++i) {
        outfile2 << exp(data2[i].y) * 1000 << "\n";
    }

    outfile1.close();
    outfile2.close();

    N = 16;
    const double T = 297.0; // Температура в К
    const double t_value = 2.3646; // t-критическое значение для 95% доверия при df=7

    ifstream file3("data3.txt");
    ifstream file4("data4.txt");

    if (!file3 || !file4 || !outfile) {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }

    for (int i = 0; i < N; ++i) file3 >> data1[i].x >> data1[i].y;
    for (int i = 0; i < N; ++i) file4 >> data2[i].x >> data2[i].y;

    vector<double> a1(N);
    vector<double> a2(N);
    double sum_a1 = 0.0;
    double sum_a2 = 0.0;

    cout << fixed << setprecision(6);
    outfile << fixed << setprecision(6);

    cout << "Таблица 2: Метод парных точек\n";
    cout << "№;x2;x1;x2-x1;y2;y1;y2-y1;a_i = dy/dx\n";
    outfile << "№;x2;x1;x2-x1;y2;y1;y2-y1;a_i = dy/dx\n";

    for (int i = 0; i < N / 2; ++i) {
        double dx1 = data1[i+8].x - data1[i].x;
        double dy1 = data1[i+8].y - data1[i].y;
        a1[i] = dy1 / dx1;
        sum_a1 += a1[i];

        cout << i + 1 << ";" << data1[i+8].x << ";" << data1[i ].x << ";" << dx1 << ";"
            << data1[i+8].y << ";" << data1[i].y << ";" << dy1 << ";" << a1[i] << endl;

        outfile << i + 1 << ";" << data1[i+8].x << ";" << data1[i ].x << ";" << dx1 << ";"
            << data1[i+8].y << ";" << data1[i].y << ";" << dy1 << ";" << a1[i] << endl;

    }
    cout << endl;
    outfile << endl;

    for (int i = 0; i < N / 2; ++i) {
        double dx2 = data2[i + 8].x - data2[i].x;
        double dy2 = data2[i + 8].y - data2[i].y;
        a2[i] = dy2 / dx2;
        sum_a2 += a2[i];
        cout << i + 1 << ";" << data2[i + 8].x << ";" << data2[i].x << ";" << dx2 << ";"
            << data2[i + 8].y << ";" << data2[i].y << ";" << dy2 << ";" << a2[i] << endl;

        outfile << i + 1 << ";" << data2[i + 8].x << ";" << data2[i].x << ";" << dx2 << ";"
            << data2[i + 8].y << ";" << data2[i].y << ";" << dy2 << ";" << a2[i] << endl;
    }

    double a_avg1 = sum_a1 / (N/2);
    double a_avg2 = sum_a2 / (N/2);

    double sum_diff1 = 0.0;
    double sum_diff_sq1 = 0.0;
    double sum_diff2 = 0.0;
    double sum_diff_sq2 = 0.0;

    cout << "\nОтклонения и квадраты отклонений:\n";
    cout << "№;a_i;a_i - ā;(a_i - ā)^2\n";
    outfile << "\nОтклонения и квадраты отклонений\n";
    outfile << "№;a_i;a_i - ā;(a_i - ā)^2\n";

    for (int i = 0; i < N/2; ++i) {
        double diff1 = a1[i] - a_avg1;
        double diff_sq1 = diff1 * diff1;
        sum_diff1 += diff1;
        sum_diff_sq1 += diff_sq1;

        cout << i + 1 << ";" << a1[i] << ";" << diff1 << ";" << diff_sq1 << endl;
        outfile << i + 1 << ";" << a1[i] << ";" << diff1 << ";" << diff_sq1 << "\n";
    }
    cout << endl;
    outfile << endl;
    for (int i = 0; i < N / 2; ++i) {
        double diff2 = a2[i] - a_avg2;
        double diff_sq2 = diff2 * diff2;
        sum_diff2 += diff2;
        sum_diff_sq2 += diff_sq2;
        cout << i + 1 << ";" << a2[i] << ";" << diff2 << ";" << diff_sq2 << endl;
        outfile << i + 1 << ";" << a2[i] << ";" << diff2 << ";" << diff_sq2 << "\n";

    }
    cout << endl;
    outfile << endl;

    double variance1 = sum_diff_sq1 / (N/2 - 1);
    double variance2 = sum_diff_sq2 / (N/2 - 1);
    double std_dev1 = sqrt(variance1);
    double std_dev2 = sqrt(variance2);
    double std_err1 = std_dev1 / sqrt(N/2);
    double std_err2 = std_dev2 / sqrt(N/2);
    double delta_a1 = t_value * std_err1;
    double delta_a2 = t_value * std_err2;

    double a_min1 = a_avg1 - delta_a1;
    double a_max1 = a_avg1 + delta_a1;
    double a_max2 = a_avg2 + delta_a2;
    double a_min2 = a_avg2 - delta_a2;

    double ek1 = T * a_avg1;
    double delta_ek1 = T * delta_a1;
    double ek2 = T * a_avg2;
    double delta_ek2 = T * delta_a2;

    vector<double> lni0(N);
    for (int i = 0; i < N; ++i)
    {
        lni0[i] = data1[i].y;
    }

    double i0 = exp(mean(lni0)-a_avg1*0.375);

    cout << "\n--- Результаты грубых---\n";
    cout << "Средний наклон (ā): " << a_avg1 << endl;
    cout << "Сумма (a_i - ā)^2: " << sum_diff_sq1 << endl;
    cout << "Стандартное отклонение: " << std_dev1 << endl;
    cout << "Стандартная ошибка: " << std_err1 << endl;
    cout << "95% доверительный интервал для tg(α): [" << a_min1 << ", " << a_max1 << "]" << endl;
    cout << "\ne/k = T * ā = " << T << " * " << a_avg1 << " = " << ek1 << endl;
    cout << "Погрешность e/k: ±" << delta_ek1 << endl;
    cout << "I0: " << i0*1e08 << endl;

    for (int i = 0; i < N; ++i)
    {
        lni0[i] = data2[i].y;
    }

    i0 = exp(mean(lni0) - a_avg2 * 0.375);

    cout << "\n--- Результаты точных---\n";
    cout << "Средний наклон (ā): " << a_avg2 << endl;
    cout << "Сумма (a_i - ā)^2: " << sum_diff_sq2 << endl;
    cout << "Стандартное отклонение: " << std_dev2 << endl;
    cout << "Стандартная ошибка: " << std_err2 << endl;
    cout << "95% доверительный интервал для tg(α): [" << a_min2 << ", " << a_max2 << "]" << endl;
    cout << "\ne/k = T * ā = " << T << " * " << a_avg2 << " = " << ek2 << endl;
    cout << "Погрешность e/k: ±" << delta_ek2 << endl;
    cout << "I0: " << i0 * 1e08 << endl;

    outfile << "\nИтоги грубых;\n";
    outfile << "Средний наклон (ā);" << a_avg1 << "\n";
    outfile << "Сумма (a_i - ā)^2;" << sum_diff_sq1 << "\n";
    outfile << "Стандартное отклонение;" << std_dev1 << "\n";
    outfile << "Стандартная ошибка;" << std_err1 << "\n";
    outfile << "95% доверительный интервал;" << a_min1 << ";" << a_max1 << "\n";
    outfile << "e/k;" << ek1 << "\n";
    outfile << "Погрешность e/k;" << delta_ek1 << "\n";

    outfile << "\nИтоги точных;\n";
    outfile << "Средний наклон (ā);" << a_avg2 << "\n";
    outfile << "Сумма (a_i - ā)^2;" << sum_diff_sq2 << "\n";
    outfile << "Стандартное отклонение;" << std_dev2 << "\n";
    outfile << "Стандартная ошибка;" << std_err2 << "\n";
    outfile << "95% доверительный интервал;" << a_min2 << ";" << a_max2 << "\n";
    outfile << "e/k;" << ek2 << "\n";
    outfile << "Погрешность e/k;" << delta_ek2 << "\n";

    outfile.close();
    cout << "\nВсе данные сохранены в файл: results_table.csv" << endl;

    return 0;
}
