#include <ros/ros.h>
#include <wiringPi.h>
#include <stdio.h>
#include <chrono>

#define EN_A 23 //Green
#define EN_B 24 //Yellow

#define PULSE_NUM 11 //encoder pulse number

#define REDUCTION_RATIO 90

const int count_turn_en = 4 * PULSE_NUM;
const int count_turn_out = count_turn_en * REDUCTION_RATIO;

int count = 0;

float angle_out_pre = 0;
float angle_out = 0;



void encoder_count_A(){
    int i,j;
    i = digitalRead(EN_A);
    j = digitalRead(EN_B);
    if (i == j){
        count --;
    }else{
        count ++;
    }
}

void encoder_count_B(){
    int i,j;
    i = digitalRead(EN_A);
    j = digitalRead(EN_B);
    if (i == j){
        count ++;
    }else{
        count --;
    }
}

float calc_angle_encoder(int _count){
    int count_temp = 360.0 / (count_turn_en) * (_count % (count_turn_en));
    if (count_temp >= 0)
        return count_temp;
    else
        return count_temp + 360.0;
}

float calc_angle_output(int _count){
    int count_temp = 360.0 / (count_turn_out) * (_count % (count_turn_out));
    if (count_temp >= 0)
        return count_temp;
    else
        return count_temp + 360.0;
}

int main(void) {
    int i;

    if(wiringPiSetupGpio() == -1) return 1;

    pinMode(EN_A, INPUT);
    pinMode(EN_B, INPUT);
    wiringPiISR(EN_A, INT_EDGE_BOTH, encoder_count_A);
    wiringPiISR(EN_B, INT_EDGE_BOTH, encoder_count_B);
    
    while (1)
    {
        printf("count %i, angle_en %f, angle_out %f \n", count, calc_angle_encoder(count), calc_angle_output(count));
        delay(5);
    }
    return 0;
}