#pragma once

#include "streamer.h"
#include "token.h"

#include <map>
#include <stack>
#include <string>

namespace sciche {
class scanner {
private:
    token m_token;
    std::stack<token> m_backup;
    size_t m_line_no;
    size_t m_line_pos;
    std::map<std::string, token::tokenType> m_reserved;
    void scanString(void);
    void scanSymbol(char ch);
    void scanDecimal(char ch);
    void scanNumber(char ch);
    void scanHex(void);
    void scanOct(void);
    void consumeToEol(void);
    void consumeToEoc(void);
    streamer* m_streamer;

public:
    scanner();
    ~scanner();
    void set_streamer(streamer* streamer);
    streamer* get_streamer(void);
    token scan(void);
    void unscan(void);
};
}; // namespace sciche
