//
//  Created by stevin on 2014-09-15
//  Copyright (c) 2014年 stevin. All rights reserved.
//
#include "epoll_server.h"
#define MAXEVENTS 64
int create_epoll_fd(const char* port){
    int retcode, sfd;
    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    retcode = getaddrinfo(NULL, port, &hints, &result);
    if(result != 0){
        fprintf(stderr, "getaddrinfo fail, error:%s", gai_strerror(retcode));
        return -1;
    }
    for(rp = result; rp != NULL; rp = rp->ai_next){
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1){
            continue;
        }
        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0){
            break;
        }
        close(sfd);
    }
    if(rp == NULL){
        fprintf (stderr, "Could not bind\n");
        return -1;
    }
    freeaddrinfo(result);
    return sfd;
}

int set_fd_noblocking(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1){
        fprintf(stderr, "fcntl get error\n");
        return -1;
    }
    if(fcntl(fd, F_SETFL, flags|O_NONBLOCK) == -1){
        fprintf(stderr, "fcntl set error\n");
        return -1;
    }
    return 0;
}

int start_server(const char* port){    
    struct epoll_event event, *events;
    int sfd, efd;
    sfd = create_epoll_fd(port);
    if(sfd == -1){
        abort();
    }
    if( set_fd_noblocking(sfd) == -1){
        abort();
    }
    if( listen(sfd, SOMAXCONN) == -1){
        fprintf(stderr, "listen error\n");
        abort();
    }
    efd = epoll_create(1024);
    if( efd == -1 ){
        fprintf(stderr, "epoll_create1 error\n");
        abort();
    }
    event.data.fd = sfd;
    event.events = (uint32_t)(EPOLLIN|EPOLLET);
    if( epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event ) == -1 ){
        fprintf(stderr, "epoll_ctl error\n");
        abort();
    }

    events = (struct epoll_event*) malloc(MAXEVENTS*sizeof(struct epoll_event));
    while(1){
        int i, n;
        n = epoll_wait(efd, events, MAXEVENTS, 10);
        for(i = 0; i < n; i++){
            struct epoll_event e = events[i];
            uint32_t es = e.events;
            if(es & EPOLLERR || es & EPOLLHUP || !(es & EPOLLIN)){
                fprintf(stderr, "epoll error\n");
                close(e.data.fd);
                continue;
            }
            if(sfd == e.data.fd){
                while(1){
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    char host_name[NI_MAXHOST], port_name[NI_MAXSERV];
                    int infd = accept(sfd, &in_addr, &in_len);
                    if( infd == -1 ){
                        if( !(errno == EAGAIN || errno == EWOULDBLOCK)){
                            fprintf(stderr, "accept error\n");
                        }
                        break;
                    }
                    if( getnameinfo(&in_addr, in_len, 
                                host_name, (size_t)sizeof(host_name), 
                                port_name, sizeof port_name,
                                (int)(NI_NUMERICHOST | NI_NUMERICSERV)) == 0 ){
                        fprintf(stdout, "fd[%d], host[%s], port[%s]\n",
                                infd, host_name,port_name);
                    }
                    if( set_fd_noblocking(infd) == -1){
                        abort();
                    }
                    event.data.fd = infd;
                    event.events = (uint32_t)(EPOLLIN | EPOLLET);
                    if( epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event ) == -1 ){
                        fprintf(stderr, "epoll_ctl error\n");
                        abort();
                    }
                }
                continue;
            }else{
                fprintf(stdout, "fd[%d] say:\n", e.data.fd);
                int byebye = 0;
                while(1){
                    size_t count;
                    char buf[512];
                    count = read(e.data.fd, buf, sizeof buf);
                    if( count == -1 ){
                        if( errno != EAGAIN ){
                            perror("read");
                            byebye = 1;
                        }
                        break;
                    }else if(count == 0){
                        byebye = 1;
                        break;
                    }
                    if( write(1, buf, count) == -1 ){
                        perror("write");
                        abort();
                    }
                }
                if(byebye){
                    fprintf(stdout, "fd[%d] closed\n",e.data.fd);
                    close(e.data.fd);
                }
            }
        }
    }
    free(events);
    close(sfd);
}

