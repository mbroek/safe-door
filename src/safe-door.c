/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Safe Door
 *
 *****************************************************************************
 * Copyright (C) 2003
 *   
 * Michiel Broek		FIDO:		2:280/2802
 * Beekmansbos 10
 * 1971 BV IJmuiden
 * the Netherlands
 *
 * This file is part of Safe Cracker door.
 *
 * This door is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * Safe Cracker is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MBSE BBS; see the file COPYING.  If not, write to the Free
 * Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *****************************************************************************/

#include "../config.h"
#include <stdlib.h>
#include <door/doorlib.h>
#include "safe-door.h"


char    *logfile = NULL;
char    *bbsname = NULL;
long	digit1 = 0;
long	digit2 = 0;
long	digit3 = 0;
long	maxtries = 0;
long	maxnumber = 0;
long	numgenerator = 0;
char	*prize = NULL;
char	*welcome = NULL;
char	*opened = NULL;
char	*workdir = NULL;

keytab_t keytab[] = {
    {(char *)"logfile",     door_parse_getstr,      (char **)&logfile},
    {(char *)"bbsname",     door_parse_getstr,      (char **)&bbsname},
    {(char *)"digit1",	    door_parse_getlong,	    (char **)&digit1},
    {(char *)"digit2",      door_parse_getlong,     (char **)&digit2},
    {(char *)"digit3",      door_parse_getlong,     (char **)&digit3},
    {(char *)"maxtries",    door_parse_getlong,	    (char **)&maxtries},
    {(char *)"maxnumber",   door_parse_getlong,	    (char **)&maxnumber},
    {(char *)"prize",	    door_parse_getstr,	    (char **)&prize},
    {(char *)"welcome",	    door_parse_getstr,	    (char **)&welcome},
    {(char *)"opened",	    door_parse_getstr,	    (char **)&opened},
    {(char *)"workdir",	    door_parse_getstr,	    (char **)&workdir},
    {(char *)"numgenerator",door_parse_getlong,	    (char **)&numgenerator},
    {NULL,                  NULL,                   NULL}
};


FILE *pSafe;

int iLoop, iFirst, iSecond, iThird;
char sFirst[4], sSecond[4], sThird[4];
int cracked;
int tries;

int  getdigits(void);
int  SafeCheckUser(int);
char *GetDateDMY(void);


int main(int argc, char **argv)
{
    long    isize;
    int	    i;
    time_t  Now;

    /*
     * Initialize and load configuration.
     */
    if (argc != 2) {
	fprintf(stderr, "Usage: %s /path/to/configfile", argv[0]);
	exit(1);
    }
    if (door_parse_config(argv[1], keytab)) {
	fprintf(stderr, "Parsing configuration failed");
	exit(1);
    }
    door_loginit(logfile, (char *)"safe-door", FALSE);

    /*
     * Now basic stuff is setup and logging is available.
     */
    if (door_load_doorsys() == FALSE) {
	exit(1);
    }

    isize = sizeof(int);
    Now = time(NULL);
    srand(Now);
    cracked = FALSE;
    tries = 0;

    door_log('+', "User starts Safe Cracker Door");

    door_clear();
    door_dispfile(welcome);

    if (SafeCheckUser(TRUE) == TRUE)
	return 1;

    door_cout(WHITE, BLACK, (char *)"In the safe lies ");
    door_enter(2);
    door_color(LIGHTMAGENTA, BLACK);
    printf("%s", prize);
    door_enter(2);
    door_cout(WHITE, BLACK, (char *)"Do you want to open the safe ? [Y/n]: ");
    fflush(stdout);
    
    door_alarm_on();
    i = toupper(door_getch());

    if (i == 'N') {
	door_log('+', "User exited Safe Cracker Door");
	door_color(LIGHTGRAY, BLACK);
	door_enter(2);
	fflush(stdout);
	return 1;
    }

    /*
     * Loop until the safe is opened, maximum trys
     * exceeded or the user is tired of this door.
     */
    while (TRUE) {
	/* Get digits, TRUE if safe cracked. */
	if (getdigits() == TRUE) {
	    SafeCheckUser(FALSE);
	    break;
	}

	door_enter(1);
	door_cout(LIGHTRED, BLACK, (char *)"Do you want to try again ? [Y/n]: ");
	fflush(stdout);

	door_alarm_on();
	i = toupper(door_getch());
	if (i == 'N') {
	    SafeCheckUser(FALSE);
	    break;
	}

	if (SafeCheckUser(FALSE) == TRUE)
	    break;
    }
    door_log('+', "User exited Safe Cracker Door");
    return 0;
}



