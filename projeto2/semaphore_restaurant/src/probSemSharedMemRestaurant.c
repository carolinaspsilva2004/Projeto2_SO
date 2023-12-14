/**
 *  \file probSemSharedMemRestaurant.c (implementation file)
 *
 *  \brief Problem name: Restaurant
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Generator process of the intervening entities.
 *
 *  Upon execution, one parameter is requested:
 *    \li name of the logging file.
 *
 *  \author Nuno Lau - December 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <math.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "logging.h"
#include "sharedDataSync.h"
#include "semaphore.h"
#include "sharedMemory.h"

/** \brief name of chef process */
#define   CHEF               "./chef"

/** \brief name of waiter process */
#define   WAITER             "./waiter"

/** \brief name of group process */
#define   GROUP              "./group"

/** \brief name of chef process */
#define   RECEPTIONIST       "./receptionist"
/**
 *  \brief Main program.
 *
 *  Its role is starting the simulation by generating the intervening entities processes (pilot, hostess and passengers)
 *  and waiting for their termination.
 */
int main (int argc, char *argv[])
{
    char nFic[51];                                                                              /*name of logging file */
    char nFicErr[] = "error_        ";                                                     /* base name of error files */
    int shmid,                                                                      /* shared memory access identifier */
        semgid;                                                                     /* semaphore set access identifier */
    unsigned int  m;                                                                             /* counting variables */
    SHARED_DATA *sh;                                                                /* pointer to shared memory region */
    int pidCH,                                                                             /* pilot process identifier */
        pidWT,                                                                     /* hostess process identifier array */
        pidRT,                                                                     /* hostess process identifier array */
        pidGR[MAXGROUPS];                                                     /* passengers processes identifier array */
    int key;                                                           /*access key to shared memory and semaphore set */
    char num[2][12];                                                     /* numeric value conversion (up to 10 digits) */
    int status,                                                                                    /* execution status */
        info;                                                                                               /* info id */
    int g, t;

    /* getting log file name */
    if(argc==2) {
        strcpy(nFic, argv[1]);
    }
    else strcpy(nFic, "");

    /* composing command line */
    if ((key = ftok (".", 'a')) == -1) {
        perror ("error on generating the key");
        exit (EXIT_FAILURE);
    }
    sprintf (num[1], "%d", key);

    /* creating and initializing the shared memory region and the log file */
    if ((shmid = shmemCreate (key, sizeof (SHARED_DATA))) == -1) { 
        perror ("error on creating the shared memory region");
        exit (EXIT_FAILURE);
    }
    if (shmemAttach (shmid, (void **) &sh) == -1) { 
        perror ("error on mapping the shared region on the process address space");
        exit (EXIT_FAILURE);
    }

    /* initialize random generator */
    srandom ((unsigned int) getpid ());                                

    /* initialize problem internal status */
    sh->fSt.st.chefStat         = WAIT_FOR_ORDER;                     /* the chef waits for an order */
    sh->fSt.st.waiterStat       = WAIT_FOR_REQUEST;                /* the waiter waits for a request */
    sh->fSt.st.receptionistStat = WAIT_FOR_REQUEST;          /* the receptionist waits for a request */
    for (g = 0; g < MAXGROUPS; g++) {
        sh->fSt.st.groupStat[g] = GOTOREST;                                /* groups are initialized */
        sh->fSt.assignedTable[g] = -1;                                     /* groups are initialized */
    }
    sh->fSt.groupsWaiting=0;

    FILE *fp = fopen("config.txt","r");
    if(fp==NULL) {
        perror("Could not open config file");
        exit(EXIT_FAILURE);
    }

    /* parse config file */
    fscanf(fp,"%*[^\n]");
    fscanf(fp,"%d ",&sh->fSt.nGroups);
    fscanf(fp,"%*[^\n]");
    for(g=0;g < sh->fSt.nGroups;g++) {
       fscanf(fp,"%d %d", &sh->fSt.startTime[g], &sh->fSt.eatTime[g]);
    }
   
    /* create log file */
    createLog (nFic, &sh->fSt);                                  
    saveState(nFic,&sh->fSt);

    /* initialize semaphore ids */
    sh->mutex                       = MUTEX;                                /* mutual exclusion semaphore id */
    sh->receptionistReq             = RECEPTIONISTREQ;                                                      
    sh->receptionistRequestPossible = RECEPTIONISTREQUESTPOSSIBLE;                                                      
    sh->waiterRequest               = WAITERREQUEST;                                                      
    sh->waiterRequestPossible       = WAITERREQUESTPOSSIBLE;                                                      
    sh->waitOrder                   = WAITORDER;                                                      
    sh->orderReceived               = ORDERRECEIVED;                                                      
    for(g=0;g<sh->fSt.nGroups;g++) {
       sh->waitForTable[g]          = WAITFORTABLE+g;                                                      
    }
    for(t=0;t<NUMTABLES;t++) {
       sh->foodArrived[t]           = FOODARRIVED+t;                                                      
       sh->tableDone[t]             = TABLEDONE+t;                                                      
       sh->requestReceived[t]       = REQUESTRECEIVED+t;                              
    }

    /* creating and initializing the semaphore set */
    if ((semgid = semCreate (key, SEM_NU)) == -1) { 
        perror ("error on creating the semaphore set");
        exit (EXIT_FAILURE);
    }
    if (semUp (semgid, sh->mutex) == -1) {                   /* enabling access to critical region */
        perror ("error on executing the up operation for semaphore access");
        exit (EXIT_FAILURE);
    }
    if (semUp (semgid, sh->waiterRequestPossible) == -1) {                   /* enabling access to critical region */
        perror ("error on executing the up operation for semaphore access");
        exit (EXIT_FAILURE);
    }
    if (semUp (semgid, sh->receptionistRequestPossible) == -1) {                   /* enabling access to critical region */
        perror ("error on executing the up operation for semaphore access");
        exit (EXIT_FAILURE);
    }

    /* generation of intervening entities processes */                            
    /* group processes */
    strcpy (nFicErr + 6, "GR");
    for (g = 0; g < sh->fSt.nGroups; g++) {           
        if ((pidGR[g] = fork ()) < 0) {
            perror ("error on the fork operation for the group");
            exit (EXIT_FAILURE);
        }
        sprintf(num[0],"%d",g);
        sprintf(nFicErr+8,"%02d",g); 
        if (pidGR[g] == 0)
            if (execl (GROUP, GROUP, num[0], nFic, num[1], nFicErr, NULL) < 0) { 
                perror ("error on the generation of the group process");
                exit (EXIT_FAILURE);
            }
    }
    /* waiter process */
    strcpy (nFicErr + 6, "WT");
    if ((pidWT = fork ()) < 0)  {                            
        perror ("error on the fork operation for the waiter");
        exit (EXIT_FAILURE);
    }
    if (pidWT == 0) {
        if (execl (WAITER, WAITER, nFic, num[1], nFicErr, NULL) < 0) {
            perror ("error on the generation of the waiter process");
            exit (EXIT_FAILURE);
        }
    }
    /* chef process */
    strcpy (nFicErr + 6, "CH");
    if ((pidCH = fork ()) < 0) {               
        perror ("error on the fork operation for the chef");
        exit (EXIT_FAILURE);
    }
    if (pidCH == 0)
        if (execl (CHEF, CHEF, nFic, num[1], nFicErr, NULL) < 0) { 
            perror ("error on the generation of the chef process");
            exit (EXIT_FAILURE);
        }

    /* receptionist process */
    strcpy (nFicErr + 6, "RT");
    if ((pidRT = fork ()) < 0) {               
        perror ("error on the fork operation for the chef");
        exit (EXIT_FAILURE);
    }
    if (pidRT == 0)
        if (execl (RECEPTIONIST, RECEPTIONIST, nFic, num[1], nFicErr, NULL) < 0) { 
            perror ("error on the generation of the receptionist process");
            exit (EXIT_FAILURE);
        }

    /* signaling start of operations */
    if (semSignal (semgid) == -1) {
        perror ("error on signaling start of operations");
        exit (EXIT_FAILURE);
    }

    /* waiting for the termination of the intervening entities processes */
    m = 0;
    do {
        info = wait (&status);
        if (info == -1) { 
            perror ("error on aiting for an intervening process");
            exit (EXIT_FAILURE);
        }
        m += 1;
    } while (m < 3+sh->fSt.nGroups);

    /* destruction of semaphore set and shared region */
    if (semDestroy (semgid) == -1) {
        perror ("error on destructing the semaphore set");
        exit (EXIT_FAILURE);
    }
    if (shmemDettach (sh) == -1) { 
        perror ("error on unmapping the shared region off the process address space");
        exit (EXIT_FAILURE);
    }
    if (shmemDestroy (shmid) == -1) { 
        perror ("error on destructing the shared region");
        exit (EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
