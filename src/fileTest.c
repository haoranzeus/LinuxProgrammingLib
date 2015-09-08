/*
 * fileTest.c
 *
 *  Created on: Sep 7, 2015
 *      Author: haoran
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

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



