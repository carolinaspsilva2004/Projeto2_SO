/**
 *  \file sharedDataSync.h (interface file)
 *
 *  \brief Problem name: Restaurant
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  \brief Definition of the shared data and the synchronization devices.
 *
 *  Both the format of the shared data, which represents the full state of the problem, and the identification of
 *  the different semaphores, which carry out the synchronization among the intervening entities, are provided.
 *
 *  \author Nuno Lau - December 2023
 */

#ifndef SHAREDDATASYNC_H_
#define SHAREDDATASYNC_H_

#include "probConst.h"
#include "probDataStruct.h"

/**
 *  \brief Definition of <em>shared information</em> data type.
 */
typedef struct
        { /** \brief full state of the problem */
          FULL_STAT fSt;

          /* semaphores ids */
          /** \brief identification of critical region protection semaphore – val = 1 */
          unsigned int mutex;
          /** \brief identification of semaphore used by receptionist to wait for groups - val = 0 */
          unsigned int receptionistReq;
          /** \brief identification of semaphore used by groups to wait before issuing receptionist request - val = 1 */
          unsigned int receptionistRequestPossible;
          /** \brief identification of semaphore used by waiter to wait for requests – val = 0  */
          unsigned int waiterRequest;
          /** \brief identification of semaphore used by groups and chef to wait before issuing waiter request - val = 1 */
          unsigned int waiterRequestPossible;
          /** \brief identification of semaphore used by chef to wait for order – val = 0  */
          unsigned int waitOrder;
          /** \brief identification of semaphore used by waiter to wait for chef – val = 0  */
          unsigned int orderReceived;
          /** \brief identification of semaphore used by groups to wait for table – val = 0 */
          unsigned int waitForTable[MAXGROUPS];
          /** \brief identification of semaphore used by groups to wait for waiter ackowledge – val = 0  */
          unsigned int requestReceived[NUMTABLES];
          /** \brief identification of semaphore used by groups to wait for food – val = 0 */
          unsigned int foodArrived[NUMTABLES];
          /** \brief identification of semaphore used by groups to wait for payment completed – val = 0 */
          unsigned int tableDone[NUMTABLES];

        } SHARED_DATA;

/** \brief number of semaphores in the set */
#define SEM_NU               ( 7 + sh->fSt.nGroups + 3*NUMTABLES )

#define MUTEX                  1
#define RECEPTIONISTREQ        2
#define RECEPTIONISTREQUESTPOSSIBLE  3
#define WAITERREQUEST          4
#define WAITERREQUESTPOSSIBLE  5
#define WAITORDER              6
#define ORDERRECEIVED          7
#define WAITFORTABLE           8
#define FOODARRIVED            (WAITFORTABLE+sh->fSt.nGroups)
#define REQUESTRECEIVED        (FOODARRIVED+NUMTABLES)
#define TABLEDONE              (REQUESTRECEIVED+NUMTABLES)

#endif /* SHAREDDATASYNC_H_ */
