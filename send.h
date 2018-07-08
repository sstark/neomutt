/**
 * @file
 * Prepare and send an email
 *
 * @authors
 * Copyright (C) 2018 Richard Russon <rich@flatcap.org>
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

#ifndef MUTT_SEND_H
#define MUTT_SEND_H

#include <stdio.h>

struct Address;
struct Body;
struct Context;
struct Envelope;
struct Header;

/* These Config Variables are only used in send.c */
extern unsigned char AbortNoattach; /* forgotten attachment detector */
extern struct Regex *AbortNoattachRegex;
extern unsigned char AbortNosubject;
extern unsigned char AbortUnmodified;
extern bool          AskFollowUp;
extern bool          AskXCommentTo;
extern char *        ContentType;
extern bool          CryptAutoencrypt;
extern bool          CryptAutopgp;
extern bool          CryptAutosign;
extern bool          CryptAutosmime;
extern bool          CryptReplyencrypt;
extern bool          CryptReplysign;
extern bool          CryptReplysignencrypted;
extern char *        EmptySubject;
extern bool          FastReply;
extern unsigned char FccAttach;
extern bool          FccClear;
extern bool          FollowupTo;
extern char *        ForwardAttributionIntro;
extern char *        ForwardAttributionTrailer;
extern unsigned char ForwardEdit;
extern char *        ForwardFormat;
extern bool          ForwardReferences;
extern bool          Hdrs;
extern unsigned char HonorFollowupTo;
extern bool          IgnoreListReplyTo;
extern unsigned char Include;
extern bool          Metoo;
extern bool          NmRecord;
extern bool          PgpReplyinline;
extern char *        PostIndentString;
extern bool          PostponeEncrypt;
extern char *        PostponeEncryptAs;
extern unsigned char Recall;
extern bool          ReplySelf;
extern unsigned char ReplyTo;
extern bool          ReplyWithXorig;
extern bool          ReverseName;
extern bool          ReverseRealname;
extern bool          SigDashes;
extern char *        Signature;
extern bool          SigOnTop;
extern bool          UseFrom;

int             ci_send_message(int flags, struct Header *msg, char *tempfile, struct Context *ctx, struct Header *cur);
void            mutt_add_to_reference_headers(struct Envelope *env, struct Envelope *curenv);
int             mutt_compose_to_sender(struct Header *hdr);
struct Address *mutt_default_from(void);
void            mutt_encode_descriptions(struct Body *b, short recurse);
int             mutt_fetch_recips(struct Envelope *out, struct Envelope *in, int flags);
void            mutt_fix_reply_recipients(struct Envelope *env);
void            mutt_forward_intro(struct Context *ctx, struct Header *cur, FILE *fp);
void            mutt_forward_trailer(struct Context *ctx, struct Header *cur, FILE *fp);
void            mutt_make_attribution(struct Context *ctx, struct Header *cur, FILE *out);
void            mutt_make_forward_subject(struct Envelope *env, struct Context *ctx, struct Header *cur);
void            mutt_make_misc_reply_headers(struct Envelope *env, struct Envelope *curenv);
void            mutt_make_post_indent(struct Context *ctx, struct Header *cur, FILE *out);
struct Address *mutt_remove_xrefs(struct Address *a, struct Address *b);
int             mutt_resend_message(FILE *fp, struct Context *ctx, struct Header *cur);
void            mutt_set_followup_to(struct Envelope *e);

#endif /* MUTT_SEND_H */
