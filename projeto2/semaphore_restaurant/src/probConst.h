/**
 *  \file probConst.h (interface file)
 *
 *  \brief Problem name: Restaurant
 *
 *  Problem simulation parameters.
 *
 *  \author Nuno Lau - December 2023
 */

#ifndef PROBCONST_H_
#define PROBCONST_H_

/* Generic parameters */

/** \brief maximum number of groups */
#define  MAXGROUPS       16 
/** \brief number of tables */
#define  NUMTABLES        2 
/** \brief controls time taken to cook */
#define  MAXCOOK        100

/** \brief controls start time standard deviation */
#define  STARTDEV         4 
/** \brief controls eat time standard deviation */
#define  EATDEV           4 

/** \brief id of table request (group->receptionist) */
#define TABLEREQ   1
/** \brief id of bill request (group->receptionist) */
#define BILLREQ    2
/** \brief id of food request (group->waiter) */
#define FOODREQ   3
/** \brief id of food ready (chef->waiter) */
#define FOODREADY 4

/* Client state constants */

/** \brief group initial state */
#define  GOTOREST          1
/** \brief client is waiting at reception or waiting for table */
#define  ATRECEPTION       2
/** \brief client is requesting food to waiter */
#define  FOOD_REQUEST      3
/** \brief client is waiting for food */
#define  WAIT_FOR_FOOD     4
/** \brief client is eating */
#define  EAT               5
/** \brief client is checking out */
#define  CHECKOUT          6
/** \brief client is leaving */
#define  LEAVING           7

/* Chef state constants */

/** \brief chef waits for food order */
#define  WAIT_FOR_ORDER    0
/** \brief chef is cooking */
#define  COOK              1
/** \brief chef is resting */
#define  REST              2

/* Waiter state constants */

/** \brief waiter waits for food request */
#define  WAIT_FOR_REQUEST   0
/** \brief waiter takes food request to chef */
#define  INFORM_CHEF        1
/** \brief waiter takes food to table */
#define  TAKE_TO_TABLE      2

/* Receptionist state constants */

/** \brief waiter waits for food request */
#define  ASSIGNTABLE        1
/** \brief waiter reiceives payment */
#define  RECVPAY            2


#endif /* PROBCONST_H_ */
