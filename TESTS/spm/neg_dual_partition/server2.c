#include <string.h>
#include "cmsis_os2.h"
#include "spm_server.h"
#include "psa_part2_partition.h"


void server_main2(void *ptr)
{
    uint32_t  signals   = 0;
    psa_msg_t msg       = {0};

    while (1) {
        signals = psa_wait_any(PSA_WAIT_BLOCK);
        MBED_ASSERT(signals == DUMMY_MSK);
        PSA_UNUSED(signals);

        psa_get(DUMMY_MSK, &msg);
        switch (msg.type) {
            case PSA_IPC_MSG_TYPE_CALL:
            case PSA_IPC_MSG_TYPE_CONNECT:
            case PSA_IPC_MSG_TYPE_DISCONNECT:
                psa_end(msg.handle, PSA_SUCCESS);
                break;
            default:
            SPM_PANIC("Invalid msg type");
        }
    }
}
