#include "ID.hpp"
#include <stdio.h>
#include <sys/random.h>
#include <time.h>

uint8_t last[16];
namespace StevEngine {
    namespace Utilities {
        uint8_t e[16];
        ID ID::empty = ID(e);

        ID::ID () {
            uint8_t rand_bytes[10];
            timespec tp;
            clock_gettime(CLOCK_REALTIME, &tp);
            uint64_t unix_ts_ms = (uint64_t)tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
            getentropy(rand_bytes, 10);
            
            uuidv7_generate(
                raw,
                unix_ts_ms,
                rand_bytes,
                last
            );

            char text[37];
            uuidv7_to_string(raw, text);
            string = std::string(text);
        }
        ID::ID (const ID& other) {
            string = other.string;
            for(int i = 0; i < 16; i++) 
                raw[i] = other.raw[i];
        }
        ID::ID (uint8_t* raw) {
            for(int i = 0; i < 16; i++) 
                this->raw[i] = raw[i];
        }
        ID::ID (std::string string) {
            this->string = string;
            uuidv7_from_string(string.c_str(), raw);
        }

        bool ID::IsNull() {
            return (this->string == empty.string); 
        }
        bool ID::operator==(const ID& other) const {
            return (string == other.string);
        }
        bool ID::operator<(const ID& other) const {
            return (string < other.string);
        }
        bool ID::operator() (const ID& lhs, const ID& rhs) const {
            return lhs.string < rhs.string;
        }
    }
}