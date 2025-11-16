#include "console_streamer.h"
#include "macros.h"

#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>

namespace sciche {
console_streamer::console_streamer()
{
    m_prompt = "> ";
    m_pos = 0;
}

console_streamer::~console_streamer()
{
}

void console_streamer::fetch(void)
{
    char* line_read;
    m_pos = 0;

    do {
        // Display a prompt and read a line
        line_read = readline(m_prompt.c_str());
    } while (line_read && strlen(line_read));

    // Add the line to history if it's not empty
    if (line_read) {
        LOG("line_read: 0x%x", line_read[0]);
        if (isprint(*line_read)) {
            add_history(line_read);
        }
    }
    else {
        exit(0);
    }

    // Process the input line
    m_line = line_read;

    //....

    // Free the memory allocated by readline
    free(line_read);

    /*
    if (m_line == "exit") {
        std::cout << "Exiting." << std::endl;
        exit(0);
    }
    */
}

char console_streamer::getch(void)
{
    if (m_pos + 1 >= m_line.size())
        fetch();
    return m_line[m_pos++];
}

void console_streamer::unch(void)
{
    m_pos--;
}

size_t console_streamer::pos(void)
{
    return m_pos;
}

void console_streamer::set_prompt(std::string str)
{
    m_prompt = str;
}
}; // namespace sciche
