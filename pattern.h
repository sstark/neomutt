/**
 * @file
 * Match patterns to emails
 *
 * @authors
 * Copyright (C) 2017 Richard Russon <rich@flatcap.org>
 * Copyright (C) 2019 Pietro Cerutti <gahr@gahr.ch>
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

#ifndef MUTT_PATTERN_H
#define MUTT_PATTERN_H

#include <regex.h>
#include <stdbool.h>
#include <stddef.h>
#include "mutt.h"

struct Buffer;
struct Email;
struct Envelope;
struct Mailbox;

/* These Config Variables are only used in pattern.c */
extern bool C_ThoroughSearch;

/* flag to mutt_pattern_comp() */
#define MUTT_FULL_MSG           (1<<0)  ///< enable body and header matching
#define MUTT_PATTERN_DYNAMIC    (1<<1)  ///< enable runtime date range evaluation

/**
 * struct Pattern - A simple (non-regex) pattern
 */
struct Pattern
{
  short op;
  bool not : 1;
  bool alladdr : 1;
  bool stringmatch : 1;
  bool groupmatch : 1;
  bool ign_case : 1; /**< ignore case for local stringmatch searches */
  bool isalias : 1;
  bool dynamic : 1;  ///< evaluate date ranges at run time
  bool ismulti : 1; /**< multiple case (only for I pattern now) */
  int min;
  int max;
  SLIST_ENTRY(Pattern) entries;
  struct PatternHead *child; /**< arguments to logical op */
  union {
    regex_t *regex;
    struct Group *group;
    char *str;
    struct ListHead multi_cases;
  } p;
};
SLIST_HEAD(PatternHead, Pattern);

typedef uint8_t PatternExecFlags;         ///< Flags for mutt_pattern_exec(), e.g. #MUTT_MATCH_FULL_ADDRESS
#define MUTT_PAT_EXEC_NO_FLAGS         0  ///< No flags are set
#define MUTT_MATCH_FULL_ADDRESS  (1 << 0) ///< Match the full address

/**
 * struct PatternCache - Cache commonly-used patterns
 *
 * This is used when a message is repeatedly pattern matched against.
 * e.g. for color, scoring, hooks.  It caches a few of the potentially slow
 * operations.
 * Each entry has a value of 0 = unset, 1 = false, 2 = true
 */
struct PatternCache
{
  int list_all;       /**< ^~l */
  int list_one;       /**<  ~l */
  int sub_all;        /**< ^~u */
  int sub_one;        /**<  ~u */
  int pers_recip_all; /**< ^~p */
  int pers_recip_one; /**<  ~p */
  int pers_from_all;  /**< ^~P */
  int pers_from_one;  /**<  ~P */
};

/**
 * enum PatternType - Types of pattern to match
 *
 * @note This enum piggy-backs on top of #MessageType
 *
 * @sa mutt_pattern_comp(), mutt_pattern_exec()
 */
enum PatternType
{
  MUTT_PAT_AND = MUTT_MT_MAX, ///< Both patterns must match
  MUTT_PAT_OR,                ///< Either pattern can match
  MUTT_PAT_THREAD,            ///< Pattern matches email thread
  MUTT_PAT_PARENT,            ///< Pattern matches parent
  MUTT_PAT_CHILDREN,          ///< Pattern matches a child email
  MUTT_PAT_TO,                ///< Pattern matches 'To:' field
  MUTT_PAT_CC,                ///< Pattern matches 'Cc:' field
  MUTT_PAT_COLLAPSED,         ///< Thread is collapsed
  MUTT_PAT_SUBJECT,           ///< Pattern matches 'Subject:' field
  MUTT_PAT_FROM,              ///< Pattern matches 'From:' field
  MUTT_PAT_DATE,              ///< Pattern matches 'Date:' field
  MUTT_PAT_DATE_RECEIVED,     ///< Pattern matches date received
  MUTT_PAT_DUPLICATED,        ///< Duplicate message
  MUTT_PAT_UNREFERENCED,      ///< Message is unreferenced in the thread
  MUTT_PAT_BROKEN,            ///< Message is part of a broken thread
  MUTT_PAT_ID,                ///< Pattern matches email's Message-Id
  MUTT_PAT_ID_EXTERNAL,       ///< Message-Id is among results from an external query
  MUTT_PAT_BODY,              ///< Pattern matches email's body
  MUTT_PAT_HEADER,            ///< Pattern matches email's header
  MUTT_PAT_HORMEL,            ///< Pattern matches email's spam score
  MUTT_PAT_WHOLE_MSG,         ///< Pattern matches raw email text
  MUTT_PAT_SENDER,            ///< Pattern matches sender
  MUTT_PAT_MESSAGE,           ///< Pattern matches message number
  MUTT_PAT_SCORE,             ///< Pattern matches email's score
  MUTT_PAT_SIZE,              ///< Pattern matches email's size
  MUTT_PAT_REFERENCE,         ///< Pattern matches 'References:' or 'In-Reply-To:' field
  MUTT_PAT_RECIPIENT,         ///< User is a recipient of the email
  MUTT_PAT_LIST,              ///< Email is on mailing list
  MUTT_PAT_SUBSCRIBED_LIST,   ///< Email is on subscribed mailing list
  MUTT_PAT_PERSONAL_RECIP,    ///< Email is addressed to the user
  MUTT_PAT_PERSONAL_FROM,     ///< Email is from the user
  MUTT_PAT_ADDRESS,           ///< Pattern matches any address field
  MUTT_PAT_CRYPT_SIGN,        ///< Message is signed
  MUTT_PAT_CRYPT_VERIFIED,    ///< Message is crypographically verified
  MUTT_PAT_CRYPT_ENCRYPT,     ///< Message is encrypted
  MUTT_PAT_PGP_KEY,           ///< Message has PGP key
  MUTT_PAT_XLABEL,            ///< Pattern matches keyword/label
  MUTT_PAT_SERVERSEARCH,      ///< Server-side pattern matches
  MUTT_PAT_DRIVER_TAGS,       ///< Pattern matches message tags
  MUTT_PAT_MIMEATTACH,        ///< Pattern matches number of attachments
  MUTT_PAT_MIMETYPE,          ///< Pattern matches MIME type
#ifdef USE_NNTP
  MUTT_PAT_NEWSGROUPS,        ///< Pattern matches newsgroup
#endif
  MUTT_PAT_MAX,
};

int mutt_pattern_exec(struct Pattern *pat, PatternExecFlags flags,
                      struct Mailbox *m, struct Email *e, struct PatternCache *cache);
struct PatternHead *mutt_pattern_comp(const char *s, int flags, struct Buffer *err);
void mutt_check_simple(struct Buffer *s, const char *simple);
void mutt_pattern_free(struct PatternHead **pat);

int mutt_which_case(const char *s);
int mutt_is_list_recipient(bool alladdr, struct Envelope *e);
int mutt_is_subscribed_list_recipient(bool alladdr, struct Envelope *e);
int mutt_pattern_func(int op, char *prompt);
int mutt_search_command(int cur, int op);

bool mutt_limit_current_thread(struct Email *e);

#endif /* MUTT_PATTERN_H */
