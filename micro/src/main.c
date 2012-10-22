#include "common.h"

/**
 * Entry function to the application, calles setup() then loop() repeatedly
 * @return always 0 (but never should return)
 */
int main(void) {
    setup();
    while(1) {
        loop();
    }
    return 0;
}
