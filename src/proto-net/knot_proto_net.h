/*
 * Copyright (c) 2015, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#ifndef KNOT_PROTOCOL_NET_H
#define KNOT_PROTOCOL_NET_H

// KNoT network protocol version
#define KNOT_NET_VERSION_MAJOR		01
#define KNOT_NET_VERSION_MINOR		00

// invalid socket fd
#define KNOT_SOCKET_FD_INVALID		-1

// net layer return codes
#define KNOT_NET_ERROR			-1
#define KNOT_NET_SUCCESS		0

// max number of sockets fd for the network layer
#ifdef ARDUINO
// If we are on Arduino, we will have only one socket
#define KNOT_SOCKET_FD_MAX		1
#include <Arduino.h>
#else
// If we are on a Gateway, we will have 255 sockets
#define KNOT_SOCKET_FD_MAX		255
#endif

// The KNoT net layer addressing schema is a socket
// abstraction for a small local network. It has 8-bit addresses
#define KNOT_NET_ID_NONE		0x00 // Node that doens't have ID
#define KNOT_NET_ID_MAX			0xF0 // max 240 device nodes
#define KNOT_NET_ID_GATEWAY		0xF1 // first gateway ID
#define KNOT_NET_ID_GATEWAY_MAX		0xFE // max number of gateways (14)
					     // This means 3360 (14*240)
					     // devices in one physical space
#define KNOT_NET_ID_BROADCAST		0xFF // Broadcast address

// Network retransmiting parameters
#define KNOT_NET_TIMEOUT		100
#define KNOT_NET_RETRIES		5

// Local network messages
#define KNOT_NETMSG_INVALID		0x00
#define KNOT_NETMSG_JOIN_LOCAL		0x01
#define KNOT_NETMSG_UNJOIN_LOCAL	0x02
#define KNOT_NETMSG_JOIN_GATEWAY	0x03
#define KNOT_NETMSG_JOIN_RESULT		0x04
#define KNOT_NETMSG_SET_ADDRESS		0x05
#define KNOT_NETMSG_APPMSG		0x06

/**
 * struct knot_net_header - net layer message header
 * @msg_type: message type
 * @to: the outgoing address
 * @from: the incoming address
 * @payload_len: payload length (in bytes)
 *
 * This struct defines the network layer message header
 */
typedef struct __attribute__ ((packed)) {
	uint8_t			msg_type;
	uint8_t			to;
	uint8_t			from;
	uint8_t			payload_len;
} knot_net_header;

// Network message size parameters
#define KNOT_NET_MSG_SIZE		128
#define KNOT_NET_PAYLOAD_SIZE		(KNOT_NET_MSG_SIZE \
					-sizeof(knot_net_header))
/**
 * struct knot_net_join_local - net layer join local message
 * @header_padding: padding for the header
 * @net_maj_version: protocol version, major number
 * @net_min_version: protocol version, minor number
 * @result: result for the join process
 *
 * This struct defines the net layer join local message.
 */
typedef struct __attribute__ ((packed)) {
	knot_net_header		header_padding;
	uint8_t			net_maj_version;
	uint8_t			net_min_version;
	uint8_t			result;
} knot_net_join_local;

/**
 * struct knot_net_msg_app - net layer application message
 * @header_padding: padding for the header
 * @data: application layer message
 *
 * This struct defines the network layer message that encapsulates an
 * application layer message.
 */
typedef struct __attribute__ ((packed)) {
	knot_net_header		header_padding;
	uint8_t			data[KNOT_NET_PAYLOAD_SIZE];
} knot_net_msg_app;

/**
 * union knot_net_msg - defines a network layer message
 * @header: net layer message header
 * @payload: application layer message
 * @join: net layer join local message
 * @buffer: message represented as a byte array
 *
 * This union defines the network layer message.
 */
typedef union {
	knot_net_header		header;
	knot_net_msg_app	payload;
	knot_net_join_local	join;
	uint8_t			buffer[KNOT_NET_MSG_SIZE];
} knot_net_msg;

