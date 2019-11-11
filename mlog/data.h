#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>
#include <time.h>

#define MAX_MESSAGE_SIZE 1000

typedef union {
    struct {
        uint8_t client_id;
        uint8_t session_id;
    } separated;
    uint16_t session_id;
} mlog_session_id;

// GENERATED
typedef struct {
    uint8_t severity;
    char message[MAX_MESSAGE_SIZE];
} mlog_user_data;

typedef struct {
    mlog_session_id id;
    time_t timestamp;
    mlog_user_data data;
} mlog_session_data;

typedef enum {
    ERROR = -1,
    NOT_INITIALIZED = 0,
    INITIALIZED = 1,
    STARTED = 2,
    STOPPED = 3,
    FINISHED = 4,
    DISCONNECTED = 5
} mlog_session_state;

typedef struct {
    uint32_t major : 10;
    uint32_t minor : 10;
    uint32_t patch : 10;
    uint32_t __pad : 2;
} mlog_software_version;

typedef struct {
    uint64_t max_index;
    uint8_t endianness;
    mlog_software_version version;
    char * user_data_format = "%d %100s";
} mlog_session_metadata;

typedef struct {
	int last_index;
    mlog_session_metadata metadata;
    mlog_session_data data[10];
    mlog_session_state state;
} mlog_session;


extern void mlog_print_session_data(const mlog_session_data * const data);
extern void mlog_add_session_data(const mlog_session_data * const data);

#endif
