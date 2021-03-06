#ifndef __CONN_POOL_H_
#define __CONN_POOL_H_

#include <stdint.h>
#include <sys/time.h>
#include "my_pool.h"
#include "my_buf.h"

enum{
    STATE_UNAVAIL = 0,
    STATE_AUTH_FAIL,
    STATE_AUTH_SUCCESS,
    STATE_READING_CLIENT,
    STATE_PREPARE_MYSQL,
    STATE_WRITING_MYSQL,
    STATE_READ_MYSQL_WRITE_CLIENT,
    STATE_IDLE
};

/**********************************************

	            /----------------------------+
	  conn_t  <--------------------------+   |
	 +------+                            |   |
	 |  my  |   --->  [my_conn_t].conn --+   |
	 +------+                                |
	 |  cli |   --->  [cli_conn_t].conn -----+
	 +------+
	 |  ... |
	 +------+

**********************************************/
typedef struct {
    uint32_t connid;
    my_conn_t *my;      // 对应的mysql连接是哪个 
    void *cli;          // 对应这个连接结构的客户端连接 
    buf_t buf;          // 缓存
    int state;          // 状态
    time_t state_time;  // 状态更改的时间
    char curdb[64];
    uint8_t comno;
    char arg[1024];
    struct timeval tv_start;
    struct timeval tv_end;
    struct list_head link;
} conn_t;

int conn_pool_init(size_t count);
conn_t *conn_open(int fd, uint32_t ip, uint16_t port);
int conn_close(conn_t *c);
int conn_close_with_my(conn_t *c);
int conn_alloc_my_conn(conn_t *c);

int conn_state_set_reading_client(conn_t *c);
int conn_state_set_writing_mysql(conn_t *c);
int conn_state_set_read_mysql_write_client(conn_t *c);
int conn_state_set_prepare_mysql(conn_t *c);
int conn_state_set_idle(conn_t *c);
int conn_state_set_auth_fail(conn_t *c);
int conn_state_set_auth_success(conn_t *c);

int conn_pool_destroy( ) ;

#endif
