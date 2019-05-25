/**
 * @file
 * Test code for mutt_buffer_increase_size()
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

void test_mutt_buffer_increase_size(void)
{
  // void mutt_buffer_increase_size(struct Buffer *buf, size_t new_size);

  {
    mutt_buffer_increase_size(NULL, 10);
    TEST_CHECK_(1, "mutt_buffer_increase_size(NULL, 10)");
  }

  {
    struct Buffer *buf = mutt_buffer_new();
    mutt_buffer_increase_size(buf, 10);
    TEST_CHECK_(1, "mutt_buffer_increase_size(buf, 10)");
    mutt_buffer_free(&buf);
  }

  {
    const int orig_size = 64;
    static int sizes[] = { 0, 32, orig_size, 128 };

    for (size_t i = 0; i < mutt_array_size(sizes); i++)
    {
      struct Buffer *buf = mutt_buffer_alloc(orig_size);
      TEST_CASE_("%d", sizes[i]);
      mutt_buffer_increase_size(buf, sizes[i]);
      TEST_CHECK(buf->dsize == MAX(orig_size, sizes[i]));
      mutt_buffer_free(&buf);
    }

  }


}
