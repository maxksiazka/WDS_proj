#ifndef TCP_API_IMPLEMENTATION_H
#define TCP_API_IMPLEMENTATION_H

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/pbuf.h"
#include "tcp_typedefs.h"

bool init_wifi_connection(const char *ssid, const char* password);

TCP_CLIENT_T *tcp_client_init(void);

bool tcp_client_open_connection(TCP_CLIENT_T *client);

//templates from lwip's website
// callback function prototypes


err_t tcp_receive_callback(void *arg, struct tcp_pcb* client_pcb, struct pbuf *p, err_t err);
err_t tcp_sent_callback(void *arg, struct tcp_pcb* client_pcb, u_int16_t length);
void tcp_error_callback(void *arg, err_t err);
err_t tcp_connected_callback(void *arg, struct tcp_pcb* client_pcb,err_t err);

/**
 * \brief A function used to terminate and free a TCP client connection instance
 *
 * \param[in] arg client instance pointer (should be TCP_CLIENT_T_*) (WILL BE FREED)
 * \return ERR_OK on success, ERR_ABRT if aborted, ERR_VAL if invalid argument
 */
err_t tcp_client_close(void *arg);


#endif // TCP_API_IMPLEMENTATION_H
