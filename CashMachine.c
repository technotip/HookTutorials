/*
*  Tutorial: https://www.youtube.com/watch?v=njoGFrt5CFY
*/

#include "hookapi.h"

#define DONE(x) accept(SBUF(x), __LINE__)
#define NOPE(x) rollback(SBUF(x), __LINE__)

int64_t hook(uint32_t reserved ) {

    uint8_t account[20];
    otxn_field(SBUF(account), sfAccount);

    uint8_t hook_acc[20];
    hook_account(SBUF(hook_acc));

    if(BUFFER_EQUAL_20(account, hook_acc)) 
        DONE("Outgoing transaction.");

    uint8_t tt = otxn_type();

    if(tt == ttPAYMENT) {
        uint8_t amount[8];
        otxn_field(SBUF(amount), sfAmount);
        uint64_t otxn_drops = AMOUNT_TO_DROPS(amount);
        uint64_t amount_xfl = float_set(-6, otxn_drops);        

        uint64_t amt_in_state = 0;
        state(SVAR(amt_in_state), SBUF(account));

        uint64_t total = float_sum(amount_xfl, amt_in_state);
        state_set(SVAR(total), SBUF(account));

        DONE("Successful Deposite.");
    }

    if(tt == ttINVOKE) {
        uint64_t amt_in_state = 0;
        if(state(SVAR(amt_in_state), SBUF(account)) != 8)
            NOPE("No Amount To Withdraw.");

        uint64_t toWithdraw = 0;
        if(otxn_param(SVAR(toWithdraw), "WD", 2) != 8)
            NOPE("You forgot to pass the amount to withdraw.");

        if(float_compare(toWithdraw, amt_in_state, COMPARE_LESS | COMPARE_EQUAL) != 1)
            NOPE("You cannot withdraw more than what you have in the treasury.");

        uint64_t total = float_sum(amt_in_state, float_negate(toWithdraw));

        if(total == 0)
            state_set(0, 0, SBUF(account));
        else
            state_set(SVAR(total), SBUF(account));

        etxn_reserve(1);
        uint8_t txn[PREPARE_PAYMENT_SIMPLE_SIZE];
        PREPARE_PAYMENT_SIMPLE(txn, float_int(toWithdraw, 6, 1), account, 0, 0);

        uint8_t emithash[32];
        if(emit(SBUF(emithash), SBUF(txn)) != 32)
            NOPE("Failed, please try again later.");

        DONE("Amount Forwarded Successfully.");        
    }

    _g(1,1); 
    return 0;
}
