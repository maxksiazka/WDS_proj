#ifndef UDP_DISCOVERY_H_
#define UDP_DISCOVERY_H_
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"


typedef enum {
    STATE_IDLE,
    STATE_IP_DISCOVERED,
    STATE_TCP_CONNECTING,
    STATE_TCP_CONNECTED,
    STATE_DISCONNECTED
} app_state_t;

typedef struct {
    ip_addr_t remote_ip;
    app_state_t state;
    struct tcp_pcb *tpcb;
} g_connection_mgr_t;
extern g_connection_mgr_t g_connection_mgr;
void udp_discovery_init(void);

void udp_discovery_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port);


#endif // UDP_DISCOVERY_H_
