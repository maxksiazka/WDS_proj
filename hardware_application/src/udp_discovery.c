#include "common.h"
#include "udp_discovery.h"
#include "pico/stdlib.h"
#include "lwip/udp.h"
#include "lwip/ip4_addr.h"
g_connection_mgr_t g_connection_mgr = {
    .state = STATE_IDLE,
};
void udp_discovery_init(void) {
    // 
    struct udp_pcb *pcb = udp_new();
    if (pcb == NULL) {
        print_debug("Failed to create UDP PCB\n");
        return;
    }
    if (udp_bind(pcb, IP_ADDR_ANY, 12345) != ERR_OK) {
        print_debug("Failed to bind UDP PCB\n");
        udp_remove(pcb);
        return;
    }
    udp_recv(pcb, udp_discovery_recv_callback, NULL);
}

void udp_discovery_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if (p == NULL) {
        return;
    }
    if (g_connection_mgr.state != STATE_IDLE) {
        pbuf_free(p);
        return;
    }
    ip4_addr_copy(g_connection_mgr.remote_ip, *addr);
    g_connection_mgr.state = STATE_IP_DISCOVERED;
    pbuf_free(p);
}
