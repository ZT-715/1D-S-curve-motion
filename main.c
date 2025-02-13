//
// Created by Gabriel Zanella on 04/02/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // C99 or greater
#include <stdlib.h>

float* calculate_time_1d(float time[6],
                         const float displacement,
                         const float velocity_max,
                         const float acceleration_max,
                         const float jerk_max) {

    float velocity1, velocity2, velocity3, velocity4 = 0.0f;
    float S1, S2, S3;

    time[0] = acceleration_max / jerk_max;
    velocity1 = jerk_max*time[0]*time[0] / 2;

    printf("Start calculate_time_1d(): \ntime[0] = %f \nvelocity1 = %f\n", time[0], velocity1);

    // evaluate second segment necessity - constant acceleration
    if (velocity1 >= velocity_max/2) {
        time[0] = sqrtf(velocity_max / jerk_max);
        time[1] = 0.0f;

        printf("V1 >= Vmax/2: \n time[0] = %f \ntime[1] = %f \n\n", time[0], time[1]);
    }
    else {
         time[1] = (velocity_max - 2*velocity1) / acceleration_max; // ac_max == ac[1]
    }

    time[2] = time[0];
    float acceleration_peak = time[0]*jerk_max;

    velocity1 = jerk_max*time[0]*time[0] / 2;
    velocity2 = acceleration_peak*time[1] + velocity1;
    velocity3 = acceleration_peak*time[2] - jerk_max*time[2]*time[2] / 2 + velocity2;

    printf("\nvelocity1 = %f \n", velocity1);
    printf("velocity2 = %f \n", velocity2);
    printf("velocity3 = %f \n", velocity3);


    S1  = (jerk_max / 6) * time[0] * time[0] * time[0];
    S2 = (acceleration_peak / 2) * time[1] * time[1] + velocity1 * time[1];
    S3 = (acceleration_peak*time[0]*time[0])/2 - (jerk_max*time[0]*time[0]*time[0])/6 + velocity2*time[0];

    printf("\nS1+S2+S3 = %f+%f+%f = %f \n",S1,S2,S3, S1 + S2 + S3);

    // If the 3 first segments are more than half the displacement
    // but the first and third are not, recalculate second segment
    if (S1 + S2 + S3 >= displacement/2) {
    //if (S1 + S2 + S3 >= displacement/2) {
        printf("S1 + S2 + S3 >= displacement/2.\n");

        // Evaluates if S1 + S3 is greater than half the displacement
        // ignores 2nd segment and recalculate 1st and 3rd segments.
        if (S1 + S3 >= displacement/2) {
            printf("S1+S3 = %f > displacement/2 \n", S1 + S3);
            time[0] = cbrtf((displacement/2) / jerk_max);
            time[1] = 0.0f;
            acceleration_peak = time[0]*jerk_max;

            if (acceleration_peak > acceleration_max) {
                fprintf(stderr, "Acceleration peak %f m*s^-2 greater than limit %f m*s^-2 \n", acceleration_peak, acceleration_max);
            }
        }
        else {
            time[1] = ( - (3*jerk_max*time[0]*time[0])
                        + sqrtf(jerk_max*jerk_max*time[0]*time[0]*time[0]*time[0] + 4*jerk_max*time[0]*displacement)
                      ) / (2*jerk_max*time[0]);
        }

        time[3] = 0.0f;
    }
    else {
        time[3] = (displacement - 2*(S1 + S2 + S3))/velocity3;

        printf("S4 = %f \n", displacement - 2*(S1 + S2 + S3));
        printf("velocity4 = %f \n", velocity4);

    }

    time[2] = time[0];
    time[4] = time[0];
    time[5] = time[1];
    time[6] = time[2];

    return time;
}

