/*
*  Tutorial: https://www.youtube.com/watch?v=qWm9HROcCJ8
*/

#include "hookapi.h"
#include <stdint.h>

#define DONE(x) accept(SBUF(x), __LINE__)
#define NOPE(x) rollback(SBUF(x), __LINE__)

int64_t hook(uint32_t reserved ) {

    uint32_t count = 0;

    uint8_t account[20];
    otxn_field(SBUF(account), sfAccount);

    uint8_t destination[20];
    otxn_field(SBUF(destination), sfDestination);    

    uint8_t hook_acc[20];
    hook_account(SBUF(hook_acc));

    if(BUFFER_EQUAL_20(hook_acc, account)) {
        uint8_t send_ns[32] = { 0x2AU, 0x8EU, 0xAAU, 0xCAU, 0xEDU, 0xC6U, 0x63U, 0xBCU, 0x49U, 0x9DU, 0x8BU, 0x66U, 0x9EU, 0x4CU, 0x7BU, 0xFFU, 0x33U, 0x35U, 0xEFU, 0x62U, 0x2DU, 0x20U, 0x6FU, 0x42U, 0x83U, 0x3DU, 0xE3U, 0x40U, 0x8EU, 0x3EU, 0x2AU, 0x80U };

        state_foreign(SVAR(count), SBUF(destination), SBUF(send_ns), SBUF(hook_acc));
        ++count;
        state_foreign_set(SVAR(count), SBUF(destination), SBUF(send_ns), SBUF(hook_acc));
        TRACEVAR(count);
    } else {
        state(SVAR(count), SBUF(account));
        ++count;
        state_set(SVAR(count), SBUF(account));
        TRACEVAR(count);
    }

    DONE("Payment Successful.");

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}
