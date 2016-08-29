/*
 * Copyright (c) 2016, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#ifndef KNOT_LIB_H
#define KNOT_LIB_H

int knot_value_type_is_valid(uint8_t type);

int knot_schema_is_valid(uint8_t type_id, uint8_t value_type, uint8_t unit);

#endif // KNOT_LIB_H
