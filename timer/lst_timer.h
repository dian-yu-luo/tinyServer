#ifndef LST_TIMER
#define LST_TIMER

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <time.h>
#include "../log/log.h"

class util_timer;

struct client_data
{
    sockaddr_in address;
    int sockfd;
    util_timer *timer;
};

class util_timer
{
    // 这就是个节点,一串上面的一个
public:
    util_timer() : prev(NULL), next(NULL) {}

public:
    time_t expire;
    /* 
    这个函数的设计就很有意思,我也不知道在我的未来事件到了的时候发生什么事件,
    那我干脆自己定义回调函数,由客户端的代码直接完成回调事件的编写,把逻辑任务留给未来完成
     */
    void (* cb_func)(client_data *);
    client_data *user_data;
    util_timer *prev;
    util_timer *next;
};

class sort_timer_lst
{
    // 这个东西的设计,用来处理用户的,一系列的时钟事件
public:
    sort_timer_lst();
    ~sort_timer_lst();

    void add_timer(util_timer *timer);
    void adjust_timer(util_timer *timer);
    void del_timer(util_timer *timer);
    void tick();

private:
    void add_timer(util_timer *timer, util_timer *lst_head);

    util_timer *head;
    util_timer *tail;
};

class Utils
{

    // 工具,定时器的作用
    // 里面直接包含了一种自定义是数据结构,这个结构保存了一个链表来进行一系列事件状态的保存
public:
    Utils() {}
    ~Utils() {}

    void init(int timeslot);

    //对文件描述符设置非阻塞
    int setnonblocking(int fd);

    //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);

    //信号处理函数
    static void sig_handler(int sig);

    //设置信号函数
    void addsig(int sig, void(handler)(int), bool restart = true);

    //定时处理任务，重新定时以不断触发SIGALRM信号
    void timer_handler();

    void show_error(int connfd, const char *info);

public:
    static int *u_pipefd;
    sort_timer_lst m_timer_lst;
    static int u_epollfd;
    int m_TIMESLOT;
};

void cb_func(client_data *user_data);

#endif
