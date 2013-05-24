/*
 * See Copyright Notice in qnode.h
 */

#ifndef __QMAILBOX_H__
#define __QMAILBOX_H__

#include "qcore.h"
#include "qengine.h"
#include "qlist.h"
#include "qmutex.h"

struct qmailbox_t {
  unsigned int      active:1;
  qlist_t           lists[2];   /* one for read, one for write */
  qlist_t          *write;      /* current write list ptr */
  qlist_t          *read;       /* current read  list ptr */
  qevent_func_t    *callback;   /* mailbox reader callback */
  void             *reader;     /* mailbox reader */
  qsignal_t        *signal;
  qmutex_t          mutex;
};

qmailbox_t* qmailbox_new(qevent_func_t *callback, void *reader);
int         qmailbox_active(qengine_t *engine, qmailbox_t *box);
void        qmailbox_add(qmailbox_t *box, qmsg_t *msg);
int         qmailbox_get(qmailbox_t *box, qlist_t **list);

#endif  /* __QMAILBOX_H__ */
