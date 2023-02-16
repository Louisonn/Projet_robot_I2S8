/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
   
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "server.h"
#include "pilot.h"

#define MAX_PENDING_CONNECTIONS 1
#define PORT_DU_SERVEUR (12355)
#define POWER 50


typedef enum{
    ORDRE_STOP = 0,
    ORDRE_SET_VELOCITY,
    ORDRE_CHECK
}ordre_e;

typedef struct{
    ordre_e ordre;
    VelocityVector vector;
}received_data_t;

typedef struct{
    int speed;
    int isBumped;
}transmitted_data_t;

//DATA transmitted is a PilotState struct

//*******************Private variables *******************


static int socket_donnees;
int running;


//*******************Private function declaration *******************

static void sendMsg();

static void readMsg(void);

//*******************Private function definition *******************

static void sendMsg(){
   PilotState state;
   int transmitted_quantity;
   
   state = Pilot_getState();
   transmitted_quantity = write(socket_donnees, &state, sizeof(state));
   if(transmitted_quantity == -1)
   {
      printf("Error : lost connection with Telco.\nCouldn't send data.");
   }

}

static void readMsg()
{
   received_data_t received_data;
   int received_quantity = read(socket_donnees, (struct sockaddr *)&received_data, sizeof(received_data));

         printf("\n%d\n\n", received_quantity);
   if(received_quantity > 0)
   {
      switch (received_data.ordre)
      {      
      case ORDRE_STOP:
         printf("\nSTOP\n\n");
         server_stop();
         break;

      case ORDRE_CHECK:
         printf("\nCHECK\n\n");
         Pilot_check();
         sendMsg();
         break;
      
      case ORDRE_SET_VELOCITY:
         printf("\nSET VEL\n\n");
         Pilot_setVelocity(received_data.vector);
         break;

      default:
         break;
      }
   }
}

//*******************Public function definition *******************
void server_start(){
   int socket_ecoute;
   struct sockaddr_in mon_adresse;

   socket_ecoute = socket (PF_INET, SOCK_STREAM, 0);
   mon_adresse.sin_family = AF_INET;
   mon_adresse.sin_port = htons(PORT_DU_SERVEUR);

   mon_adresse.sin_addr.s_addr = htonl(INADDR_ANY);

   bind(socket_ecoute, (struct sockaddr *)&mon_adresse, sizeof(mon_adresse));

   listen(socket_ecoute, MAX_PENDING_CONNECTIONS);

   printf("\n WAITING A CONNECTION\n\n");


   socket_donnees = accept(socket_ecoute, NULL, 0);

   printf("\nGOT CONNECTION !!!\n\n");

   Pilot_start();

   running = 1;
   while(running)
   {
      readMsg();
   }
}


/**
 * Stop AdminUI
 *
 */
void server_stop(){

   close(socket_donnees);
   Pilot_stop();
}
   