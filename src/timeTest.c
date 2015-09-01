/*
 * timeTest.c
 *
 *  Created on: Sep 1, 2015
 *      Author: haoran
 */

#include <time.h>

#include "timeTest.h"


void print_tm(const struct tm *ptm){
	printf("tm_sec:		%d\n", ptm->tm_sec);
	printf("tm_min:		%d\n", ptm->tm_min);
	printf("tm_hour:	%d\n", ptm->tm_hour);
	printf("tm_mday:	%d\n", ptm->tm_mday);
	printf("tm_mon:		%d\n", ptm->tm_mon);
	printf("tm_year:	%d\n", ptm->tm_year);
	printf("tm_wday:	%d\n", ptm->tm_wday);
	printf("tm_yday:	%d\n", ptm->tm_yday);
	printf("tm_isdst:	%d\n", ptm->tm_isdst);
}

void timeTest(){
	time_t time1;
	time(&time1);
	struct tm *utc_tm1 = gmtime(&time1);
	print_tm(utc_tm1);
}
