# Velocity Profile calculator

Takes 4 parameters, displacement expected and velocity, acceleration and jerk limits, then calculates time of each of the 7 constant jerk segments to arrive at a continuous acceleration curve for movement, also known as **S-curve**.

The time results of ```calculate_time_1d()``` can generate a **csv** with all motion profile curves using ```generate_curve_1d()```, and visualized with the Python script ```Plot.py```.

#### Requirements:
1. GCC
2. Python3 with Matplotlib and Pandas installed

## Flow chart:
![Flow Chart](./flowchart.png)

## Compile:
```
>> gcc main.c -o main.o  -std=c99 -Wall -pedantic
```

## Run:
```main.o``` takes 4 arguments, displacent, maximum velocity, maximum acceleration and maximum jerk:
```
>> ./main.o  0.5 0.3 0.3 0.6 && python plot.py
Displacement is 0.500000 m
Velocity is     0.300000 m/s
Acceleration is 0.300000 m/s2
Jerk is         0.600000 m/s3

Start:
time[0] = 0.500000
velocity[0] = 0.075000
velocity[0] = 0.075000
velocity[1] = 0.225000
velocity[2] = 0.300000
S1+S2+S3 = 0.012500+0.075000+0.137500 = 0.225000
S4 = 0.050000
Const. vel. I: 0.300000
Time  0: 0.500000
Time  1: 0.500000
Time  2: 0.500000
Time  3: 0.166667
Time  4: 0.500000
Time  5: 0.500000
Time  6: 0.500000

Total time is 3.166667
```

![Output Example](./result_V2/motion_plot.png)

## Tests

To cover 100% of the code, 4 different inputs are required based on the flow chart.
The tests may be executed with ```./test.sh``` or manually.
### 01 — Maximum Velocity (V₁ ≤ V<sub>max</sub>/2)
```
>> ./main.o  1 0.3 0.3 0.6 && python plot.py
```
![Test01](./tests/test01.png)
### 02 — Limited Velocity (V₁ > V<sub>max</sub>/2)
```
>> ./main.o  0.4 0.3 0.3 0.6 && python plot.py
```
![Test02](./tests/test02.png)
### 03 — Limited Velocity (S1 + S2 + S3 > S/2)
```
>> ./main.o  0.4 0.1 0.3 0.6 && python plot.py
```
![Test03](./tests/test03.png)
### 04 — Limited Acceleration (S1 + S3 > S/2)
```
>> ./main.o  0.01 0.166 0.5 0.1 && python plot.py
```
![Test04](./tests/test04.png)
## REFERENCE

Ng Yu Ki (2008). "A New Velocity Profile Generation for High-Efficiency CNC Machining Applications."