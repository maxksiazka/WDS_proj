#ifndef UDP_DISCOVERY_H_
#define UDP_DISCOVERY_H_
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

/**
 * @brief Enumeration of the different states of the application
 *
 * This enumeration defines the various states that the application can be in
 * during its lifecycle. The states include:
 * - STATE_IDLE: The initial state where the application is waiting for an IP
 * address to be discovered.
 * - STATE_IP_DISCOVERED: The state indicating that an IP address has been
 * successfully discovered.
 * - STATE_TCP_CONNECTING: The state where the application is attempting to
 * establish a TCP connection with the discovered IP address.
 * - STATE_TCP_CONNECTED: The state indicating that a TCP connection has been
 * successfully established.
 * - STATE_DISCONNECTED: The state indicating that the TCP connection has been
 * lost or closed.
 *
 * These states are used to manage the flow of the application and ensure that
 * it responds appropriately to different events, such as discovering an IP
 * address or losing a TCP connection.
 * TODO: Consider moving this enum to a more general header file if it is used
 * across multiple modules.
 */
typedef enum {
    STATE_IDLE,
    STATE_IP_DISCOVERED,
    STATE_TCP_CONNECTING,
    STATE_TCP_CONNECTED,
    STATE_DISCONNECTED
} app_state_t;

/**
 * @brief Structure to manage the connection state and information
 *
 * This structure holds the relevant information for managing the connection
 * state of the application. It includes:
 * - remote_ip: The IP address of the remote device that has been discovered.
 * - state: The current state of the application (see @link app_state_t
 * @endlink).
 * - tpcb: A pointer to the TCP protocol control block.
 */
typedef struct {
    ip_addr_t remote_ip;
    app_state_t state;
    struct tcp_pcb* tpcb;
} g_connection_mgr_t;
extern g_connection_mgr_t g_connection_mgr;
/**
 * @brief Initialization of the UDP discovery module
 *
 * This function creates a new UDP protocol control block, binds it to any
 * available IP address and a specific port (12345), and sets up a callback
 * function to handle incoming UDP packets.
 */
void udp_discovery_init(void);


#endif // UDP_DISCOVERY_H_
