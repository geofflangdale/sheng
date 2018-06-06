#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "sheng.h"
#include "basicdfa.h"
#include <chrono>

const int BLOCK_SIZE = 16384;
const int REPEATS = 100000;

using namespace std;

void demo_bdfa(BasicDFA & sh) {
    const u8 * char_data = (const u8 *)"Now is the time for all good men to come to the aid of the party. XXY";
    for (u32 i = 0; i < strlen((const char *)char_data); ++i) {
        BasicDFA::State s = sh.apply(char_data, i, sh.start_state);
        cout << i << "/" << (int)s << "  ";
        if (!((i + 1) % 10)) {
            cout << "\n";
        }   
    }
    cout << "\n";
}

never_inline void performance_test_bdfa(BasicDFA & sh, u8 * data) {
    BasicDFA::State s = sh.start_state;
    auto start = std::chrono::steady_clock::now();
    for (u32 i = 0; i < REPEATS; i++) {
        s = sh.apply(data, BLOCK_SIZE, s);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> secs_clock = end - start;
    double secs = secs_clock.count();
    size_t bytes_scanned = BLOCK_SIZE * REPEATS;
    cout << "final state: " << (int)s
         << " bytes scanned: " << bytes_scanned
         << " seconds: " << secs << "\n";
    cout << " bytes per ns " << (bytes_scanned/secs)/1000000000.0 << "\n";
}

void demo(Sheng & sh) {
    const u8 * char_data = (const u8 *)"Now is the time for all good men to come to the aid of the party. XXY";
    for (u32 i = 0; i < strlen((const char *)char_data); ++i) {
        Sheng::State s = sh.apply(char_data, i, sh.start_state);
        cout << i << "/" << (int)get_byte_at_offset(s, 0) << "  ";
        if (!((i + 1) % 10)) {
            cout << "\n";
        }   
    }
    cout << "\n";
}

never_inline void performance_test(Sheng & sh, u8 * data) {
    Sheng::State s = sh.start_state;
    auto start = std::chrono::steady_clock::now();
    for (u32 i = 0; i < REPEATS; i++) {
        s = sh.apply(data, BLOCK_SIZE, s);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> secs_clock = end - start;
    double secs = secs_clock.count();
    size_t bytes_scanned = BLOCK_SIZE * REPEATS;
    cout << "final state: " << (int)get_byte_at_offset(s, 0)
         << " bytes scanned: " << bytes_scanned
         << " seconds: " << secs << "\n";
    cout << " bytes per ns " << (bytes_scanned/secs)/1000000000.0 << "\n";
}

int main(UNUSED int argc, UNUSED char * argv[]) {
    vector<tuple<u32, u32, u8>> transitions;

    // detect the regex /good.*party/s by hand-building the DFA
    transitions.push_back(make_tuple(1,2,'g'));
    transitions.push_back(make_tuple(2,3,'o'));
    transitions.push_back(make_tuple(3,4,'o'));
    transitions.push_back(make_tuple(4,5,'d'));
    for (u32 i = 0; i < 256; i++) {
        if (i != 'p') {
            transitions.push_back(make_tuple(5,5,(u8)i));
        } else {
            transitions.push_back(make_tuple(5,6,'p'));
        }
    }
    transitions.push_back(make_tuple(6,7,'a'));
    transitions.push_back(make_tuple(7,8,'r'));
    transitions.push_back(make_tuple(8,9,'t'));
    transitions.push_back(make_tuple(9,10,'y'));

    u8 * data = new u8[BLOCK_SIZE];
    for (u32 i = 0; i < BLOCK_SIZE; i++) {
        data[i] = rand() % 256;
    }

    cout << "\nSheng\n";
    Sheng sh(transitions, 1, 1); 
    demo(sh);
    performance_test(sh, data);

    cout << "\nBasic DFA\n";
    BasicDFA bdfa(transitions, 1, 1); 
    demo_bdfa(bdfa);
    performance_test_bdfa(bdfa, data);
    cout << "\n";
}
