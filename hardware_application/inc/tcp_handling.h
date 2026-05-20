#ifndef TCP_HANDLING_H_
#define TCP_HANDLING_H_
#include "tcp_typedefs.h"
#include <lwip/tcp.h>

// mapping enum to string representations
// https://www.linkedin.com/pulse/mapping-enum-string-c-language-sathishkumar-duraisamy

/**
 * \brief A function used to echo the data currently in buffer back to the
 * sender
 *
 * \param[in,out] arg Data pointer to tcp_client (should be TCP_CLIENT_T_*)
 * \param[in,out] client_pcb TCP protocol control block, should be the one
 * inside arg
 * \param[in] err Current error status
 * \return ERR_OK on success,
 */
err_t tcp_echo_commmand(void* arg, struct tcp_pcb* client_pcb, err_t err);

// TODO
err_t tcp_handle_message(void* arg, struct tcp_pcb* client_pcb, err_t err);

err_t tcp_send_message(void * arg, data_packet_t* packet);

/**
 * @brief CRC16 checksum calculation function.
 *
 * This function calculates the CRC16 checksum for the given data packet. It
 * processes all fields of the packet except for the checksum itself, using the
 * CRC-16-CCITT polynomial (0x1021). The resulting checksum is returned as a
 * 16-bit unsigned integer.
 *
 * @param[in] packet -- Pointer to the data packet for which the checksum is to
 * be calculated
 * @param[in] initial_value -- Initial value for the checksum calculation (can
 * be used for chaining checksums)
 * @retval uint16_t -- Calculated CRC16 checksum for the given IMU data packet
 */
uint16_t calculate_crc_ccitt(const data_packet_t* data, uint16_t initial_value);
#endif // TCP_HANDLING_H_