/*
 * Ask user for digits, returns TRUE if the safe is cracked.
 */
int getdigits(void)
{
    int	    i;
    char    temp[81];

    door_color(WHITE, BLACK);
    printf("\n\nPlease enter three numbers consisting from 1 to %ld\n", maxnumber);
    printf("%s", (char *)"Please enter three combinations.");

    while (TRUE) {
	door_enter(2);
	door_cout(LIGHTRED, BLACK, (char *)"1st Digit: ");
	door_color(LIGHTBLUE, BLACK);
	fflush(stdout);
	door_getnum(sFirst, 2);
	sprintf(temp, "1st: %s", sFirst);
	if((strcmp(sFirst, "")) != 0) {
	    iFirst=atoi(sFirst);
	}

	if ((iFirst > maxnumber) || (iFirst <= 0) || (strcmp(sFirst, "") == 0)) {
	    door_color(WHITE, BLUE);
	    printf("\nPlease try again! You must input a number greater than Zero and less than %ld.", maxnumber);
	} else 
	    break;
    }

    while (TRUE) {
	door_enter(1);
	door_cout(LIGHTRED, BLACK, (char *)"2nd digit: ");
	door_color(LIGHTBLUE, BLACK);
	fflush(stdout);
	door_getnum(sSecond, 2);
	sprintf(temp, "2nd: %s", sSecond);
	if((strcmp(sSecond, "")) != 0) {
	    iSecond=atoi(sSecond);
	}

	if((iSecond > maxnumber) || (iSecond <= 0) || (strcmp(sSecond, "") == 0)) {
	    door_color(WHITE, BLUE);
	    printf("\nPlease try again! You must input a number greater than Zero and less than %ld.\n", maxnumber);
	} else
	    break;
    }

    while (TRUE) {
	door_enter(1);
	door_cout(LIGHTRED, BLACK, (char *)"3rd digit: ");
	door_color(LIGHTBLUE, BLACK);
	fflush(stdout);
	door_getnum(sThird, 2);
	sprintf(temp, "3rd: %s", sThird);
	if((strcmp(sThird, "")) != 0) {
	    iThird=atoi(sThird);
	}

	if((iThird > maxnumber) || (iThird <= 0) || (strcmp(sThird, "") == 0)) {
	    door_color(WHITE, BLUE);
	    printf("\nPlease try again! You must input a number greater than Zero and less than %ld.\n", maxnumber);
	} else
	    break;
    }

    door_enter(1);
    door_cout(LIGHTRED, BLACK, (char *)"Left : ");
    door_cout(LIGHTBLUE, BLACK, sFirst);
    
    door_enter(1);
    door_cout(LIGHTRED, BLACK, (char *)"Right: ");
    door_cout(LIGHTBLUE, BLACK, sSecond);
    
    door_enter(1);
    door_cout(LIGHTRED, BLACK, (char *)"Left : ");
    door_cout(LIGHTBLUE, BLACK, sThird);
    
    door_enter(2);
    door_cout(LIGHTRED, BLACK, (char *)"Attempt to open safe with this combination [Y/n]: ");
    fflush(stdout);
    door_alarm_on();
    i = toupper(door_getch());
    sprintf(temp, "%c", i);

    if ((i == 'Y') || (i == 13)) {
	printf("\n\n");
	tries++;
	door_log('+', "Attempt %d with combination %d %d %d", tries, iFirst, iSecond, iThird);

	door_cout(LIGHTRED, BLACK, (char *)"Left : ");
	for (iLoop = 0; iLoop < iFirst; iLoop++) {
	    door_cout(YELLOW, BLACK, (char *)".");
	    fflush(stdout);
	    usleep(100000);
	}
	door_cout(LIGHTBLUE, BLACK, sFirst);
	door_enter(1);

	door_cout(LIGHTRED, BLACK, (char *)"Right: ");
	for (iLoop = 0; iLoop < iSecond; iLoop++) {
	    door_cout(YELLOW, BLACK, (char *)".");
	    fflush(stdout);
	    usleep(100000);
	}
	door_cout(LIGHTBLUE, BLACK, sSecond);
	door_enter(1);

	door_cout(LIGHTRED, BLACK, (char *)"Left : ");
	for (iLoop = 0; iLoop < iThird; iLoop++) {
	    door_cout(YELLOW, BLACK, (char *)"."); 
	    fflush(stdout);
	    usleep(100000);
	}
	door_cout(LIGHTBLUE, BLACK, sThird);
	door_enter(1);

	if (numgenerator) {
	    digit1 = (rand() % maxnumber) + 1;
	    digit2 = (rand() % maxnumber) + 1;
	    digit3 = (rand() % maxnumber) + 1;
	}

	if ((digit1 == iFirst) && (digit2 == iSecond) && (digit3 == iThird)) {

	    door_dispfile(opened);
	    cracked = TRUE;

	    door_enter(1);
	    door_cout(LIGHTRED, BLACK, (char *)"You have won the following...");
	    door_enter(2);
	    door_cout(LIGHTMAGENTA, BLACK, prize);
	    door_enter(1);

	    door_log('!', "User opened Safe Cracker Door");

	    door_waitenter();
	    door_enter(2);
	    door_color(LIGHTGRAY, BLACK);
	    fflush(stdout);
	    return TRUE;
	}

	door_enter(1);
	door_cout(LIGHTGREEN, BLACK, (char *)"Sorry - You didn't open the safe!");
	door_enter(1);

	if (numgenerator) {
	    door_enter(1);
	    door_cout(LIGHTRED, BLACK, (char *)"The safe code was:");
	    door_enter(2);
	    door_color(LIGHTRED, BLACK);
	    printf("Left : %ld\n", digit1);
	    printf("Right: %ld\n", digit2);
	    printf("Left : %ld\n", digit3);
	}

	door_enter(1);
	door_cout(LIGHTGREEN, BLACK, (char *)"Please press key to continue");
	door_alarm_on();
	door_getch();
    }
    return FALSE;
}



