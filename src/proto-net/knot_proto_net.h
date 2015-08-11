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

// The KNoT ID addressing schema is a socket abstraction for a local network 
#define KNOT_NET_ID_NONE		0x00 // Node that doens't have ID
#define KNOT_NET_ID_MAX			0xF0 // max 240 device nodes
#define KNOT_NET_ID_GATEWAY1		0xF1 // first gateway ID
#define KNOT_NET_ID_GATEWAY13		0xFD // last gateway ID
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

typedef struct __attribute__ ((packed)) {
	uint8_t			msg_type;
	uint8_t			to;
	uint8_t			from;
	uint8_t			payload_len;
} knot_net_header;

// Network message size parameters
#define KNOT_NET_MSG_SIZE		128  // MTU 128 bytes
#define KNOT_NET_PAYLOAD_SIZE		(KNOT_NET_MSG_SIZE \\
					-sizeof(KNOT_NET_HEADER))
// Network messages definition
typedef struct __attribute__ ((packed)) {
	knot_net_header		header_padding;
	uint8_t			net_maj_version;
	uint8_t			net_min_version;
	uint8_t			result;
} knot_net_join_local;

typedef struct __attribute__ ((packed)) {
	knot_net_header		header_padding;
	uint8_t			data[KNOT_NET_PAYLOAD_SIZE];
} knot_net_msg_app;

typedef union {
	knot_net_header		header;
	knot_net_msg_app	payload;
	knot_net_join_local	join;
	uint8_t			buffer[KNOT_NET_MSG_SIZE];
} knot_net_msg;


// TODO: define network structure
// TODO: define driver structure and phy layer integration
// TODO: define network layer API

#endif //KNOT_PROTOCOL_NET_H