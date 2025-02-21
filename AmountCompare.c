#include "hookapi.h"

#define DONE(x) accept(SBUF(x), __LINE__)
#define NOPE(x) rollback(SBUF(x), __LINE__)

int64_t hook(uint32_t reserved ) {
    _g(1,1);    

    uint8_t account[20];
    otxn_field(SBUF(account), sfAccount);

    uint8_t hook_acc[20];
    hook_account(SBUF(hook_acc));

    if(BUFFER_EQUAL_20(account, hook_acc)) 
        DONE("Outgoing transaction.");

    uint8_t amount[8];
    otxn_field(SBUF(amount), sfAmount);
    uint64_t otxn_drops = AMOUNT_TO_DROPS(amount);
    uint64_t amount_xfl = float_set(-6, otxn_drops);
    
    // Reject if amount is less than 1 XAH
    if(float_compare(amount_xfl, 6089866696204910592, COMPARE_LESS) == 1)
        NOPE("You cannot send less than 1 XAH.");

    DONE("Payment Successful.");
    return 0;
}
