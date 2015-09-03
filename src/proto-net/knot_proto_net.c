/*
 * Copyright (c) 2015, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#include <knot_proto_net.h>
#include <abstract_driver.h>

/**
 * struct knot_socket - socket structure
 * @fd: socket file descriptor
 * @phy: physical layer used for this socket
 * @type: socket type
 * @my_addr: socket local address
 *
 * This struct defines the socket parameters.
 */
typedef struct __attribute__ ((packed)) {
	int fd;
	knot_socket_type_t type;
	uint8_t my_addr;
	uint8_t to_addr;
} knot_socket;

static knot_socket sockets[KNOT_SOCKET_FD_MAX];
static abstract_driver *driver = &driver_empty; // defined in abstract_driver.h

/**
 * is_phy_supported() - verifies if the phy is supported by platform.
 */
static uint8_t is_phy_supported(knot_phy_t phy)
{
	uint8_t ret = 0;

	if (phy >= 0 && phy < KNOT_PHY_MAX) {
		ret = drivers[phy]->valid;
	}
	return ret;
}

/**
 * is_type_supported() - verifies if the phy is supported by platform.
 */
static uint8_t is_type_supported(knot_socket_type_t type)
{
	uint8_t ret = 0;

	if (type >= 0 && type < KNOT_SOCKET_TYPE_MAX) {
		ret = 1;
	}
	return ret;
}

/**
 * allocate_socket() - allocates next free socket in the socket list.
 */
static int allocate_socket()
{
	int i = 0;

	for (i = 0; i < KNOT_SOCKET_FD_MAX; i++) {
		if (sockets[i].fd == KNOT_SOCKET_FD_INVALID) {
			break;
		}
	}

	if (i >= KNOT_SOCKET_FD_MAX) {
		i = KNOT_SOCKET_FD_INVALID;
	}

	return i;
}

/**
 * knot_net_socket() - creates a KNoT socket.
 */
int knot_net_socket(uint8_t my_addr, knot_socket_type_t type)
{
	int driver_fd = KNOT_SOCKET_FD_INVALID;
	int new_socket = KNOT_SOCKET_FD_INVALID;

	if (is_type_supported(type)) {
		new_socket = allocate_socket();

		if (new_socket != KNOT_SOCKET_FD_INVALID) {
			driver_fd = driver->socket();

			if (driver_fd >= 0) {
				sockets[new_socket].fd = driver_fd;
				sockets[new_socket].my_addr = KNOT_NET_ID_NONE;
				sockets[new_socket].to_addr = KNOT_NET_ID_NONE;
				sockets[new_socket].type = type;
			} else {
				// TODO: log error
				new_socket = KNOT_SOCKET_FD_INVALID;
			}

		} else {
			// TODO: log error
		}
	} else {
		// TODO: log error
	}

	return new_socket;
}

/**
 * knot_net_accept() - creates a new socket from server socket new connection.
 */
int knot_net_accept(int socket, uint8_t *from_addr)
{
	int new_socket = KNOT_SOCKET_FD_INVALID;
	int driver_fd = KNOT_SOCKET_FD_INVALID;

	if (sockets[socket].fd != KNOT_SOCKET_FD_INVALID &&
	    sockets[socket].type == KNOT_SOCKET_SERVER) {
		driver_fd = driver->accept(sockets[socket].fd);

		if (driver_fd >= 0) {
			new_socket = allocate_socket();

			if (new_socket != KNOT_SOCKET_FD_INVALID) {
				sockets[new_socket].fd = driver_fd;
				sockets[new_socket].my_addr = new_socket;
				sockets[new_socket].to_addr = sockets[socket].my_addr;
				sockets[new_socket].type = KNOT_SOCKET_CLIENT;
				*from_addr = (uint8_t) new_socket;
			} else {
				// TODO: send msg KNOT_NETMSG_JOIN_RESULT back
				driver->close(driver_fd);
			}
		} else {
			// TODO: log error
		}
	} else {
		// TODO: log error
	}

	return new_socket;
}

/**
 * knot_net_connect() - connects to a server socket.
 */
int knot_net_connect(int socket, uint8_t to_addr)
{
	int ret = KNOT_NET_ERROR;

	if (sockets[socket].fd != KNOT_SOCKET_FD_INVALID &&
	    sockets[socket].type == KNOT_SOCKET_CLIENT) {
		ret = driver->connect(sockets[socket].fd, to_addr);

		if (ret == KNOT_NET_SUCCESS) {
			sockets[socket].to_addr = to_addr;
		} else {
			// TODO: log error
		}
	} else {
		// TODO: log error
	}

	return ret;

}

/**
 * knot_net_available() - verifies if there is any incoming message on socket.
 */
int knot_net_available(int socket)
{
	return driver->available(sockets[socket].fd);
}

/**
 * knot_net_recv() - receives a message from other node.
 */
int knot_net_recv(int socket, uint8_t *from_addr, knot_net_msg *msg)
{
// TODO: implement receive function
}

/**
 * knot_net_send() - send a message to other node.
 */
int knot_net_send(int socket, uint8_t to_addr, knot_net_msg *msg)
{
// TODO: implement send function
}

/**
 * knot_net_init() - initializes the KNoT network layer.
 */
void knot_net_init(knot_phy_t phy)
{
	int ret = KNOT_NET_ERROR;
	int i = 0;

	for (i = 0; i < KNOT_SOCKET_FD_MAX; i++) {
		sockets[i].fd = KNOT_SOCKET_FD_INVALID;
	}

	if (is_phy_supported(phy)) {
		driver = drivers[phy]; // defined in abstract_driver.h
		ret = driver->probe();
	}

	return ret;
}
