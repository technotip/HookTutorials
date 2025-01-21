/*
* https://www.youtube.com/watch?v=1S704xZVxlY
*/

#include "hookapi.h"

#define DONE(x) accept(SBUF(x), __LINE__)
#define NOPE(x) rollback(SBUF(x), __LINE__)

int64_t hook(uint32_t reserved ) {

  // uint8_t master_acc[20] = { 0xB6U, 0x75U, 0xB3U, 0xDEU, 0x01U, 0x0AU, 0x7CU, 0x7AU, 0x2DU, 0xF6U, 0x55U, 0xC5U, 0x4CU, 0x28U, 0x4AU, 0x21U, 0x13U, 0xFFU, 0xD7U, 0x6BU };

    uint8_t master_acc[20];
    if (otxn_param(SBUF(master_acc), "M", 1) != 20)
        NOPE("Misconfigured. Destination account not passed as HookParameter.");

    uint64_t amount_param;
    if (otxn_param(SVAR(amount_param), "A", 1) != 8)
        NOPE("Misconfigured. Amount not passed as HookParameter.");        

    uint8_t hook_acc[20];
    hook_account(SBUF(hook_acc));

    uint8_t account[20];
    otxn_field(SBUF(account), sfAccount);

    if(BUFFER_EQUAL_20(hook_acc, account))
        DONE("Outgoing Transaction");

    uint8_t amount[8];
    if(otxn_field(SBUF(amount), sfAmount) != 8)
        DONE("Non-XAH Transaction, accepted!");

   uint64_t otxn_drops = AMOUNT_TO_DROPS(amount);
   int64_t amount_xfl = float_set(-6, otxn_drops);

   if (float_compare(amount_xfl, amount_param, COMPARE_LESS) == 1)
        NOPE("Insufficient Balance Transferred.");

    etxn_reserve(1);
    uint8_t txn[PREPARE_PAYMENT_SIMPLE_SIZE];
    PREPARE_PAYMENT_SIMPLE(txn, float_int(amount_param, 6, 1), master_acc, 0, 0);

    uint8_t emithash[32];
    if(emit(SBUF(emithash), SBUF(txn)) != 32)
        NOPE("Failed, please try again later.");

    DONE("Amount Forwarded Successfully.");
    _g(1,1);
    return 0;
}
