#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "sheng.h"

const int BLOCK_SIZE = 16384;
const int REPEATS = 100000;

using namespace std;
int main(UNUSED int argc, UNUSED char * argv[]) {
    vector<tuple<u32, u32, u8>> transitions;
    // TODO: make a DFA by filling in some transitions, just to show doing something fun
 
    u8 * data = new u8[BLOCK_SIZE];
    for (u32 i = 0; i < BLOCK_SIZE; i++) {
        data[i] = rand() % 256;
    }
    const u8 * char_data = (const u8 *)"Now is the time for all good men to come to the aid of the party.";
    Sheng sh(transitions, 1, 0); 
    
    Sheng::State s = sh.apply(char_data, strlen((const char *)char_data), sh.start_state);

    for (u32 i = 0; i < REPEATS; i++) {
        s = sh.apply(data, BLOCK_SIZE, s);
    }

    union {
        m128 sse;
        u8 bytes[16];
    } u;
    u.sse = s;
    cout << "final state: " << (int)u.bytes[0] << "bytes scanned: " << BLOCK_SIZE*REPEATS << "\n";
}
