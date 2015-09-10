/*
 * Copyright (c) 2015, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#ifndef KNOT_PROTOCOL_APP_H
#define KNOT_PROTOCOL_APP_H

#include <knot_types.h>
#include <knot_proto_net.h>

// Each KNoT Device or user has a unique ID and token as identification
// mechanism
#define KNOT_PROTOCOL_TOKEN_LEN			40
#define KNOT_PROTOCOL_UUID_LEN			36
// A human readable name for each device
#define KNOT_PROTOCOL_DEVICE_NAME_LEN		64
// A human readable name for each data source/sink in the device
#define KNOT_PROTOCOL_DATA_NAME_LEN		64

#define KNOT_PROTOCOL_DATA_ID_MAX		0xFE
#define KNOT_PROTOCOL_DATA_ID_NA		0xFF

#define KNOT_MSG_INVALID			0x00
// KNoT connect/register messages
#define KNOT_MSG_REGISTER_REQ			0x10
#define KNOT_MSG_REGISTER_RESP			0x11
#define KNOT_MSG_UNREGISTER_REQ			0x12
#define KNOT_MSG_UNREGISTER_RESP		0x13
#define KNOT_MSG_AUTH_REQ			0x14
#define KNOT_MSG_AUTH_RESP			0x15
// KNoT response messages (from device)
#define KNOT_MSG_POST_DATA			0x20
#define KNOT_MSG_POST_INTERVAL			0x21
#define KNOT_MSG_POST_COMMAND			0x22
// KNoT request messages (from gateway)
#define KNOT_MSG_GET_DATA			0x30
#define KNOT_MSG_SET_DATA			0x31
#define KNOT_MSG_GET_COMMAND			0x32
#define KNOT_MSG_SET_COMMAND			0x33
// KNoT device config messages
#define KNOT_MSG_POST_SCHEMA_START		0x40
#define KNOT_MSG_POST_SCHEMA			0x41
#define KNOT_MSG_POST_SCHEMA_END		0x42
// KNoT data sending config messages
#define KNOT_MSG_CFG_GET_INTERVAL		0x50
#define KNOT_MSG_CFG_SET_INTERVAL		0x51
#define KNOT_MSG_CFG_GET_THRESHOLD		0x52
#define KNOT_MSG_CFG_SET_THRESHOLD		0x53


typedef struct __attribute__ ((packed)) {
	uint8_t			msg_type;
	uint8_t			payload_len;
} knot_msg_header;

#define KNOT_MSG_MAX_SIZE	KNOT_NET_PAYLOAD_SIZE

typedef struct __attribute__ ((packed)) {
	uint8_t			data_id;
	uint8_t			unit;
	int16_t			multiplier;
	int16_t			value_int;
	int16_t			value_dec;
} knot_data_float;

typedef struct __attribute__ ((packed)) {
	uint8_t			data_id;
	uint8_t			unit;
	int16_t			multiplier;
	int32_t			value;
} knot_data_int;

typedef struct __attribute__ ((packed)) {
	uint8_t			data_id;
	uint8_t			unit;
	int16_t			multiplier;
	uint16_t		padding1;
	uint8_t			padding2;
	uint8_t			value;
} knot_data_bool;

// raw message len: 7 bytes
#define KNOT_MAX_RAW_LEN	(sizeof(knot_data_int)-sizeof(uint8_t))

typedef struct __attribute__ ((packed)) {
	uint8_t			data_id;
	uint8_t			value[KNOT_MAX_RAW_LEN];
} knot_data_raw;

typedef union {
	uint8_t			data_id;
	knot_data_float		_float;
	knot_data_int		_int;
	knot_data_bool		_bool;
	knot_data_raw		_raw;
} knot_data;

#define KNOT_MSG_MAX_NUM_DATA	((KNOT_MSG_MAX_SIZE-sizeof(knot_msg_header)) \
				/sizeof(knot_data)) // currently 10

typedef struct __attribute__ ((packed)) {
	knot_msg_header		header_padding;
	knot_data		data[KNOT_MSG_MAX_NUM_DATA];
} knot_msg_data;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		header_padding;
	uint8_t			result;
	uint8_t			uuid[KNOT_PROTOCOL_UUID_LEN];
	uint8_t			token[KNOT_PROTOCOL_TOKEN_LEN];
} knot_msg_credential;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		header_padding;
	char			devName[KNOT_PROTOCOL_DEVICE_NAME_LEN];
} knot_msg_register;

typedef struct __attribute__ ((packed)) {
	uint16_t		data_type;
	uint8_t			data_id;
	char 			name[KNOT_PROTOCOL_DATA_NAME_LEN];
} knot_schema;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		header_padding;
	knot_schema		schema;
} knot_msg_config;

typedef union {
	knot_msg_header		header;
	knot_msg_data		data;
	knot_msg_credential	cred;
	knot_msg_register	reg;
	knot_msg_config		config;
	uint8_t			buffer[KNOT_MSG_MAX_SIZE];
} knot_msg;


#endif //KNOT_PROTOCOL_APP_H