/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
   
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "remoteUI.h"
#include "client.h"

#define LEFT_KEY     'q'
#define RIGHT_KEY    'd'
#define FORWARD_KEY  'z'
#define BACKWARD_KEY 's'
#define STOP_KEY     ' '
#define CLEARLOG_KEY 'e'
#define ASK4LOG_KEY  'r'
#define QUIT_KEY     'a'

#define POWER 50


typedef int bool;




//Private functions

static void captureChoice();

static VelocityVector translate(Direction dir);

static void ask4log();

static void askClearLog();

static void askMvt(Direction dir);

static void eraseLog(); 

static void quit();

static void Remote_run();

static void display();

static void displayState();



bool running = 1;

received_data_t state;




// Function Definition

static void captureChoice(){

   struct termios oldt, newt;
   tcgetattr(STDIN_FILENO, &oldt);
   newt = oldt;
   newt.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
   char choice = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

   switch(choice)
      {
         case LEFT_KEY :
            askMvt(LEFT);
         break;
         case RIGHT_KEY :
            askMvt(RIGHT);
         break;
         case FORWARD_KEY :
            askMvt(FORWARD);
         break;
         case BACKWARD_KEY :
            askMvt(BACKWARD);
         break;
         case STOP_KEY :
            askMvt(STOP);
         break;
         case CLEARLOG_KEY :
            askClearLog();
         break;
         case ASK4LOG_KEY :
            ask4log();
         break;
         case QUIT_KEY :
            quit();
         break;
         default :
         break;
      }
}


static VelocityVector translate(Direction dir){ 
   VelocityVector vector = {dir, POWER};
   return vector;
}

static void askMvt(Direction dir){
   ordre_e ordre = ORDRE_SET_VELOCITY;
   VelocityVector vector;
   vector = translate(dir);
   transmitted_data_t data = {ordre, vector};
   client_sendMsg(&data);
   printf("checkpoint\n");
}

static void ask4log(){
   ordre_e ordre = ORDRE_CHECK;
   VelocityVector vector = {STOP,0};
   transmitted_data_t data = {ordre, vector};
   client_sendMsg(&data);

   client_readMsg(&state);
   displayState();
}

static void askClearLog(){
   eraseLog();
}

static void quit(){
   running = 0;

   ordre_e ordre = ORDRE_STOP;
   VelocityVector vector = {STOP,0};
   transmitted_data_t data = {ordre, vector};
   client_sendMsg(&data);
   

   client_stop();
}

static void eraseLog(){
   printf("\033[2J");
}

static void Remote_run(){
   while(running){
      display();
      captureChoice();
   }
}

static void display(){
   printf("ACTIONS DISPONIBLES :\n");
   printf("\tq : aller à droite\n");
   printf("\td : aller à droite\n");
   printf("\tz : avancer\n");
   printf("\ts : reculer\n");
   printf("\t  : stopper\n");
   printf("\te : effacer les log\n");
   printf("\tr : afficher l'état du robot\n");
   printf("\ta : quitter\n");
   printf("\n\n***********************************************\n\n");
}

static void displayState(){

   printf("ETAT DU ROBOT : \n");
   printf("\tSpeed : %d\n",state.speed);
   if(state.isBumped == 1){
      printf("Le robot est entrée en collision\n");
   }
   printf("\n\n***********************************************\n\n");

}

/**
 * initialize in memory RemoteUI
 * 
 */
void RemoteUI_new(){
}

void RemoteUI_start(){
   client_start();
   Remote_run();
}


/**
 * Stop RemoteUI
 *
 */
void RemoteUI_stop(){
}

/**
 * destruct the RemoteUI from memory 
 *
 */
void RemoteUI_free(){

}

   