/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
  

#include <stdio.h>
#include "robot.h"
#include "mrpiz.h"

#define LEFT_MOTOR MD
#define RIGHT_MOTOR MA
#define LIGHT_SENSOR S1
#define FRONT_BUMPER S3
#define FLOOR_SENSOR S2


static CmdState command;

void Robot_start(){
    if(mrpiz_init()){
        printf("Echec initialisation\n\n");
    }
    command.left_cmd = 0;
    command.right_cmd = 0;
}

void Robot_stop(){
    mrpiz_close();
}

int Robot_getRobotSpeed(){
    int averageSpeed = (command.left_cmd+command.right_cmd)/2;
    return averageSpeed;
}

SensorState Robot_getSensorState(){
    SensorState sensor;
    int sensorFrontLeft = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT);
    int sensorFrontCenter = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER);
    int sensorFrontRight = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT);

    if (sensorFrontLeft && sensorFrontCenter && sensorFrontRight){
        sensor.collision = NO_BUMP;
    }
    else{
        sensor.collision = BUMPED;
    }
    return sensor;
}   

void Robot_setWheelsVelocity(int mr,int ml){
    command.left_cmd = ml;
    command.right_cmd = mr;

    if(mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, mr)){
        printf("Echec set motor droit\n");
    }
    if(mrpiz_motor_set(MRPIZ_MOTOR_LEFT, ml)){
        printf("Echec set motor gauche\n\n");
    }
}