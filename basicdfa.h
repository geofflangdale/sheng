#ifndef BASIC_DFA_H
#define BASIC_DFA_H
#include "common_defs.h"
#include <tuple>

struct BasicDFA {
    typedef u8 State;
    u8 transitions[16][256];
    State start_state;
    BasicDFA(std::vector<std::tuple<u32, u32, u8>> & trans_vec, u8 start_state_, u8 default_state) {
        for (u32 i = 0; i < 16; ++i) {
            for (u32 j = 0; j < 256; ++j) {
                transitions[i][j] = default_state;
            }
        }
        for (auto p : trans_vec) {
            u32 from, to;
            u8 c;
            std::tie(from, to, c) = p;
            transitions[from][c] = to;
        }
        start_state = start_state_;
    }
    State apply(const u8 * data, size_t len, State s) {
        size_t i = 0;
        for (; i+7 < len; i+=8) {
            u8 c1 = data[i+0];
            u8 c2 = data[i+1];
            u8 c3 = data[i+2];
            u8 c4 = data[i+3];
            u8 c5 = data[i+4];
            u8 c6 = data[i+5];
            u8 c7 = data[i+6];
            u8 c8 = data[i+7];
            s = transitions[s][c1];
            s = transitions[s][c2];
            s = transitions[s][c3];
            s = transitions[s][c4];
            s = transitions[s][c5];
            s = transitions[s][c6];
            s = transitions[s][c7];
            s = transitions[s][c8];
        }
        for (; i < len; ++i) {
            s = transitions[s][data[i]];
        }
        return s;
    }
};

#endif
