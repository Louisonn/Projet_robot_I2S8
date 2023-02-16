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

#include "client.h"

#define MAX_PENDING_CONNECTIONS 1
#define PORT_DU_SERVEUR (12355)
#define POWER 50



//*******************Private variables *******************

int socket_donnees;

//*******************Private function definition *******************

void client_sendMsg(transmitted_data_t * transmitted_data){
   int transmitted_quantity;
   transmitted_quantity = write(socket_donnees, transmitted_data, sizeof(*transmitted_data));
   if(transmitted_quantity == -1)
   {
      printf("Error : lost connection with Telco.\nCouldn't send data.");
   }
}

void client_readMsg(received_data_t * state){
   int received_quantity;
   received_data_t tmp_state = {0, 0};
   received_quantity = read(socket_donnees, &tmp_state, sizeof(tmp_state));
   if(received_quantity > 0)
   {
      *state = tmp_state;
   }
}

//*******************Public function definition *******************
void client_start(){

   struct sockaddr_in adresse_du_serveur;

   socket_donnees = socket (PF_INET, SOCK_STREAM, 0);
   adresse_du_serveur.sin_family = AF_INET;
   adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);

   adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname("127.0.0.1")->h_addr_list[0]);

   connect(socket_donnees, (struct sockaddr *)&adresse_du_serveur,sizeof(adresse_du_serveur));
}


/**
 * Stop AdminUI
 *
 */
void client_stop(){

   close(socket_donnees);
   exit(0);
}

   