/* Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <string.h>
#include "unity.h"
#include "spm_panic.h"
#include "spm_server.h"
#include "spm_client.h"
#include "spm_panic.h"
#include "psa_part1_partition.h"
#include "psa_client_common.h"

#define MINOR_VER 5

void server_main1(void *ptr)
{
    psa_msg_t msg = {0};
    uint32_t signals = 0;

    while (true) {
        signals = psa_wait_any(PSA_WAIT_BLOCK);
        if (signals & PART1_CALL_NON_EXISTS_EXTERN_SFID_MSK) {
            psa_get(PART1_CALL_NON_EXISTS_EXTERN_SFID_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    psa_end(msg.handle, PSA_SUCCESS);
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    psa_connect(PART2_CALL_INSIDE_PARTITION, MINOR_VER);
                    TEST_FAIL_MESSAGE("server_call_sfid_without_extern_sfid negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_call_sfid_without_extern_sfid msg type failure");
                }
            }
        } else {
            SPM_ASSERT(false);
        }
    }
}
