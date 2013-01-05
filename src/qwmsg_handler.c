/*
 * See Copyright Notice in qnode.h
 */

#include <stdio.h>
#include "qassert.h"
#include "qactor.h"
#include "qdefines.h"
#include "qlist.h"
#include "qlog.h"
#include "qluautil.h"
#include "qmalloc.h"
#include "qmsg.h"
#include "qserver.h"
#include "qthread.h"

static int server_handle_wrong_msg(qserver_t *server, qmsg_t *msg) {
  UNUSED(server);
  UNUSED(msg);
  qerror("handle server type %d msg error", msg->type);
  return 0;
}

static int server_handle_spawn_msg(qserver_t *server, qmsg_t *msg) {
  qinfo("handle spawn msg");
  qid_t aid = msg->args.spawn.aid;
  lua_State *state = msg->args.spawn.state;
  qactor_t *actor = qactor_new(aid, state);
  actor->parent = msg->args.spawn.parent;
  msg->args.spawn.actor = actor;
  qmsg_set_undelete(msg);
  msg->sender_id = QSERVER_THREAD_TID;
  msg->receiver_id = qserver_worker_thread();
  qserver_send_mail(msg);
  server->actors[aid] = actor;
  return 0;
}

static int server_handle_thread_start_msg(qserver_t *server, qmsg_t *msg) {
  qinfo("handle thread start msg");
  static int thread_num = 0;
  UNUSED(server);
  UNUSED(msg);
  ++thread_num;
  if (thread_num == g_server->config->thread_num) {
  }
  return 0;
}

static int server_handle_thread_box_msg(qserver_t *server, qmsg_t *msg) {
  UNUSED(server);
  qinfo("handle info msg\n");
  qmsg_set_undelete(msg);
  qserver_send_mail(msg);
  return 0;
}

wmsg_handler wmsg_handlers[] = {
  &server_handle_wrong_msg,         /* wrong */
  &server_handle_thread_start_msg,  /* w_thread_started */
  &server_handle_wrong_msg,         /* s_init, wrong */
  &server_handle_thread_box_msg,    /* w_thread_box */
  &server_handle_wrong_msg,         /* s_start, wrong */
  &server_handle_spawn_msg,         /* spawn */
};
