#ifndef _MONGO_H
#define _MONGO_H

#include <stdint.h>
#include <bson/bson.h>
#include <stddef.h>

typedef enum {
   MONGOC_OPCODE_REPLY = 1,
   MONGOC_OPCODE_UPDATE = 2001,
   MONGOC_OPCODE_INSERT = 2002,
   MONGOC_OPCODE_QUERY = 2004,
   MONGOC_OPCODE_GET_MORE = 2005,
   MONGOC_OPCODE_DELETE = 2006,
   MONGOC_OPCODE_KILL_CURSORS = 2007,
   MONGOC_OPCODE_COMPRESSED = 2012,
   MONGOC_OPCODE_MSG = 2013,
   OP_COMMAND = 2010, 
   OP_COMMANDREPLY = 2011,
} mongoc_opcode_t;

#ifdef _WIN32
#include <stddef.h>
#else
#include <sys/uio.h>
#endif

#ifdef _WIN32
typedef struct {
   size_t iov_len;
   char *iov_base;
} mongoc_iovec_t;

BSON_STATIC_ASSERT2 (sizeof_iovect_t,
                     sizeof (mongoc_iovec_t) == sizeof (WSABUF));
BSON_STATIC_ASSERT2 (offsetof_iovec_base,
                     offsetof (mongoc_iovec_t, iov_base) ==
                        offsetof (WSABUF, buf));
BSON_STATIC_ASSERT2 (offsetof_iovec_len,
                     offsetof (mongoc_iovec_t, iov_len) ==
                        offsetof (WSABUF, len));

#else
typedef struct iovec mongoc_iovec_t;
#endif

typedef struct _mongoc_rpc_section_t {
   uint8_t payload_type;
   union {
      /* payload_type == 0 */
      const uint8_t *bson_document;
      /* payload_type == 1 */
      struct {
         int32_t size;
         uint32_t size_le;
         const char *identifier;
         const uint8_t *bson_documents;
      } sequence;
   } payload;
} mongoc_rpc_section_t;


#define RPC(_name, _code) \
   typedef struct {       \
      _code               \
   } mongoc_rpc_##_name##_t;
#define ENUM_FIELD(_name) uint32_t _name;
#define INT32_FIELD(_name) int32_t _name;
#define UINT8_FIELD(_name) uint8_t _name;
#define INT64_FIELD(_name) int64_t _name;
#define INT64_ARRAY_FIELD(_len, _name) \
   int32_t _len;                       \
   int64_t *_name;
#define CSTRING_FIELD(_name) const char *_name;
#define BSON_FIELD(_name) const uint8_t *_name;
#define BSON_ARRAY_FIELD(_name) \
   const uint8_t *_name;        \
   int32_t _name##_len;
#define IOVEC_ARRAY_FIELD(_name) \
   const mongoc_iovec_t *_name;  \
   int32_t n_##_name;            \
   mongoc_iovec_t _name##_recv;
#define SECTION_ARRAY_FIELD(_name) \
   mongoc_rpc_section_t _name[2];  \
   int32_t n_##_name;
#define RAW_BUFFER_FIELD(_name) \
   const uint8_t *_name;        \
   int32_t _name##_len;
#define BSON_OPTIONAL(_check, _code) _code


#pragma pack(1)
#include "op-delete.def"
#include "op-get-more.def"
#include "op-header.def"
#include "op-insert.def"
#include "op-kill-cursors.def"
#include "op-query.def"
#include "op-reply.def"
#include "op-reply-header.def"
#include "op-update.def"
#include "op-compressed.def"
#include "op-command.def"
#include "op-commandreply.def"
/* restore default packing */
#pragma pack()

#include "op-msg.def"

typedef union {
   mongoc_rpc_delete_t delete_;
   mongoc_rpc_get_more_t get_more;
   mongoc_rpc_header_t header;
   mongoc_rpc_insert_t insert;
   mongoc_rpc_kill_cursors_t kill_cursors;
   mongoc_rpc_msg_t msg;
   mongoc_rpc_query_t query;
   mongoc_rpc_reply_t reply;
   mongoc_rpc_reply_header_t reply_header;
   mongoc_rpc_update_t update;
   mongoc_rpc_compressed_t compressed;
   mongoc_rpc_command_t command;
   mongoc_rpc_commandreply_t commandreply;
} mongoc_rpc_t;

#undef RPC
#undef ENUM_FIELD
#undef UINT8_FIELD
#undef INT32_FIELD
#undef INT64_FIELD
#undef INT64_ARRAY_FIELD
#undef CSTRING_FIELD
#undef BSON_FIELD
#undef BSON_ARRAY_FIELD
#undef IOVEC_ARRAY_FIELD
#undef SECTION_ARRAY_FIELD
#undef BSON_OPTIONAL
#undef RAW_BUFFER_FIELD

void
_mongoc_rpc_printf (mongoc_rpc_t *rpc);
bool
_mongoc_rpc_scatter (mongoc_rpc_t *rpc, const uint8_t *buf, size_t buflen);
bool
_mongoc_rpc_scatter_reply_header_only (mongoc_rpc_t *rpc,
                                       const uint8_t *buf,
                                       size_t buflen);


#endif
