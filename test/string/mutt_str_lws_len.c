/**
 * @file
 * Test code for mutt_str_lws_len()
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

void test_mutt_str_lws_len(void)
{
  // size_t mutt_str_lws_len(const char *s, size_t n);

  {
    TEST_CHECK(mutt_str_lws_len(NULL, 10) == 0);
  }
  // null
  // no leading space
  // leading space tab CR LF
  // mix of leading space

}
