# Настройки вывода в PNG
set terminal pngcairo enhanced font "Arial,12" size 1000,800
set output "smooth_distribution_ms.png"

# Настройки графика
set title "Кривая распределения интервалов частот"
set xlabel "Интервалы частот, кГц" font ",14"
set ylabel "Доля случаев (δn)" font ",14"
set grid y
set yrange [0:0.38]
set xrange [4.480:4.550]
set key top right box

# Явное задание границ интервалов на оси X
set xtics (\
  "4.490 кГц" 4.490, \
  "4.500 кГц" 4.500, \
  "4.510 кГц" 4.510, \
  "4.520 кГц" 4.520, \
  "4.530 кГц" 4.530, \
  "4.540 кГц" 4.540, \
  "4.550 кГц" 4.550 \
) rotate by 0 font ",12"

# Данные (центр интервала, δn, левая граница, правая граница)
$data << EOD
# Центр  δn    Нижняя   Верхняя
4.495 0.0833 4.490 4.500
4.505 0.1000 4.500 4.510
4.515 0.1500 4.510 4.520
4.525 0.3167 4.520 4.530
4.535 0.2667 4.530 4.540
4.545 0.0833 4.540 4.550
EOD

# Построение гладкой кривой с аннотациями
plot $data using 1:2 smooth csplines with lines lw 3 lc rgb "#e74c3c" title "Доля случаев", \
     '' using 1:2 with points pt 7 ps 2.0 lc rgb "#e74c3c" title "Экспериментальные точки", \
     '' using 1:2:(sprintf("%.3f", $2)) with labels offset 0,1 font ",10" notitle, \
     '' using 1:2:(sprintf("[%.3f-%.3f кГц]", $3, $4)) with labels offset 0,-1.5 font ",8" tc rgb "#7f8c8d" notitle

# Сохранение и завершение
set output
set terminal pop