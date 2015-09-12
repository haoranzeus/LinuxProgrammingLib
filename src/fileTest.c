/*
 * fileTest.c
 *
 *  Created on: Sep 7, 2015
 *      Author: haoran
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "timeTest.h"

void fileTest1(){
	int fd = open("/tmp/a.txt", O_WRONLY | O_TRUNC);
	if(-1 == fd) {
		perror("open error");
		return;
	}
	if (-1 == write(fd, "Hello World!", sizeof("Hello World!"))) {
		perror("write error");
		return;
	}
	if (-1 == close(fd)) {
		perror("close error");
		return;
	}
	fd = open("/tmp/a.txt", O_RDONLY);
	if (-1 == fd) {
		perror("open error");
		return;
	}
	char tmp[20];
	ssize_t readsize = read(fd, tmp, 20);
	if (-1 == readsize) {
		perror("read error");
		return;
	}
	printf("%s\n", tmp);
	if (-1 == lseek(fd, 6, SEEK_SET)) {
		perror("lseek error");
		return;
	}
	readsize = read(fd, tmp, 20);
	if (-1 == readsize) {
		perror("read error");
		return;
	}
	printf("%s\n", tmp);
}

void fileTest2() {
	int fd = open("/tmp/b.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (-1 == fd) {
		perror("open error");
		return;
	}
	if (-1 == write(fd, "This is the first line.\n", sizeof("This is the first line.\n") - 1)) {
		perror("write error");
		return;
	}
	if (-1 == close(fd)) {
		perror("close error");
		return;
	}
	fd = open("/tmp/b.txt", O_RDWR | O_APPEND);
	if (-1 == fd) {
		perror("open error");
		return;
	}
	char tmp;
	printf("please input something, end with a '*'\n");
	char str[20];
	while('*' != (tmp = getchar())) {
		str[0] = tmp;
		if (-1 == write(fd, str, 1)) {
			perror("write error");
			return;
		}
	}
	if (-1 == lseek(fd, 0, SEEK_SET)) {
		perror("lseek error");
		return;
	}

	int readsize;
	do {
		readsize = read(fd, str, 20);
		if (-1 == readsize) {
			perror("read error");
			return;
		}
		if (-1 == write(1, str, readsize)) {
			perror("write error");
			return;
		}
	} while(0 != readsize);
	close(fd);
}

void dupTest(){
	int newfd = dup(1);
	if (-1 == newfd) {
		perror("dup error");
		return;
	}
	if (-1 == write(newfd, "Hello World.", strlen("Hello World."))) {
		perror("write error");
		return;
	}
	if (-1 == close(newfd)) {
		perror("close error");
		return;
	}
}

void dup2Test(){
	if (-1 == dup2(1, 2)) {
		perror("dup2 error");
		return;
	}
	errorTest();
}

void fcntlTest1() {
	int newfd = fcntl(1, F_DUPFD, 0);
	if (-1 == newfd) {
		perror("dup error");
		return;
	}
	if (-1 == write(newfd, "Hello World.", strlen("Hello World."))) {
		perror("write error");
		return;
	}
	if (-1 == close(newfd)) {
		perror("close error");
		return;
	}
}

void fcntlTest2() {
	int newfd1 = fcntl(1, F_DUPFD, 10);
	if (-1 == newfd1) {
		perror("dup error");
		return;
	}
	int newfd2 = fcntl(1, F_DUPFD_CLOEXEC, 11);
	if (-1 == newfd2) {
		perror("dup error");
		return;
	}
	printf("newfd1: %d\n", newfd1);
	printf("newfd2: %d\n", newfd2);

	if (0 == fork()){
		execl("/tmp/a.out", NULL);
	}

	sleep(1);
	if (-1 == close(newfd1)) {
		perror("close error");
		return;
	}
	if (-1 == close(newfd2)) {
		perror("close error");
		return;
	}
}

void dupTest2(){
	int newfd = dup(1);
	if (-1 == newfd) {
		perror("dup error");
		return;
	}
	printf("newfd1: %d\n", newfd);
	if (0 == fork()){
		execl("/tmp/a.out", NULL);
	}
	if (-1 == close(newfd)) {
		perror("close error");
		return;
	}
}

void fcntlTestGETFD_SETFD(){
	int fd1 = open("/tmp/a.txt", O_RDONLY);
	if (-1 == fd1) {
		perror("open fd1 error");
		return;
	}
	int fd1Attr = fcntl(fd1, F_GETFD);
	if (-1 == fd1Attr) {
		perror("fcntl fd1 error");
	}
	if (FD_CLOEXEC == fd1Attr) {
		printf("fd1: FD_CLOEXEC set\n");
	}
	else {
		printf("fd1: FD_CLOEXEC not set\n");
	}

	int fd2 = open("/tmp/b.txt", O_RDONLY | O_CLOEXEC);
	if (-1 == fd2) {
		perror("open fd2 error");
		return;
	}
	int fd2Attr = fcntl(fd2, F_GETFD);
	if (-1 == fd2Attr) {
		perror("fcntl fd2 error");
	}
	if (FD_CLOEXEC ==fd2Attr) {
		printf("fd2: FD_CLOEXEC set\n");
	}
	else {
		printf("fd2: FD_CLOEXEC not set\n");
	}

	// set
	if (-1 == fcntl(fd1, F_SETFD, fd1Attr | FD_CLOEXEC)) {
		perror("fcntl fd1 F_SETFD error");
	}
	if (-1 == fcntl(fd2, F_SETFD, fd2Attr & ~FD_CLOEXEC)) {
		perror("fcntl fd2 F_SETFD error");
	}


	printf("After F_SETFD\n");

	fd1Attr = fcntl(fd1, F_GETFD);
	if (-1 == fd1Attr) {
		perror("fcntl fd1 error");
	}
	if (FD_CLOEXEC == fd1Attr) {
		printf("fd1: FD_CLOEXEC set\n");
	}
	else {
		printf("fd1: FD_CLOEXEC not set\n");
	}

	fd2Attr = fcntl(fd2, F_GETFD);
	if (-1 == fd2Attr) {
		perror("fcntl fd2 error");
	}
	if (FD_CLOEXEC ==fd2Attr) {
		printf("fd2: FD_CLOEXEC set\n");
	}
	else {
		printf("fd2: FD_CLOEXEC not set\n");
	}

	close(fd1);
	close(fd2);
}

void set_fl(int fd, int flags){
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");
	val |= flags;	// turn on flags
	if (fcntl(fd, F_SETFL, val) < 0)
		perror("fcntl F_SETFL error");
}

void clr_fl(int fd, int flags){
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");
	val &= ~flags;	// turn off flags
	if (fcntl(fd, F_SETFL, val) < 0)
		perror("fcntl F_SETFL error");
}

void fcntlTestGETFL_SETFL(){
	int fd = open("/tmp/a.txt", O_RDWR | O_APPEND);
	if (-1 == fd) {
		perror("open error");
	}
	// get file status
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");
	if (val & O_APPEND)
		printf("append\n");
	if (val & O_NONBLOCK)
		printf("nonblock\n");

	set_fl(fd, O_NONBLOCK);		// set O_NONBLOCK
	clr_fl(fd, O_APPEND);		// clear O_APPEND

	// get file status now
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");
	printf("after set O_NONBLOCK and clear O_APPEND\n");
	if (val & O_APPEND)
		printf("append\n");
	if (val & O_NONBLOCK)
		printf("nonblock\n");
}



