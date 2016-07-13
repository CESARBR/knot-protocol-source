/*
 * Copyright (c) 2016, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#include <stdint.h>
#include <errno.h>

#include "knot_types.h"
#include "knot_lib.h"

int knot_value_type_is_valid(uint8_t type)
{
	if (type < KNOT_VALUE_TYPE_MIN || type > KNOT_VALUE_TYPE_MAX)
		return ERANGE;

	return 0;
}
