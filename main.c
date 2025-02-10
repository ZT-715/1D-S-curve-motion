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

    float velocity[6];
    float S1, S2, S3;

    time[0] = acceleration_max / jerk_max;
    velocity[0] = jerk_max*time[0]*time[0] / 2;

    printf("Start: \ntime[0] = %f \nvelocity[0] = %f\n", time[0], velocity[0]);

    // evaluate second segment necessity - constant acceleration
    if (velocity[0] >= velocity_max/2) {
        time[0] = sqrtf(velocity_max / jerk_max);
        time[1] = 0.0f;

        printf("seg. II.: \n time[0] = %f \ntime[1] = %f \n\n", time[0], time[1]);
    }
    else {
         time[1] = (velocity_max - 2*velocity[0]) / acceleration_max; // ac_max == ac[1]
    }

    time[2] = time[0];

    velocity[0] = jerk_max*time[0]*time[0] / 2;
    velocity[1] = acceleration_max*time[1] + velocity[0];
    velocity[2] = acceleration_max*time[2] - jerk_max*time[2]*time[2] / 2 + velocity[1];

    printf("velocity[0] = %f \n", velocity[0]);
    printf("velocity[1] = %f \n", velocity[1]);
    printf("velocity[2] = %f \n", velocity[2]); // V2 > limit when 1 0.3 0.5 0.6


    S1  = (jerk_max / 6) * time[0] * time[0] * time[0];
    S2 = (acceleration_max / 2) * time[1] * time[1] + velocity[0] * time[0];
    S3 = (acceleration_max*time[0]*time[0])/2 - (jerk_max*time[0]*time[0]*time[0])/6 + velocity[1]*time[0];

    // printf("\n(S1+S3)+S2 = %f+%f = %f \n", 2*velocity[0]*time[0], time[1]*(acceleration_max*time[1]/2 + velocity[0]), 2*velocity[0]*time[0] + time[1]*(acceleration_max*time[1]/2 + velocity[0]));
    printf("S1+S2+S3 = %f+%f+%f = %f \n",S1,S2,S3, S1 + S2 + S3);


    if (velocity[0] >= velocity_max/2) {
        fprintf(stderr, "velocity[2] %f greater than limit %f \n", velocity[2], velocity_max);
    }

    // If the 3 first segments are more than half the displacement
    // but the first and third are not, recalculate second segment
    if (S1 + S2 + S3 >= displacement/2) {
    //if (S1 + S2 + S3 >= displacement/2) {
        printf("S1 + S2 + S3 >= displacement/2.\n");

        // Evaluates if S1 + S3 is greater than half the displacement
        // ignores 2nd segment and recalculate 1st and 3rd segments.
        if (S1 + S3 >= displacement/2) {
        //if (S1 + S3 >= displacement/2) {
            printf("S1+S2 = %f > displacement/2 \n", 2*velocity[0]*time[0]);
            time[0] = cbrtf( (displacement/2) / jerk_max);
            time[1] = 0.0f;
        }
        else { // OK!
            time[1] = ( - (3*jerk_max*time[0]*time[0])
                        + sqrtf(jerk_max*jerk_max*time[0]*time[0]*time[0]*time[0] + 4*jerk_max*time[0]*displacement)
                      ) / (2*jerk_max*time[0]);
        }

        time[3] = 0.0f;
    }
    else {
        time[3] = (displacement - 2*(S1 + S2 + S3))/velocity[2];
//        time[3] = (displacement -
//             2*(2*velocity[0]*time[0] + time[1]*(acceleration_max*time[1]/2 + velocity[0]))
//         ) / velocity[2];
        // printf("Displacement of segment 4: %f \n", displacement - 2*(2*velocity[0]*time[0] + time[1]*(acceleration_max*time[1]/2 + velocity[0])));
        printf("S4 = %f \n", displacement - 2*(S1 + S2 + S3));
        printf("Const. vel. I: %f \n", velocity[2]);

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

    for (int i = 0; i < 7; i++) {
        time_total += time[i];
    }

    int step_total = time_total / time_delta;

    float* const position_curve = malloc(sizeof(float)*step_total);
    float* const velocity_curve = malloc(sizeof(float)*step_total);
    float* const acceleration_curve = malloc(sizeof(float)*step_total);
    float* const jerk_curve = malloc(sizeof(float)*step_total);

    int const steps1 = (int) (time[0]/time_delta) - 1;
    int const steps2 = (int) (time[1]/time_delta) + steps1;
    int const steps3 = (int) (time[2]/time_delta) + steps2;
    int const steps4 = (int) (time[3]/time_delta) + steps3;
    int const steps5 = (int) (time[4]/time_delta) + steps4;
    int const steps6 = (int) (time[5]/time_delta) + steps5;
    int const steps7 = step_total;

    float const velocity1 = jerk_max*time[0]*time[0] / 2;
    float const velocity2 = acceleration_max*time[1] + velocity1;
    float const velocity3 = acceleration_max*time[2] - jerk_max*time[2]*time[2] / 2 + velocity2;
    float const velocity4 = velocity3;
    float const velocity5 = - jerk_max*time[4]*time[4] / 2 + velocity4;
    float const velocity6 = - acceleration_max*time[5] + velocity5;
    float const velocity7 = 0.0f;

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
                    position_curve[step_counter] = (acceleration_max / 2) * time_j2 +
                                                   velocity1 * time_j +
                                                   position_curve[steps1];
                    velocity_curve[step_counter] = acceleration_max * time_j + velocity1;
                    acceleration_curve[step_counter] = acceleration_max;
                    jerk_curve[step_counter] = 0;
                    break;
                case 2:
                    position_curve[step_counter] = (acceleration_max*time_j2)/2 - (jerk_max*time_j3)/6 + velocity2*time_j + position_curve[steps2];
                    velocity_curve[step_counter] = time_j * (acceleration_max - (jerk_max / 2) * time_j) +
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
                   position_curve[step_counter] = (-acceleration_max / 2) * time_j2 +
                                                  velocity5 * time_j +
                                                  position_curve[steps5];
                   velocity_curve[step_counter] = velocity5 - acceleration_max * time_j;
                   acceleration_curve[step_counter] = - acceleration_max;
                   jerk_curve[step_counter] = 0;
                    break;
                case 6:
                   position_curve[step_counter] = (jerk_max / 6) * time_j3 -
                                                  (acceleration_max / 2) * time_j2 +
                                                  velocity6 * time_j +
                                                  position_curve[steps6];
                   velocity_curve[step_counter] = velocity6 -
                                                  time_j * (acceleration_max - (jerk_max / 2) * time_j);
                   acceleration_curve[step_counter] = acceleration_curve[steps6] + jerk_max*time_j;
                   jerk_curve[step_counter] = jerk_max;
                    break;
                default:
                    break;
            }
            step_counter++;
        }
    }
    if (steps7 != step_counter - 1) {
        fprintf(stderr, "Error in generate_curve_1d: step total not equal steps counted. (%d != %d) \n", steps7, step_counter - 1);
    }

    FILE* file = fopen("output.csv", "w");
    fprintf(file, "time(s);position(m);velocity(m/s);acceleration(m*s^-2);jerk(m*s^-3)\n");

    for (int i = 0; i < step_total; i++) {
        fprintf(file, "%f;%f;%f;%f;%f\n", i*time_delta, position_curve[i], velocity_curve[i], acceleration_curve[i], jerk_curve[i]);
    }

    free(position_curve);
    free(velocity_curve);
    free(acceleration_curve);
    free(jerk_curve);
    fclose(file);

    printf("File output.csv generated successfully.");
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

    printf("Displacement is %f m   \n", displacement);
    printf("Velocity is     %f m/s \n", velocity_max);
    printf("Acceleration is %f m/s2\n", acceleration_max);
    printf("Jerk is         %f m/s3\n\n", jerk_max);

    calculate_time_1d(time, displacement, velocity_max, acceleration_max, jerk_max);

    for (int i = 0; i < 7; i++) {
        printf("Time %2d: %f\n", i, time[i]);
    }

    float total_time = 0.0f;
    for (int i = 0; i < 7; i++) {
        total_time += time[i];
    }

    printf("\nTotal time is %f\n", total_time);

    generate_curve_1d(time, displacement, velocity_max, acceleration_max, jerk_max);

    return 0;
}
