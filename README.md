# Velocity Profile
Compile:
```
>> gcc main.c -o main.a -std=c99 -Wall -pedantic
```

Run:

```
>> ./main.a 0.5 0.3 0.3 0.6 && python plot.py
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

![plot](./result_V2/motion_plot.png)

# REFERENCE

A NEW VELOCITY PROFILE
GENERATION FOR HIGH
EFFICIENCY CNC MACHINING
APPLICATION, NG YU KI