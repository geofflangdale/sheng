#include "common_defs.h"
#include <tuple>
#include <vector>
typedef unsigned char u8;
typedef unsigned int u32;

void set_byte_at_offset(m128 & in, u32 offset, u8 content) {
    union {
        m128 sse;
        u8 bytes[16];
    } u;
    u.sse = in;
    u.bytes[offset] = content;
    in = u.sse;
}

u8 get_byte_at_offset(m128 in, u32 offset) {
    union {
        m128 sse;
        u8 bytes[16];
    } u;
    u.sse = in;
    return u.bytes[offset];
}

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

struct Sheng {
    typedef m128 State;
    m128 transitions[256];
    State start_state;

    Sheng(std::vector<std::tuple<u32, u32, u8>> & trans_vec, u8 start_state_, u8 default_state) {
        // fill all transitions with default state
        for (u32 i = 0; i < 256; ++i) {
            transitions[i] = _mm_set1_epi8(default_state);
        }
        // fill in state transition for slot 'from' to point to 'to' for our character transition c
        for (auto p : trans_vec) {
            u32 from, to;
            u8 c;
            std::tie(from, to, c) = p;
            set_byte_at_offset(transitions[c], from, to);
        }
        start_state =  _mm_set1_epi8(start_state_); // put everyone into start state - why not?
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
            s = _mm_shuffle_epi8(transitions[c1], s);
            s = _mm_shuffle_epi8(transitions[c2], s);
            s = _mm_shuffle_epi8(transitions[c3], s);
            s = _mm_shuffle_epi8(transitions[c4], s);
            s = _mm_shuffle_epi8(transitions[c5], s);
            s = _mm_shuffle_epi8(transitions[c6], s);
            s = _mm_shuffle_epi8(transitions[c7], s);
            s = _mm_shuffle_epi8(transitions[c8], s);
        }
        for (; i < len; ++i) {
            s = _mm_shuffle_epi8(transitions[data[i]], s);
        }
        return s;
    }

};
