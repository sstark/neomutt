/**
 * @file
 * Type representing a mailbox
 *
 * @authors
 * Copyright (C) 2017-2018 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page config_magic Type: Mailbox types
 *
 * Type representing a mailbox.
 */

#include "config.h"
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include "mutt/mutt.h"
#include "set.h"
#include "types.h"

/**
 * MagicValues - Valid strings for mailbox types
 *
 * These strings are case-insensitive.
 */
const char *MagicValues[] = {
  NULL, "mbox", "MMDF", "MH", "Maildir", NULL,
};

/**
 * magic_string_set - Set a Mailbox Magic by string - Implements ::cst_string_set()
 */
static int magic_string_set(const struct ConfigSet *cs, void *var, struct ConfigDef *cdef,
                            const char *value, struct Buffer *err)
{
  if (!cs || !cdef)
    return CSR_ERR_CODE; /* LCOV_EXCL_LINE */

  if (!value || !value[0])
  {
    mutt_buffer_printf(err, "Option %s may not be empty", cdef->name);
    return CSR_ERR_INVALID | CSR_INV_TYPE;
  }

  int num = -1;
  for (size_t i = 1; MagicValues[i]; i++)
  {
    if (mutt_str_strcasecmp(MagicValues[i], value) == 0)
    {
      num = i;
      break;
    }
  }

  if (num < 1)
  {
    mutt_buffer_printf(err, "Invalid magic value: %s", value);
    return CSR_ERR_INVALID | CSR_INV_TYPE;
  }

  if (var)
  {
    if (num == (*(short *) var))
      return CSR_SUCCESS | CSR_SUC_NO_CHANGE;

    if (cdef->validator)
    {
      int rc = cdef->validator(cs, cdef, (intptr_t) num, err);

      if (CSR_RESULT(rc) != CSR_SUCCESS)
        return rc | CSR_INV_VALIDATOR;
    }

    *(short *) var = num;
  }
  else
  {
    cdef->initial = num;
  }

  return CSR_SUCCESS;
}

/**
 * magic_string_get - Get a Mailbox Magic as a string - Implements ::cst_string_get()
 */
static int magic_string_get(const struct ConfigSet *cs, void *var,
                            const struct ConfigDef *cdef, struct Buffer *result)
{
  if (!cs || !cdef)
    return CSR_ERR_CODE; /* LCOV_EXCL_LINE */

  unsigned int value;

  if (var)
    value = *(short *) var;
  else
    value = (int) cdef->initial;

  if ((value < 1) || (value >= (mutt_array_size(MagicValues) - 1)))
  {
    mutt_debug(LL_DEBUG1, "Variable has an invalid value: %d\n", value);
    return CSR_ERR_INVALID | CSR_INV_TYPE;
  }

  mutt_buffer_addstr(result, MagicValues[value]);
  return CSR_SUCCESS;
}

/**
 * magic_native_set - Set a Mailbox Magic config item by int - Implements ::cst_native_set()
 */
static int magic_native_set(const struct ConfigSet *cs, void *var,
                            const struct ConfigDef *cdef, intptr_t value, struct Buffer *err)
{
  if (!cs || !var || !cdef)
    return CSR_ERR_CODE; /* LCOV_EXCL_LINE */

  if ((value < 1) || (value >= (mutt_array_size(MagicValues) - 1)))
  {
    mutt_buffer_printf(err, "Invalid magic value: %ld", value);
    return CSR_ERR_INVALID | CSR_INV_TYPE;
  }

  if (value == (*(short *) var))
    return CSR_SUCCESS | CSR_SUC_NO_CHANGE;

  if (cdef->validator)
  {
    int rc = cdef->validator(cs, cdef, value, err);

    if (CSR_RESULT(rc) != CSR_SUCCESS)
      return rc | CSR_INV_VALIDATOR;
  }

  *(short *) var = value;
  return CSR_SUCCESS;
}

/**
 * magic_native_get - Get an int from a Mailbox Magic config item - Implements ::cst_native_get()
 */
static intptr_t magic_native_get(const struct ConfigSet *cs, void *var,
                                 const struct ConfigDef *cdef, struct Buffer *err)
{
  if (!cs || !var || !cdef)
    return INT_MIN; /* LCOV_EXCL_LINE */

  return *(short *) var;
}

/**
 * magic_reset - Reset a Mailbox Magic to its initial value - Implements ::cst_reset()
 */
static int magic_reset(const struct ConfigSet *cs, void *var,
                       const struct ConfigDef *cdef, struct Buffer *err)
{
  if (!cs || !var || !cdef)
    return CSR_ERR_CODE; /* LCOV_EXCL_LINE */

  if (cdef->initial == (*(short *) var))
    return CSR_SUCCESS | CSR_SUC_NO_CHANGE;

  if (cdef->validator)
  {
    int rc = cdef->validator(cs, cdef, cdef->initial, err);

    if (CSR_RESULT(rc) != CSR_SUCCESS)
      return rc | CSR_INV_VALIDATOR;
  }

  *(short *) var = cdef->initial;
  return CSR_SUCCESS;
}

/**
 * magic_init - Register the Mailbox Magic config type
 * @param cs Config items
 */
void magic_init(struct ConfigSet *cs)
{
  const struct ConfigSetType cst_magic = {
    "magic",
    magic_string_set,
    magic_string_get,
    magic_native_set,
    magic_native_get,
    magic_reset,
    NULL,
  };
  cs_register_type(cs, DT_MAGIC, &cst_magic);
}
