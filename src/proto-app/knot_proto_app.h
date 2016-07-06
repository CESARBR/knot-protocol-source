/*
 * Copyright (c) 2015, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */
#include <stdint.h>

#ifndef KNOT_PROTOCOL_APP_H
#define KNOT_PROTOCOL_APP_H

#define KNOT_NO_DATA			2
#define KNOT_DONE		1
#define KNOT_SUCCESS				0
#define KNOT_ERROR_UNKNOWN -1
#define KNOT_INVALID_DEVICE		-2
#define KNOT_INVALID_DATA	-3
#define KNOT_INVALID_DATA_RAW	-4
#define KNOT_DEVICE_NOT_FOUND		-5
#define KNOT_GW_FAILURE		-6
#define KNOT_CLOUD_FAILURE		-7
#define KNOT_CLOUD_OFFLINE	-8
#define KNOT_INVALID_UUID				-9
#define KNOT_INVALID_UUID_CLOUD	-10
#define KNOT_REGISTER_INVALID_DEVICENAME	-11
#define KNOT_INVALID_SCHEMA -12
#define KNOT_SCHEMA_NOT_FOUND	-13
#define KNOT_SCHEMA_EMPTY	-14
#define KNOT_INVALID_CREDENTIAL -15
#define KNOT_CREDENTIAL_UNAUTHORIZED -16

// Each KNoT Device or user has a unique ID and token as identification
// mechanism
#define KNOT_PROTOCOL_TOKEN_LEN			40
#define KNOT_PROTOCOL_UUID_LEN			36
// A human readable name for each device
#define KNOT_PROTOCOL_DEVICE_NAME_LEN		64
// A human readint8_table name for each data source/sink in the device
#define KNOT_PROTOCOL_DATA_NAME_LEN		64

#define KNOT_PROTOCOL_DATA_ID_MAX		0xFE
#define KNOT_PROTOCOL_DATA_ID_NA		0xFF

#define KNOT_MSG_INVALID			0x00
// KNoT connect/register messages (from device)
#define KNOT_MSG_REGISTER_REQ			0x10
#define KNOT_MSG_REGISTER_RESP			0x11
#define KNOT_MSG_UNREGISTER_REQ			0x12
#define KNOT_MSG_UNREGISTER_RESP		0x13
#define KNOT_MSG_AUTH_REQ			0x14
#define KNOT_MSG_AUTH_RESP			0x15
/*
 * KNoT device config messages (from device)
 * END flag indicates end of schema transfer.
 */
#define KNOT_MSG_SCHEMA			0x40
#define KNOT_MSG_SCHEMA_FLAG_END	0x01
#define KNOT_MSG_SCHEMA_RESP		0x44
// KNoT data sending config messages (from gateway)
#define KNOT_MSG_GET_INTERVAL		0x50
#define KNOT_MSG_SET_INTERVAL		0x51
#define KNOT_MSG_GET_THRESHOLD		0x52
#define KNOT_MSG_SET_THRESHOLD		0x53
// KNoT request messages (from gateway)
#define KNOT_MSG_GET_DATA			0x30
#define KNOT_MSG_SET_DATA			0x31
#define KNOT_MSG_GET_COMMAND			0x32
#define KNOT_MSG_SET_COMMAND			0x33
// KNoT response messages (from device)
#define KNOT_MSG_DATA					0x20
#define KNOT_MSG_DATA_RESP		0x21
#define KNOT_MSG_COMMAND		0x22
#define KNOT_MSG_INTERVAL			0x24
#define KNOT_MSG_THRESHOLD	0x26

#define KNOT_MSG_MAX_SIZE		KNOT_NET_PAYLOAD_SIZE

typedef struct __attribute__ ((packed)) {
	uint8_t			type;
	uint8_t			payload_len;
} knot_msg_header;

typedef struct __attribute__ ((packed)) {
	uint8_t			id;
	uint8_t			unit;
	uint8_t			value_type;
} knot_data_header;

#define KNOT_DATA_MAX_SIZE	(KNOT_MSG_MAX_SIZE - sizeof(knot_msg_header))
#define KNOT_DATA_RAW_SIZE	(KNOT_DATA_MAX_SIZE - sizeof(knot_data_header))

typedef struct __attribute__ ((packed)) {
	knot_data_header	hdr;
	uint8_t				value[KNOT_DATA_RAW_SIZE];
} knot_data_raw;

typedef struct __attribute__ ((packed)) {
	knot_data_header	hdr;
	int32_t			multiplier;
	int32_t			value_int;
	uint32_t			value_dec;
} knot_data_float;

typedef struct __attribute__ ((packed)) {
	knot_data_header	hdr;
	int32_t			multiplier;
	int32_t			value;
} knot_data_int;

typedef struct __attribute__ ((packed)) {
	knot_data_header	hdr;
	uint8_t			value;
} knot_data_bool;

typedef union __attribute__ ((packed)) {
	knot_data_header	hdr;
	knot_data_float		float_k;
	knot_data_int			int_k;
	knot_data_bool		bool_k;
	knot_data_raw			raw_k;
	uint8_t						data[KNOT_DATA_MAX_SIZE];
} knot_data;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
	int8_t				result;
} knot_msg_result;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
	knot_data						payload;
} knot_msg_data;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
	int8_t				result;
	char			uuid[KNOT_PROTOCOL_UUID_LEN];
} knot_msg_credential;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
	char			devName[KNOT_PROTOCOL_DEVICE_NAME_LEN];
} knot_msg_register;

/* Requirement: authenticated PHY link */
typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
} knot_msg_unregister;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
	char			uuid[KNOT_PROTOCOL_UUID_LEN];
	char                    token[KNOT_PROTOCOL_TOKEN_LEN];
} knot_msg_authentication;

typedef struct __attribute__ ((packed)) {
	uint8_t			data_id;
	uint16_t			data_type;
	char				name[KNOT_PROTOCOL_DATA_NAME_LEN];
} knot_schema;

typedef struct __attribute__ ((packed)) {
	knot_msg_header		hdr;
	knot_schema		schema;
} knot_msg_config;

typedef union {
	knot_msg_header		hdr;
	knot_msg_result			action;
	knot_msg_data		data;
	knot_msg_credential	cred;
	knot_msg_register	reg;
	knot_msg_unregister	unreg;
	knot_msg_authentication	auth;
	knot_msg_config		config;
	uint8_t						msg[KNOT_MSG_MAX_SIZE];
} knot_msg;

#endif //KNOT_PROTOCOL_APP_H
