#ifndef	_SAFETOOL_H
#define	_SAFETOOL_H

/* $Id$ */

/*
 * Structure for Safe Cracker Door Data File (safe.data)
 */
struct  crackerhdr {
    long            hdrsize;                /* Size of header          */
    long            recsize;                /* Size of records         */
};

struct  cracker {
    char            Date[12];               /* Date used               */
    char            Name[36];               /* User name               */
    int             Trys;                   /* Trys today              */
    unsigned        Opened : 1;             /* If user succeeded       */
};

struct  crackerhdr      safehdr;                /* Safe cracker structure  */
struct  cracker         safe;


#endif
