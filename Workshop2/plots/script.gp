set title "Зависимость чувствительности пластин вертикального отклонения от напряжения"
set xlabel "Напряжение, В"
set ylabel "Чувствительность, мм/В"
set grid
set xrange [6:31]
set xtics 6,1,31
set yrange [0.5:0.6] 
plot "v_data.txt" using 1:2 with linespoints title "Экспериментальные данные"
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "sensitivity_vs_voltage.png"
replot

set title "Зависимость чувствительности пластин горизонтального отклонения от напряжения"
set xlabel "Напряжение, В"
set ylabel "Чувствительность, мм/В"
set grid
set xrange [4:27.5]
set xtics 4,1,28
set yrange [0.6:0.9]  # Устанавливаем диапазон Y для лучшей визуализации
plot "h_data.txt" using 1:2 with linespoints title "Экспериментальные данные"
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "horizontal_sensitivity_vs_voltage.png"
replot

f(x) = a*x + b  # Линейная модель
fit f(x) "h_data.txt" using 1:2 via a, b
plot "h_data.txt" using 1:2 with points title "Данные", f(x) with lines title sprintf("Аппроксимация: %.3f x + %.3f", a, b)

