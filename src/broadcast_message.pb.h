/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_BROADCAST_MESSAGE_PB_H_INCLUDED
#define PB_BROADCAST_MESSAGE_PB_H_INCLUDED
#include "nanopb/pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _broadcast_message_header_Attr {
    broadcast_message_header_Attr_RESERVE = 0,
    broadcast_message_header_Attr_REQUEST = 1,
    broadcast_message_header_Attr_REQACK = 2,
    broadcast_message_header_Attr_PUBLISH = 3,
    broadcast_message_header_Attr_PUBACK = 4,
    broadcast_message_header_Attr_NOTIFY = 5,
    broadcast_message_header_Attr_PUSH = 6
} broadcast_message_header_Attr;

/* Struct definitions */
typedef struct _broadcast_message_header {
    uint32_t dev;
    uint32_t cmd;
    uint32_t id;
    uint32_t dataLen;
    broadcast_message_header_Attr attr;
    int32_t ack;
    uint32_t seq;
} broadcast_message_header;

typedef struct _broadcast_message_payload {
    char name[10];
    char value[20];
} broadcast_message_payload;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _broadcast_message_header_Attr_MIN broadcast_message_header_Attr_RESERVE
#define _broadcast_message_header_Attr_MAX broadcast_message_header_Attr_PUSH
#define _broadcast_message_header_Attr_ARRAYSIZE ((broadcast_message_header_Attr)(broadcast_message_header_Attr_PUSH+1))

#define broadcast_message_header_attr_ENUMTYPE broadcast_message_header_Attr



/* Initializer values for message structs */
#define broadcast_message_header_init_default    {0, 0, 0, 0, _broadcast_message_header_Attr_MIN, 0, 0}
#define broadcast_message_payload_init_default   {"", ""}
#define broadcast_message_header_init_zero       {0, 0, 0, 0, _broadcast_message_header_Attr_MIN, 0, 0}
#define broadcast_message_payload_init_zero      {"", ""}

/* Field tags (for use in manual encoding/decoding) */
#define broadcast_message_header_dev_tag         1
#define broadcast_message_header_cmd_tag         2
#define broadcast_message_header_id_tag          3
#define broadcast_message_header_dataLen_tag     4
#define broadcast_message_header_attr_tag        5
#define broadcast_message_header_ack_tag         6
#define broadcast_message_header_seq_tag         7
#define broadcast_message_payload_name_tag       1
#define broadcast_message_payload_value_tag      2

/* Struct field encoding specification for nanopb */
#define broadcast_message_header_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   dev,               1) \
X(a, STATIC,   SINGULAR, UINT32,   cmd,               2) \
X(a, STATIC,   SINGULAR, UINT32,   id,                3) \
X(a, STATIC,   SINGULAR, UINT32,   dataLen,           4) \
X(a, STATIC,   SINGULAR, UENUM,    attr,              5) \
X(a, STATIC,   SINGULAR, SINT32,   ack,               6) \
X(a, STATIC,   SINGULAR, UINT32,   seq,               7)
#define broadcast_message_header_CALLBACK NULL
#define broadcast_message_header_DEFAULT NULL

#define broadcast_message_payload_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   name,              1) \
X(a, STATIC,   SINGULAR, STRING,   value,             2)
#define broadcast_message_payload_CALLBACK NULL
#define broadcast_message_payload_DEFAULT NULL

extern const pb_msgdesc_t broadcast_message_header_msg;
extern const pb_msgdesc_t broadcast_message_payload_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define broadcast_message_header_fields &broadcast_message_header_msg
#define broadcast_message_payload_fields &broadcast_message_payload_msg

/* Maximum encoded size of messages (where known) */
#define BROADCAST_MESSAGE_PB_H_MAX_SIZE          broadcast_message_header_size
#define broadcast_message_header_size            38
#define broadcast_message_payload_size           32

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
