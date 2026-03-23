#ifndef TCP_API_IMPLEMENTATION_H
#define TCP_API_IMPLEMENTATION_H

#include "tcp_typedefs.h"

/**
 * @brief A function used to initialize the Wi-Fi connection to the network
 *
 * @param[in] ssid Wi-Fi network SSID to connect to
 * @param[in] password Wi-Fi network password to connect to
 * @return true if Wi-Fi connection is successfully established, false otherwise
 */
bool init_wifi_connection(const char *ssid, const char* password);

/**
 * @brief A function used to initialize a TCP client connection instance
 *
 * @return TCP_CLIENT_T* pointer to the initialized client instance, NULL if initialization failed
 */
TCP_CLIENT_T *tcp_client_init(void);

/**
 * @brief A function used to establish a TCP client connection to the server
 *
 * @param[in] client a pointer to the TCP client instance to be connected (should be TCP_CLIENT_T_*)
 * @return true if connection is successfully established, false otherwise
 */
bool tcp_client_open_connection(TCP_CLIENT_T *client);



/**
 * \brief A function used to terminate and free a TCP client connection instance
 *
 * \param[in] arg client instance pointer (should be TCP_CLIENT_T_*) (WILL BE FREED)
 * \return ERR_OK on success, ERR_ABRT if aborted, ERR_VAL if invalid argument
 */
err_t tcp_client_close(void *arg);


#endif // TCP_API_IMPLEMENTATION_H