/**
 * enum knot_phy_t - defines the physical layer data streams
 * @KNOT_PHY_ETH: ethernet data stream
 * @KNOT_PHY_NRF24: NRF24L01 radio data stream
 * @KNOT_PHY_NRF51: NRF51 BT radio data stream
 * @KNOT_PHY_NRF905: NRF905 radio data stream
 * @KNOT_PHY_RFM69: RFM69 radio data stream
 * @KNOT_PHY_ESP8266: ESP8266 WiFi radio data stream
 * @KNOT_PHY_ASK: ASK radio data stream
 * @KNOT_PHY_SERIAL: serial data stream
 *
 * This enum defines the physical layer data streams supported
 * by the KNoT platform.
 */
typedef enum {
	KNOT_PHY_SERIAL = 0,
	KNOT_PHY_NRF24,
	KNOT_PHY_NRF51,
	KNOT_PHY_NRF905,
	KNOT_PHY_RFM69,
	KNOT_PHY_ETH,
	KNOT_PHY_ESP8266,
	KNOT_PHY_ASK,
	KNOT_PHY_MAX
} knot_phy_t;

/**
 * enum knot_socket_type_t - defines the socket type (server/client)
 * @KNOT_SOCKET_CLIENT: client socket
 * @KNOT_SOCKET_SERVER: server socket
 *
 * This enum defines the socket types supported by the KNoT platform.
 */
typedef enum {
	KNOT_SOCKET_CLIENT = 0,
	KNOT_SOCKET_SERVER,
	KNOT_SOCKET_TYPE_MAX
} knot_socket_type_t;

/**
 * knot_net_socket() - creates a KNoT socket.
 * @my_addr:	the KNoT address that the socket will bound to.
 * @type:	the type of the socket (client or server).
 *
 * This function creates a KNoT socket using the physical layer driver
 * specified as parameter. It also sets the startung addres for the socket
 * and performs the necessary actions to the socket act as a client socket
 * or a server socket (for the server it also performs the bind and listen
 * actions).
 *
 * Return: the file descriptor for the socket created or -1 if any error.
 */
int knot_net_socket(uint8_t my_addr, knot_socket_type_t type);

/**
 * knot_net_accept() - creates a new socket from server socket new connection.
 * @socket:	server socket file descriptor.
 * @from_addr:	the address from the incoming socket.
 *
 * This function creates a KNoT client socket from the pending sockets queue
 * of a server socket.
 *
 * Return: the file descriptor for the socket created or -1 if no pending
 * connections.
 */
int knot_net_accept(int socket, uint8_t *from_addr);

/**
 * knot_net_connect() - connects to a server socket.
 * @socket:	client socket file descriptor.
 * @to_addr:	the address of the server socket.
 *
 * This function connects a KNoT client socket to a server socket.
 *
 * Return: 0 if success or -1 if any error.
 */
int knot_net_connect(int socket, uint8_t to_addr);

/**
 * knot_net_available() - verifies if there is any incoming message on socket.
 * @socket:	socket file descriptor.
 *
 * This function verifies if a socket have any incoming pending message.
 *
 * Return: number of incomming pending messages for this socket.
 */
int knot_net_available(int socket);

/**
 * knot_net_recv() - receives a message from other node.
 * @socket:	socket file descriptor.
 * @from_addr:	field filled with the address of the incoming node.
 * @msg:	received message.
 *
 * This function receives a message from other node in the same local network.
 *
 * Return: number of bytes received.
 */
int knot_net_recv(int socket, uint8_t *from_addr, knot_net_msg *msg);

/**
 * knot_net_send() - send a message to other node.
 * @socket:	socket file descriptor.
 * @to_addr:	the address of the outgoing node.
 * @msg:	message to be sent.
 *
 * This function sends a message to other node in the same local network.
 *
 * Return: number of bytes sent.
 */
int knot_net_send(int socket, uint8_t to_addr, knot_net_msg *msg);

/**
 * knot_net_init() - initializes the KNoT network layer.
 * @phy:	physical layer (driver) to be used by the network layer.
 *
 * This function initializes the KNoT network layer, setting up the
 * desired physical layer driver.
 * It must be called in the beginning of the program.
 */
void knot_net_init(knot_phy_t phy);

#endif //KNOT_PROTOCOL_NET_H