void generate_curve_1d(float time[6],
                       const float displacement,
                       const float velocity_max,
                       const float acceleration_max,
                       const float jerk_max) {

    float time_delta = 0.0001; // 1 us

    float time_total = 0.0f;

    for (int i = 0; i < 6; i++) {
        time_total += time[i];
    }

    int step_total = time_total / time_delta;

    float* const position_curve = malloc(sizeof(float)*step_total);
    float* const velocity_curve = malloc(sizeof(float)*step_total);
    float* const acceleration_curve = malloc(sizeof(float)*step_total);
    float* const jerk_curve = malloc(sizeof(float)*step_total);

    if (position_curve == NULL || velocity_curve == NULL || acceleration_curve == NULL || jerk_curve == NULL) {
        fprintf(stderr, "Error in allocating memory for curve data.\n");
    }

    int const steps1 = (int) (time[0]/time_delta) - 1;
    int const steps2 = (int) (time[1]/time_delta) + steps1;
    int const steps3 = (int) (time[2]/time_delta) + steps2;
    int const steps4 = (int) (time[3]/time_delta) + steps3;
    int const steps5 = (int) (time[4]/time_delta) + steps4;
    int const steps6 = (int) (time[5]/time_delta) + steps5;
    // int const steps7 = step_total;

    float const acceleration_peak = time[0]*jerk_max;

    float const velocity1 = jerk_max*time[0]*time[0] / 2;
    float const velocity2 = acceleration_peak*time[1] + velocity1;
    float const velocity3 = acceleration_peak*time[2] - jerk_max*time[2]*time[2] / 2 + velocity2;
    float const velocity4 = velocity3;
    float const velocity5 = - jerk_max*time[4]*time[4] / 2 + velocity4;
    float const velocity6 = - acceleration_peak*time[5] + velocity5;
    // float const velocity7 = 0.0f;

    int step_counter = 0;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < time[i]/time_delta; j++) {
            float time_j = j*time_delta;
            float time_j2 = time_j * time_j;
            float time_j3 = time_j2 * time_j;

            switch (i){
                case 0:
                    position_curve[step_counter] = (jerk_max / 6) * time_j3;
                    velocity_curve[step_counter] = (jerk_max / 2) * time_j2;
                    acceleration_curve[step_counter] = jerk_max*time_j;
                    jerk_curve[step_counter] = jerk_max;

                    break;
                case 1:
                    position_curve[step_counter] = (acceleration_peak / 2) * time_j2 +
                                                   velocity1 * time_j +
                                                   position_curve[steps1];
                    velocity_curve[step_counter] = acceleration_peak * time_j + velocity1;
                    acceleration_curve[step_counter] = acceleration_peak;
                    jerk_curve[step_counter] = 0;
                    break;
                case 2:
                    position_curve[step_counter] = (acceleration_peak*time_j2)/2 - (jerk_max*time_j3)/6 + velocity2*time_j + position_curve[steps2];
                    velocity_curve[step_counter] = time_j * (acceleration_peak - (jerk_max / 2) * time_j) +
                                                   velocity2;
                    acceleration_curve[step_counter] = acceleration_curve[steps2] - jerk_max*time_j;
                    jerk_curve[step_counter] = - jerk_max;
                    break;
                case 3:
                    position_curve[step_counter] = velocity3 * time_j + position_curve[steps3];
                    velocity_curve[step_counter] = velocity3;
                    acceleration_curve[step_counter] = 0;
                    jerk_curve[step_counter] = 0;
                    break;
                case 4:
                   position_curve[step_counter] = (-jerk_max / 6) * time_j3 +
                                                  velocity4 * time_j +
                                                  position_curve[steps4];
                   velocity_curve[step_counter] = velocity4 - (jerk_max / 2) * time_j2;
                   acceleration_curve[step_counter] = - jerk_max*time_j;
                   jerk_curve[step_counter] = -jerk_max;
                    break;
                case 5:
                   position_curve[step_counter] = (-acceleration_peak / 2) * time_j2 +
                                                  velocity5 * time_j +
                                                  position_curve[steps5];
                   velocity_curve[step_counter] = velocity5 - acceleration_peak * time_j;
                   acceleration_curve[step_counter] = - acceleration_peak;
                   jerk_curve[step_counter] = 0;
                    break;
                case 6:
                   position_curve[step_counter] = (jerk_max / 6) * time_j3 -
                                                  (acceleration_peak / 2) * time_j2 +
                                                  velocity6 * time_j +
                                                  position_curve[steps6];
                   velocity_curve[step_counter] = velocity6 -
                                                  time_j * (acceleration_peak - (jerk_max / 2) * time_j);
                   acceleration_curve[step_counter] = acceleration_curve[steps6] + jerk_max*time_j;
                   jerk_curve[step_counter] = jerk_max;
                    break;
                default:
                    break;
            }
            step_counter++;
        }
    }

    FILE* file = fopen("output.csv", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening output.csv\n");
    }

    fprintf(file, "time(s);position(m);velocity(m/s);acceleration(m*s^-2);jerk(m*s^-3)\n");

    for (int i = 0; i < step_total; i++) {
        fprintf(file, "%f;%f;%f;%f;%f\n", i*time_delta, position_curve[i], velocity_curve[i], acceleration_curve[i], jerk_curve[i]);
    }

    free(position_curve);
    free(velocity_curve);
    free(acceleration_curve);
    free(jerk_curve);
    fclose(file);

    printf("File output.csv generated.\n\n");
}

int main(int argc, char** argv) {
    float time[7];

    float displacement, velocity_max, acceleration_max, jerk_max;
//    float benchmark[4][4] = {{ , , , },
//                             { , , , },
//                             { , , , },
//                             { , , , }};
//    if (argc == 5) {
        displacement = strtof(argv[1], NULL);
        velocity_max = strtof(argv[2], NULL);
        acceleration_max = strtof(argv[3], NULL);
        jerk_max = strtof(argv[4], NULL);
//    }

    printf("\nDisplacement is       %f m   \n", displacement);
    printf("Velocity limit        %f m/s \n", velocity_max);
    printf("Acceleration limit is %f m/s2\n", acceleration_max);
    printf("Jerk limit is         %f m/s3\n\n", jerk_max);

    calculate_time_1d(time, displacement, velocity_max, acceleration_max, jerk_max);

    printf("\n\n");
    for (int i = 0; i < 7; i++) {
        printf("Time %2d: %f s\n", i, time[i]);
    }

    float total_time = 0.0f;
    for (int i = 0; i < 7; i++) {
        total_time += time[i];
    }

    printf("\nTotal time is %f\n", total_time);

    generate_curve_1d(time, displacement, velocity_max, acceleration_max, jerk_max);

    return 0;
}
