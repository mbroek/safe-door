/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Safe Door maintenance tool
 *
 *****************************************************************************
 * Copyright (C) 2003-2005
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
 * Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *****************************************************************************/

#include "../config.h"
#include <stdlib.h>
#include <door.h>
#include "safe-tool.h"


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
char    *yes = NULL;
char    *no = NULL;
char    *lang1 = NULL;
char    *lang2 = NULL;
char    *lang3 = NULL;
char    *lang4 = NULL;
char    *lang5 = NULL;
char    *lang6 = NULL;
char    *lang7 = NULL;
char    *lang8 = NULL;
char    *lang9 = NULL;
char    *lang10 = NULL;
char    *lang11 = NULL;
char    *lang12 = NULL;
char    *lang13 = NULL;
char    *lang14 = NULL;
char    *lang15 = NULL;
char    *lang16 = NULL;
char    *lang17 = NULL;
char    *lang18 = NULL;
char    *lang19 = NULL;
char    *lang20 = NULL;
char    *lang21 = NULL;
char    *lang22 = NULL;
char    *lang23 = NULL;
char    *lang24 = NULL;
char	*lang25 = NULL;


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
    {(char *)"yes",         door_parse_getstr,      (char **)&yes},
    {(char *)"no",          door_parse_getstr,      (char **)&no},
    {(char *)"lang1",       door_parse_getstr,      (char **)&lang1},
    {(char *)"lang2",       door_parse_getstr,      (char **)&lang2},
    {(char *)"lang3",       door_parse_getstr,      (char **)&lang3},
    {(char *)"lang4",       door_parse_getstr,      (char **)&lang4},
    {(char *)"lang5",       door_parse_getstr,      (char **)&lang5},
    {(char *)"lang6",       door_parse_getstr,      (char **)&lang6},
    {(char *)"lang7",       door_parse_getstr,      (char **)&lang7},
    {(char *)"lang8",       door_parse_getstr,      (char **)&lang8},
    {(char *)"lang9",       door_parse_getstr,      (char **)&lang9},
    {(char *)"lang10",      door_parse_getstr,      (char **)&lang10},
    {(char *)"lang11",      door_parse_getstr,      (char **)&lang11},
    {(char *)"lang12",      door_parse_getstr,      (char **)&lang12},
    {(char *)"lang13",      door_parse_getstr,      (char **)&lang13},
    {(char *)"lang14",      door_parse_getstr,      (char **)&lang14},
    {(char *)"lang15",      door_parse_getstr,      (char **)&lang15},
    {(char *)"lang16",      door_parse_getstr,      (char **)&lang16},
    {(char *)"lang17",      door_parse_getstr,      (char **)&lang17},
    {(char *)"lang18",      door_parse_getstr,      (char **)&lang18},
    {(char *)"lang19",      door_parse_getstr,      (char **)&lang19},
    {(char *)"lang20",      door_parse_getstr,      (char **)&lang20},
    {(char *)"lang21",      door_parse_getstr,      (char **)&lang21},
    {(char *)"lang22",      door_parse_getstr,      (char **)&lang22},
    {(char *)"lang23",      door_parse_getstr,      (char **)&lang23},
    {(char *)"lang24",      door_parse_getstr,      (char **)&lang24},
    {(char *)"lang25",	    door_parse_getstr,	    (char **)&lang25},
    {NULL,                  NULL,                   NULL}
};


void	tool(char *);



int main(int argc, char **argv)
{
    /*
     * Initialize and load configuration.
     */
    if (argc != 2) {
	fprintf(stderr, "Usage: %s /path/to/configfile\n\n", argv[0]);
	exit(1);
    }
    if (door_parse_config(argv[1], keytab)) {
	fprintf(stderr, "Parsing configuration failed\n\n");
	exit(1);
    }
    door_loginit(logfile, (char *)"safe-tool", FALSE);
    door_log(' ', "");
    door_log(' ', "SAFE-TOOL v%s started", VERSION);
    door_log('+', "User starts Safe Cracker maintenance tool");

    fprintf(stdout, "SAFE-TOOL v%s\n", VERSION);
    fprintf(stdout, "%s\n\n", COPYRIGHT);
    tool(argv[1]);

    door_log(' ', "SAFE-TOOL finished");
    return 0;
}



void tool(char *config)
{
    FILE    *fp;
    char    *temp;
    int	    records = 0, cracked = FALSE;

    if (numgenerator) {
	fprintf(stdout, "Safe combination : Random generated\n");
    } else {
	fprintf(stdout, "Safe combination : %ld %ld %ld\n", digit1, digit2, digit3);
    }
    fprintf(stdout, "Maximum tries ...: %ld\n", maxtries);
    fprintf(stdout, "Maximum number ..: %ld\n", maxnumber);
    fprintf(stdout, "Prize ...........: %s\n\n", prize);

    temp = calloc(PATH_MAX, sizeof(char));
    snprintf(temp, PATH_MAX, "%s/safe.data", workdir);

    if ((fp = fopen(temp, "r+")) == NULL) {
	fprintf(stdout, "There is no safe.data file, the door is never used!\n\n");
	free(temp);
	return;
    }
    fread(&safehdr, sizeof(safehdr), 1, fp);
    while (fread(&safe, safehdr.recsize, 1, fp) == 1) {
	records++;
    }
    if (!records) {
	fclose(fp);
	free(temp);
	fprintf(stdout, "There are no records in the safe.data file!\n\n");
	return;
    }
    
    fprintf(stdout, "Date         User                                 Trys Opened\n");
    fprintf(stdout, "------------ ------------------------------------ ---- ------\n");
    fseek(fp, safehdr.hdrsize, SEEK_SET);

    while (fread(&safe, safehdr.recsize, 1, fp) == 1) {
	fprintf(stdout, "%-12s %-36s %4d %s\n", safe.Date, safe.Name, safe.Trys, safe.Opened?"Yes":"No");
	if (safe.Opened) {
	    cracked = TRUE;
	    door_log('+', "The safed is cracked by user %s", safe.Name);
	}
    }
    if (!cracked) {
	fclose(fp);
	free(temp);
	fprintf(stdout, "\nNobody has cracked the safe yet!\n\n");
	return;
    }

    /*
     * The safe is cracked, option to reset the safe.
     */
    fprintf(stdout, "\nThe safe is cracked, reset the safe [y/N]: ");
    fflush(stdout);
    if (toupper(getchar()) == 'Y') {

	fseek(fp, safehdr.hdrsize, SEEK_SET);
	while (fread(&safe, safehdr.recsize, 1, fp) == 1) {
	    if (safe.Opened) {
		door_log('+', "Resetting safe record of user %s", safe.Name);
		fprintf(stdout, "Resetting safe record of user %s\n", safe.Name);
		safe.Trys = 0;
		safe.Opened = FALSE;
		fseek(fp, - safehdr.recsize, SEEK_CUR);
		fwrite(&safe, safehdr.recsize, 1, fp);
	    }
	}

	if (!numgenerator) {
	    fprintf(stdout, "*** REMEMBER, edit %s to enter a new combination\n", config);
	}
    }

    fclose(fp);
    free(temp);
    fprintf(stdout, "\n");
}


