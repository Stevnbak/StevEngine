#pragma once
#include <uuidv7.h>
#include <string>
#include <stdint.h>

namespace StevEngine {
    namespace Utilities {
        class ID {
            public:
                ID ();
                ID (const ID& other);
                ID (uint8_t* raw);
                ID (std::string string);
                std::string GetString() { return string; }
                ID& operator=(const ID& other);
                bool operator==(const ID& other) const;
                bool operator<(const ID& other) const;
                bool operator() (const ID& lhs, const ID& rhs) const;
                bool IsNull();
                static ID empty;
            private:
                uint8_t raw[16];
                std::string string;
        };
    }
}