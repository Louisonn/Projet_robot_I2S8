/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer

#include <stdio.h>
#include "pilot.h"
#include "robot.h"



typedef int bool; //TYPE BOOLEAN

typedef void(*actionFunction)(void);

typedef enum
{
    S_FORGET=0, // pas de changement d'état, mais consommation de l'event
    S_IDLE, // état OFF
    S_RUNNING, // état ON
    S_DEATH, // MAE détruite
    NB_STATE
} state_e;

typedef enum
{
    A_NOP = 0,      // on ne fait rien
    A_SEND_MVT,
    A_STOP,     // Stop mouvement
    A_KILL,      // Kill la MAE
    NB_ACTION
} transitionAction_e;

typedef enum
{
    E_BUMP=0,   // Colision
    E_SET_VELOCITY,
    E_STOP,     // stop called by user
    E_KILL,      // Kill MAE
    NB_EVENT
} event_e;

typedef struct
{
    state_e nextState;
    transitionAction_e action;
} transition_t;


//**************************Private fonctions prototypes**************************

static void run(event_e e);

static bool hasBumped();

static void sendMvt();    // Action A_SEND_MVT

static void stop();                         // Action A_STOP

static void kill();                         // Action A_KILL

static void nullFunction();


//**************************Private variables**************************

static const transition_t transitionTab[NB_STATE][NB_EVENT] = {
    [S_IDLE][E_KILL] = {S_DEATH, A_KILL},
    [S_IDLE][E_SET_VELOCITY] = {S_RUNNING, A_SEND_MVT},
    [S_RUNNING][E_BUMP] = {S_IDLE, A_STOP},
    [S_RUNNING][E_SET_VELOCITY] = {S_RUNNING, A_SEND_MVT},
    [S_RUNNING][E_STOP] = {S_IDLE, A_STOP},
    [S_RUNNING][E_KILL] = {S_DEATH, A_KILL}
};

static const actionFunction transitionFct[NB_ACTION] = {&nullFunction, &sendMvt, &stop, &kill};

state_e myState;

static VelocityVector vectorToSend;

//**************************Private fonction definition**************************

/*
    Contains STATE MACHINE
*/
static void run(event_e event)
{    

    transition_t transition = transitionTab[myState][event];

   
    if(transition.nextState != S_FORGET)
    {
        transitionFct[transition.action]();
        myState = transition.nextState;
    }
}

static bool hasBumped(){
    if(Robot_getSensorState().collision == 0){
    return 0; // has not bumped
    }
    return 1; // has bumped
}

static void sendMvt()
{
    switch(vectorToSend.dir){
        case RIGHT:
            Robot_setWheelsVelocity(-vectorToSend.power,vectorToSend.power);
        break;
        case LEFT:
            Robot_setWheelsVelocity(vectorToSend.power,-vectorToSend.power);
        break;
        case FORWARD:
            Robot_setWheelsVelocity(vectorToSend.power,vectorToSend.power);
        break;
        case BACKWARD:
            Robot_setWheelsVelocity(-vectorToSend.power,-vectorToSend.power);
        break;
        default:
            Robot_setWheelsVelocity(0,0);
        break;
    }
}

static void stop()
{
    //printf("still working\n");
    vectorToSend.dir = STOP;
    vectorToSend.power = 0;
    sendMvt();
}

static void kill()
{
    stop();
    Robot_stop();
}

static void nullFunction()
{
}

//**************************Public fonction definition**************************

void Pilot_start(){
    vectorToSend.dir = STOP;
    vectorToSend.power = 0; 
    myState = S_IDLE;
    Robot_start();
}

void Pilot_stop(){
    run(E_KILL);
}

void Pilot_new(){
    
}

void Pilot_free(){
    
}

/*
    Set the velocityVector for the robot
    Called by admin
*/
void Pilot_setVelocity(VelocityVector vel){
    vectorToSend = vel;
    if(vel.dir == STOP){
        run(E_STOP);
    }
    else{
        printf("bhhgjkbj\n");
        run(E_SET_VELOCITY);
    }
}


/*
    Check if there is a collision
    Called by admin
*/
void Pilot_check(){
    bool bump = hasBumped();
    if(bump){
        run(E_BUMP);
    }
}

/*
    Return the state of the robot
*/
PilotState Pilot_getState(){
    PilotState state;
    state.sensor = Robot_getSensorState();
    state.speed = Robot_getRobotSpeed();
    return state;
}