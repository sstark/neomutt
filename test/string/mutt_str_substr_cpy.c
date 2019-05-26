/**
 * @file
 * Test code for mutt_str_substr_cpy()
 *
 * @authors
 * Copyright (C) 2019 Richard Russon <rich@flatcap.org>
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

#define TEST_NO_MAIN
#include "acutest.h"
#include "config.h"
#include "mutt/mutt.h"

void test_mutt_str_substr_cpy(void)
{
  // char *mutt_str_substr_cpy(char *dest, const char *begin, const char *end, size_t destlen);

  const char *str = "apple banana";

  {
    TEST_CHECK(mutt_str_substr_cpy(NULL, str + 3, str + 7, 32) == NULL);
  }

  {
    char buf[32] = { 0 };
    TEST_CHECK(mutt_str_substr_cpy(buf, NULL, str + 7, sizeof(buf)) == buf);
  }

  {
    char buf[32] = { 0 };
    TEST_CHECK(mutt_str_substr_cpy(buf, str + 3, NULL, sizeof(buf)) == buf);
  }
  // rename to copy
  // reorder params
  // null
  // empty
  // normal
  // limit: 0, short, len, overlen

}
