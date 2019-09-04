#include <bson/bson.h>

#include "mongo.h"

#define RPC(_name, _code)                                               \
   static void _mongoc_rpc_printf_##_name (mongoc_rpc_##_name##_t *rpc) \
   {                                                                    \
      BSON_ASSERT (rpc);                                                \
      _code                                                             \
   }
#define UINT8_FIELD(_name) \
    do {\
        if (strcmp("msg_len", #_name) \
                && strcmp("request_id", #_name)\
                && strcmp("response_to", #_name)\
                && strcmp("opcode", #_name)\
                ) \
            printf ("  " #_name " : %u\n", rpc->_name);\
    }while(0);
#define INT32_FIELD(_name) \
    do {\
        if (strcmp("msg_len", #_name) \
                && strcmp("request_id", #_name)\
                && strcmp("response_to", #_name)\
                && strcmp("opcode", #_name)\
                && strcmp("flags", #_name)\
                ) \
            printf ("  " #_name " : %d\n", rpc->_name);\
    }while(0);
#define ENUM_FIELD(_name) 
//printf ("  " #_name " : %u\n", rpc->_name);
#define INT64_FIELD(_name) \
   printf ("  " #_name " : %" PRIi64 "\n", (int64_t) rpc->_name);
#define CSTRING_FIELD(_name) printf ("  " #_name " : %s\n", rpc->_name);
#define BSON_FIELD(_name)                                   \
   do {                                                     \
      bson_t b;                                             \
      char *s;                                              \
      int32_t __l;                                          \
      memcpy (&__l, rpc->_name, 4);                         \
      __l = BSON_UINT32_FROM_LE (__l);                      \
      BSON_ASSERT (bson_init_static (&b, rpc->_name, __l)); \
      s = bson_as_relaxed_extended_json (&b, NULL);         \
      printf ("  " #_name " : %s\n", s);                    \
      bson_free (s);                                        \
      bson_destroy (&b);                                    \
   } while (0);
#define BSON_ARRAY_FIELD(_name)                                       \
   do {                                                               \
      bson_reader_t *__r;                                             \
      bool __eof;                                                     \
      const bson_t *__b;                                              \
      __r = bson_reader_new_from_data (rpc->_name, rpc->_name##_len); \
      while ((__b = bson_reader_read (__r, &__eof))) {                \
         char *s = bson_as_relaxed_extended_json (__b, NULL);         \
         printf ("  " #_name " : %s\n", s);                           \
         bson_free (s);                                               \
      }                                                               \
      bson_reader_destroy (__r);                                      \
   } while (0);
#define IOVEC_ARRAY_FIELD(_name)                           \
   do {                                                    \
      ssize_t _i;                                          \
      size_t _j;                                           \
      for (_i = 0; _i < rpc->n_##_name; _i++) {            \
         printf ("  " #_name " : ");                       \
         for (_j = 0; _j < rpc->_name[_i].iov_len; _j++) { \
            uint8_t u;                                     \
            u = ((char *) rpc->_name[_i].iov_base)[_j];    \
            printf (" %02x", u);                           \
         }                                                 \
         printf ("\n");                                    \
      }                                                    \
   } while (0);
#define SECTION_ARRAY_FIELD(_name)                                          \
   do {                                                                     \
      ssize_t _i;                                                           \
      printf ("  " #_name " : %d\n", rpc->n_##_name);                       \
      for (_i = 0; _i < rpc->n_##_name; _i++) {                             \
         if (rpc->_name[_i].payload_type == 0) {                            \
            do {                                                            \
               bson_t b;                                                    \
               char *s;                                                     \
               int32_t __l;                                                 \
               memcpy (&__l, rpc->_name[_i].payload.bson_document, 4);      \
               __l = BSON_UINT32_FROM_LE (__l);                             \
               BSON_ASSERT (bson_init_static (                              \
                  &b, rpc->_name[_i].payload.bson_document, __l));          \
               s = bson_as_relaxed_extended_json (&b, NULL);                \
               printf ("  Type %d: %s\n", rpc->_name[_i].payload_type, s);  \
               bson_free (s);                                               \
               bson_destroy (&b);                                           \
            } while (0);                                                    \
         } else if (rpc->_name[_i].payload_type == 1) {                     \
            bson_reader_t *__r;                                             \
            int max = rpc->_name[_i].payload.sequence.size -                \
                      strlen (rpc->_name[_i].payload.sequence.identifier) - \
                      1 - sizeof (int32_t);                                 \
            bool __eof;                                                     \
            const bson_t *__b;                                              \
            printf ("  Identifier: %s\n",                                   \
                    rpc->_name[_i].payload.sequence.identifier);            \
            printf ("  Size: %d\n", max);                                   \
            __r = bson_reader_new_from_data (                               \
               rpc->_name[_i].payload.sequence.bson_documents, max);        \
            while ((__b = bson_reader_read (__r, &__eof))) {                \
               char *s = bson_as_relaxed_extended_json (__b, NULL);         \
               bson_free (s);                                               \
            }                                                               \
            bson_reader_destroy (__r);                                      \
         }                                                                  \
      }                                                                     \
   } while (0);
#define BSON_OPTIONAL(_check, _code) \
   if (rpc->_check) {                \
      _code                          \
   }
#define RAW_BUFFER_FIELD(_name)                      \
   {                                                 \
      ssize_t __i;                                   \
      printf ("  " #_name " :");                     \
      for (__i = 0; __i < rpc->_name##_len; __i++) { \
         uint8_t u;                                  \
         u = ((char *) rpc->_name)[__i];             \
         printf (" %02x", u);                        \
      }                                              \
      printf ("\n");                                 \
   }
#define INT64_ARRAY_FIELD(_len, _name)                                     \
   do {                                                                    \
      ssize_t i;                                                           \
      for (i = 0; i < rpc->_len; i++) {                                    \
         printf ("  " #_name " : %" PRIi64 "\n", (int64_t) rpc->_name[i]); \
      }                                                                    \
      rpc->_len = BSON_UINT32_FROM_LE (rpc->_len);                         \
   } while (0);


#include "op-delete.def"
#include "op-get-more.def"
#include "op-insert.def"
#include "op-kill-cursors.def"
#include "op-msg.def"
#include "op-query.def"
#include "op-reply.def"
#include "op-compressed.def"
#include "op-update.def"
#include "op-command.def"
#include "op-commandreply.def"


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





#define RPC(_name, _code)                                             \
   static bool _mongoc_rpc_scatter_##_name (                          \
      mongoc_rpc_##_name##_t *rpc, const uint8_t *buf, size_t buflen) \
   {                                                                  \
      BSON_ASSERT (rpc);                                              \
      BSON_ASSERT (buf);                                              \
      BSON_ASSERT (buflen);                                           \
      _code return true;                                              \
   }
#define UINT8_FIELD(_name)       \
   if (buflen < 1) {             \
      return false;              \
   }                             \
   memcpy (&rpc->_name, buf, 1); \
   buflen -= 1;                  \
   buf += 1;
#define INT32_FIELD(_name)       \
   if (buflen < 4) {             \
      return false;              \
   }                             \
   memcpy (&rpc->_name, buf, 4); \
   buflen -= 4;                  \
   buf += 4;
#define ENUM_FIELD INT32_FIELD
#define INT64_FIELD(_name)       \
   if (buflen < 8) {             \
      return false;              \
   }                             \
   memcpy (&rpc->_name, buf, 8); \
   buflen -= 8;                  \
   buf += 8;
#define INT64_ARRAY_FIELD(_len, _name)              \
   do {                                             \
      size_t needed;                                \
      if (buflen < 4) {                             \
         return false;                              \
      }                                             \
      memcpy (&rpc->_len, buf, 4);                  \
      buflen -= 4;                                  \
      buf += 4;                                     \
      needed = BSON_UINT32_FROM_LE (rpc->_len) * 8; \
      if (needed > buflen) {                        \
         return false;                              \
      }                                             \
      rpc->_name = (int64_t *) buf;                 \
      buf += needed;                                \
      buflen -= needed;                             \
   } while (0);
#define CSTRING_FIELD(_name)                 \
   do {                                      \
      size_t __i;                            \
      bool found = false;                    \
      for (__i = 0; __i < buflen; __i++) {   \
         if (!buf[__i]) {                    \
            rpc->_name = (const char *) buf; \
            buflen -= __i + 1;               \
            buf += __i + 1;                  \
            found = true;                    \
            break;                           \
         }                                   \
      }                                      \
      if (!found) {                          \
         return false;                       \
      }                                      \
   } while (0);
#define BSON_FIELD(_name)              \
   do {                                \
      uint32_t __l;                    \
      if (buflen < 4) {                \
         return false;                 \
      }                                \
      memcpy (&__l, buf, 4);           \
      __l = BSON_UINT32_FROM_LE (__l); \
      if (__l < 5 || __l > buflen) {   \
         return false;                 \
      }                                \
      rpc->_name = (uint8_t *) buf;    \
      buf += __l;                      \
      buflen -= __l;                   \
   } while (0);
#define BSON_ARRAY_FIELD(_name)         \
   rpc->_name = (uint8_t *) buf;        \
   rpc->_name##_len = (int32_t) buflen; \
   buf = NULL;                          \
   buflen = 0;
#define BSON_OPTIONAL(_check, _code) \
   if (buflen) {                     \
      _code                          \
   }
#define IOVEC_ARRAY_FIELD(_name)              \
   rpc->_name##_recv.iov_base = (void *) buf; \
   rpc->_name##_recv.iov_len = buflen;        \
   rpc->_name = &rpc->_name##_recv;           \
   rpc->n_##_name = 1;                        \
   buf = NULL;                                \
   buflen = 0;
#define SECTION_ARRAY_FIELD(_name)                                          \
   do {                                                                     \
      uint32_t __l;                                                         \
      mongoc_rpc_section_t *section = &rpc->_name[rpc->n_##_name];          \
      section->payload_type = buf[0];                                       \
      buf++;                                                                \
      buflen -= 1;                                                          \
      memcpy (&__l, buf, 4);                                                \
      __l = BSON_UINT32_FROM_LE (__l);                                      \
      if (section->payload_type == 0) {                                     \
         section->payload.bson_document = buf;                              \
      } else {                                                              \
         const uint8_t *section_buf = buf + 4;                              \
         section->payload.sequence.size = __l;                              \
         section->payload.sequence.identifier = (const char *) section_buf; \
         section_buf += strlen ((const char *) section_buf) + 1;            \
         section->payload.sequence.bson_documents = section_buf;            \
      }                                                                     \
      buf += __l;                                                           \
      buflen -= __l;                                                        \
      rpc->n_##_name++;                                                     \
   } while (buflen);
#define RAW_BUFFER_FIELD(_name)         \
   rpc->_name = (void *) buf;           \
   rpc->_name##_len = (int32_t) buflen; \
   buf = NULL;                          \
   buflen = 0;


#include "op-delete.def"
#include "op-get-more.def"
#include "op-header.def"
#include "op-insert.def"
#include "op-kill-cursors.def"
#include "op-msg.def"
#include "op-query.def"
#include "op-reply.def"
#include "op-reply-header.def"
#include "op-compressed.def"
#include "op-update.def"
#include "op-command.def"
#include "op-commandreply.def"


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
_mongoc_rpc_printf (mongoc_rpc_t *rpc)
{
   switch ((mongoc_opcode_t) rpc->header.opcode) {
   case MONGOC_OPCODE_REPLY:
      _mongoc_rpc_printf_reply (&rpc->reply);
      break;
   case MONGOC_OPCODE_MSG:
      _mongoc_rpc_printf_msg (&rpc->msg);
      break;
   case MONGOC_OPCODE_UPDATE:
      _mongoc_rpc_printf_update (&rpc->update);
      break;
   case MONGOC_OPCODE_INSERT:
      _mongoc_rpc_printf_insert (&rpc->insert);
      break;
   case MONGOC_OPCODE_QUERY:
      _mongoc_rpc_printf_query (&rpc->query);
      break;
   case MONGOC_OPCODE_GET_MORE:
      _mongoc_rpc_printf_get_more (&rpc->get_more);
      break;
   case MONGOC_OPCODE_DELETE:
      _mongoc_rpc_printf_delete (&rpc->delete_);
      break;
   case MONGOC_OPCODE_KILL_CURSORS:
      _mongoc_rpc_printf_kill_cursors (&rpc->kill_cursors);
      break;
   case MONGOC_OPCODE_COMPRESSED:
      _mongoc_rpc_printf_compressed (&rpc->compressed);
      break;
   case OP_COMMAND:
      _mongoc_rpc_printf_command (&rpc->command);
      break;
   case OP_COMMANDREPLY:
      _mongoc_rpc_printf_commandreply (&rpc->commandreply);
      break;
   default:
      printf ("Unknown rpc type: 0x%08x\n", rpc->header.opcode);
      break;
   }
   printf ("\n");
}

bool
_mongoc_rpc_scatter (mongoc_rpc_t *rpc, const uint8_t *buf, size_t buflen)
{
   mongoc_opcode_t opcode;

   memset (rpc, 0, sizeof *rpc);

   if (BSON_UNLIKELY (buflen < 16)) {
      return false;
   }

   if (!_mongoc_rpc_scatter_header (&rpc->header, buf, 16)) {
      return false;
   }
   opcode = (mongoc_opcode_t) BSON_UINT32_FROM_LE (rpc->header.opcode);

   switch (opcode) {
   case MONGOC_OPCODE_COMPRESSED:
      return _mongoc_rpc_scatter_compressed (&rpc->compressed, buf, buflen);

   case MONGOC_OPCODE_REPLY:
      return _mongoc_rpc_scatter_reply (&rpc->reply, buf, buflen);

   case MONGOC_OPCODE_MSG:
      return _mongoc_rpc_scatter_msg (&rpc->msg, buf, buflen);


   case MONGOC_OPCODE_UPDATE:
      return _mongoc_rpc_scatter_update (&rpc->update, buf, buflen);

   case MONGOC_OPCODE_INSERT:
      return _mongoc_rpc_scatter_insert (&rpc->insert, buf, buflen);

   case MONGOC_OPCODE_QUERY:
      return _mongoc_rpc_scatter_query (&rpc->query, buf, buflen);

   case MONGOC_OPCODE_GET_MORE:
      return _mongoc_rpc_scatter_get_more (&rpc->get_more, buf, buflen);

   case MONGOC_OPCODE_DELETE:
      return _mongoc_rpc_scatter_delete (&rpc->delete_, buf, buflen);

   case MONGOC_OPCODE_KILL_CURSORS:
      return _mongoc_rpc_scatter_kill_cursors (&rpc->kill_cursors, buf, buflen);

   case OP_COMMAND:
      return _mongoc_rpc_scatter_command(&rpc->command, buf, buflen);
   
   case OP_COMMANDREPLY:
      return _mongoc_rpc_scatter_commandreply(&rpc->commandreply, buf, buflen);
   
   default:
      printf ("Unknown rpc type: 0x%08d\n", opcode);
      return false;
   }
}


bool
_mongoc_rpc_scatter_reply_header_only (mongoc_rpc_t *rpc,
                                       const uint8_t *buf,
                                       size_t buflen)
{
   if (BSON_UNLIKELY (buflen < sizeof (mongoc_rpc_reply_header_t))) {
      return false;
   }
   //mongoc_counter_op_ingress_reply_inc ();
   //mongoc_counter_op_ingress_total_inc ();
   return _mongoc_rpc_scatter_reply_header (&rpc->reply_header, buf, buflen);
}


