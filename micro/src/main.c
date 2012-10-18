#include "common.h"

/**
 * Entry script to the application, calles setup() then loop() repeatedly
 * @return always 0
 */
int main(void) {
    setup();
    while(1) {
        loop();
    }
    return 0;
}
