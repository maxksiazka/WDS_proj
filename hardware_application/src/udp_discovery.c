#include "udp_discovery.h"
#include "common.h"
#include "lwip/ip4_addr.h"
#include "lwip/udp.h"
#define UNUSED __attribute__((unused))

/**
 * @brief LWIP UDP receive callback function for handling incoming discovery
 * messages
 *
 * This function is called by the LWIP stack when a UDP packet is received on
 * the bound port (12345). It processes the incoming packet to check if it
 * contains a valid discovery message. If a valid discovery message is received
 * and the application is in the idle state, it updates the connection manager
 * with the discovered IP address and transitions the state to indicate that an
 * IP has been discovered.
 *
 * @param[in] arg -- User-defined argument (not used in this implementation, can
 * be set to NULL)
 * @param[in] pcb -- Pointer to the UDP protocol control block that received the
 * packet
 * @param[in] p -- Pointer to the pbuf structure containing the received packet
 * data
 * @param[in] addr -- Pointer to the IP address of the sender of the packet
 * @param[in] port -- Port number from which the packet was sent
 *
 * @important The function should follow the LWIP UDP receive callback signature
 * and is expected to be registered using udp_recv() during the initialization
 * of the UDP discovery module. It is marked as static since it is only intended
 * to be used within this source file.
 *
 */
static void udp_discovery_recv_callback(void* arg, struct udp_pcb* pcb,
                                        struct pbuf* p, const ip_addr_t* addr,
                                        uint16_t port);

g_connection_mgr_t g_connection_mgr = {
    .state = STATE_IDLE,
};
void udp_discovery_init(void) {
    struct udp_pcb* pcb = udp_new();
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

static void udp_discovery_recv_callback(UNUSED void* arg,
                                        UNUSED struct udp_pcb* pcb,
                                        struct pbuf* p, const ip_addr_t* addr,
                                        u16_t port) {
    print_debug("UDP discovery message received from %s:%d\n",
                ipaddr_ntoa(addr), port);
    if (p == NULL) {
        return;
    }
    if (g_connection_mgr.state != STATE_IDLE) {
        pbuf_free(p);
        return;
    }
    if (strncmp((char*)p->payload, "DISCOVER", 7) != 0) {
        print_debug("Received invalid discovery message\n");
        pbuf_free(p);
        return;
    }
    ip4_addr_copy(g_connection_mgr.remote_ip, *addr);
    g_connection_mgr.state = STATE_IP_DISCOVERED;
    print_debug("Discovered IP: %s\n", ipaddr_ntoa(addr));
    pbuf_free(p);
}
