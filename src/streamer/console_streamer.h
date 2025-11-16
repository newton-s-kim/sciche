#pragma once

#include "streamer.h"

#include <string>

namespace sciche {
class console_streamer : public streamer {
private:
    size_t m_pos;
    std::string m_line;
    void fetch(void);
    std::string m_prompt;

public:
    console_streamer();
    ~console_streamer();
    char getch(void);
    void unch(void);
    size_t pos(void);
    void set_prompt(std::string str);
};
}; // namespace sciche
