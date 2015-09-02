/*
 * timeTest.c
 *
 *  Created on: Sep 1, 2015
 *      Author: haoran
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

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
	printf("==========timeTest1==========\n");
	time_t paratime;
	time(&paratime);
	printf("test gmtim():\n");
	struct tm *utc_tm = gmtime(&paratime);
	print_tm(utc_tm);

	printf("test localtime():\n");
	struct tm *loc_tm = localtime(&paratime);
	print_tm(loc_tm);


	printf("==========timeTest2==========\n");
	printf("test clock_gettime():\n");
	struct timeval tvl;
	clock_gettime(CLOCK_REALTIME, &tvl);
	utc_tm = gmtime(&tvl.tv_sec);
	print_tm(utc_tm);
	printf("micro second: %ld\n", tvl.tv_usec);

	printf("==========timeTest3==========\n");
	printf("test mktime()\n");
	time_t ttmp = mktime(utc_tm);
	printf("time_t from mktime() is: %ld\n", ttmp);
}

void timePrintTest(){
	time_t t;
	struct tm *tmp;
	char buf1[16];
	char buf2[64];

	time(&t);
	tmp = localtime(&t);
	if (strftime(buf1, 16, "time and date: %r, %a, %b, %d, %Y", tmp) == 0)
		printf("buffer length 16 is too small\n");
	else
		printf("%s\n", buf1);
	if (strftime(buf2, 64, "time and date: %r, %a, %b, %d, %Y", tmp) == 0)
		printf("buffer length 64 is too small\n");
	else
		printf("%s\n", buf2);

	struct tm tmp2;
	char *p = buf2 + strlen("time and date: ");
	if (NULL == strptime(p, "%r, %a, %b, %d, %Y", &tmp2)) {
		printf("some error occured\n");
	}
	print_tm(&tmp2);
}


