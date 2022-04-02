#ifndef __DBI_OVER_TCP_H__
#define __DBI_OVER_TCP_H__

struct dbi_tcp_config
{
    int port;
    volatile int *cancel;
};

/* dbi_over_tcp main thread */
int *libtcp_dbi_thread(struct dbi_tcp_config *conf);

#endif