/*
 * Returns true when safe already cracked or maximum trys exceeded
 */
int SafeCheckUser(int init)
{
    char *File, *Name, *Date;

    File = calloc(PATH_MAX, sizeof(char));
    Name = calloc(50, sizeof(char));
    Date = calloc(50, sizeof(char));

    sprintf(Name, "%s", doorsys.username);
    sprintf(Date, "%s", (char *) GetDateDMY());
    sprintf(File, "%s/safe.data", workdir);

    if ((pSafe = fopen(File, "r+")) == NULL) {
	if ((pSafe = fopen(File, "w")) != NULL) {
	    safehdr.hdrsize = sizeof(safehdr);
	    safehdr.recsize = sizeof(safe);
	    fwrite(&safehdr, sizeof(safehdr), 1, pSafe);
	    sprintf(safe.Date, "%s", (char *) GetDateDMY());
	    sprintf(safe.Name, "%s", Name);
	    safe.Trys   = 0;
	    safe.Opened = FALSE;
	    fwrite(&safe, sizeof(safe), 1, pSafe);
	    fclose(pSafe);
	    chmod(File, 0660);
	}
    } else {
	fread(&safehdr, sizeof(safehdr), 1, pSafe);
	/*
	 * Check if safe already cracked
	 */
	while (fread(&safe, safehdr.recsize, 1, pSafe) == 1) {
	    if (safe.Opened) {
		fclose(pSafe);
		door_log('+', "Safe is currently LOCKED - exiting door.");

		door_cout(WHITE, RED, (char *)"THE SAFE IS CURRENTLY LOCKED");
		door_enter(2);
		door_color(LIGHTRED, BLACK);

		printf("%s, %s\n", safe.Name, (char *)"has cracked the safe.");

		door_cout(LIGHTGREEN, BLACK, (char *)"The safe will remain locked until the sysop rewards the user.");
		door_enter(2);
		door_waitenter();
		door_color(LIGHTGRAY, BLACK);
		door_enter(2);
		free(File);
		free(Name);
		free(Date);
		return TRUE;
	    }
	}
	fseek(pSafe, safehdr.hdrsize, SEEK_SET);

	/*
	 * Check if this user is already in the database
	 */
	while (fread(&safe, safehdr.recsize, 1, pSafe) == 1) {
	    if ((strcmp(Name, safe.Name)) == 0) {
		if ((strcmp(Date, safe.Date)) != 0) {
		    /*
		     * User found, but last time used is not today.
		     * Reset this user.
		     */
		    fseek(pSafe, - safehdr.recsize, SEEK_CUR);
		    sprintf(safe.Date, "%s", (char *) GetDateDMY());
		    safe.Trys = 0;
		    tries = 0;
		    safe.Opened = FALSE;
		    fwrite(&safe, safehdr.recsize, 1, pSafe);
		    fclose(pSafe);
		    free(File);
		    free(Name);
		    free(Date);
		    return FALSE;
		} else {
		    /*
		     * User found, last time is today, check attempts
		     */
		    fseek(pSafe, - safehdr.recsize, SEEK_CUR);
		    if (init)
			tries = safe.Trys;
		    else {
			safe.Trys = tries;
		    }
		    safe.Opened = cracked;
		    fwrite(&safe, safehdr.recsize, 1, pSafe);
		    fclose(pSafe);
		    free(File);
		    free(Name);
		    free(Date);
		    if (safe.Trys >= maxtries) {
			door_log('+', "Maximum trys per day exceeded");
			door_enter(2);
			door_cout(WHITE, BLACK, (char *)"Maximum trys per day exceeded");
			door_enter(1);
			sleep(3);
			return TRUE;
		    }
		    return FALSE;
		}
	    }
	}
	
	/*
	 * User not found, append new record
	 */
	fclose(pSafe);
	if ((pSafe = fopen(File, "a")) == NULL) {
	    door_log('?', "Can't append to %s", File);
	    free(File);
	    free(Name);
	    free(Date);
	    return TRUE;
	}
	fseek(pSafe, 0, SEEK_END);
	memset(&safe, 0, sizeof(safe));
	sprintf(safe.Date, "%s", (char *) GetDateDMY());
	sprintf(safe.Name, "%s", Name);
	safe.Trys   = 0;
	safe.Opened = FALSE;
	tries = 0;
	fwrite(&safe, sizeof(safe), 1, pSafe);
	fclose(pSafe);
	door_log('+', "Append new safe.data record");
    }

    free(File);
    free(Name);
    free(Date);
    return FALSE;
}



/*
 * This function returns the date for today, to test against other functions
 *                 DD-MM-YYYY (DAY-MONTH-YEAR)
 */
char *GetDateDMY()
{
    static char     tdate[15];
    struct tm       *l_d;
    time_t          T_Now;

    T_Now = time(NULL);
    l_d = localtime(&T_Now);
    sprintf(tdate, "%02d-%02d-%04d", l_d->tm_mday,l_d->tm_mon+1,l_d->tm_year+1900);
    return(tdate);
}


