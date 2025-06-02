# Настройки графика
set title "Гистограмма распределения интервалов частот"
set xlabel "Интервалы частот, кГц"  # Указаны единицы измерения
set ylabel "Число случаев (Δn)"
set style data histogram
set style fill solid border -1
set boxwidth 0.5 absolute
set grid y
set yrange [0:20]

# Явное задание позиций и меток границ интервалов
set xtics (\
  "4.490 кГц" 4.490, \
  "4.500 кГц" 4.500, \
  "4.510 кГц" 4.510, \
  "4.520 кГц" 4.520, \
  "4.530 кГц" 4.530, \
  "4.540 кГц" 4.540, \
  "4.550 кГц" 4.550 \
) rotate by -45

# Границы отображения по оси X
set xrange [4.480:4.550]

# Данные (левая граница интервала, Δn, ширина интервала)
$data << EOD
4.490 5 0.01
4.500 6 0.01
4.510 9 0.01
4.520 19 0.01
4.530 16 0.01
4.540 5 0.01
EOD

# Построение гистограммы с явным указанием границ
plot $data using 1:2:3 with boxes lc rgb "#3498db" title "Число случаев"

# Сохранение в PNG
set terminal pngcairo enhanced font "Arial,12" size 800,600
set output "histogram_ms.png"
replot
set terminal pop