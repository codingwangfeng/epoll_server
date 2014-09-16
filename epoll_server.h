/**
  *  Author : WANG FENG
  *    File : epoll_server.h
  *    Date : 2014年 09月 16日 星期二 16:00:02 CST
  *    Desc :
  * History :1.created on 2014年 09月 16日 星期二 16:00:02 CST by WANG FENG
  */
#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>

int create_epoll_fd(const char* port);
int set_fd_noblocking(int fd);
int start_server();

#endif    // #ifndef __EPOLL_SERVER_H__

