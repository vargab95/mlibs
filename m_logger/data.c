#include "stdio.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "data.h"

static mlog_session sessions[65536];

void mlog_print_session_data(const mlog_session_data *const data)
{
    printf("Client id: %d\n", data->id.separated.client_id);
    printf("Session id: %d\n", data->id.separated.session_id);
    printf("Timestamp: %d\n", data->timestamp);
    printf("Severity: %d\n", data->data.severity);
    printf("Message: %s\n", data->data.message);
}

void mlog_add_session_data(const mlog_session_data *data)
{
    mlog_session *session = &sessions[data->id.session_id];
    session->last_index++;
    session->data[session->last_index] = *data;
    session->data[session->last_index].timestamp = time(0);
    if (session->last_index >= 9)
    {
        char path[256];
        int outputFile;
        sprintf(path, "data/%04x%08x%08x", data->id.session_id, session->data[0].timestamp,
                session->data[session->last_index].timestamp);
        outputFile = open(path, O_WRONLY | O_CREAT | O_TRUNC);
        if (outputFile <= 0)
        {
            printf("Error while writing file");
        }
        else
        {
            write(outputFile, session->data, sizeof(session->data));
            close(outputFile);
        }
        session->last_index = -1;
    }
}
