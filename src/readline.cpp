#include "readline.hpp"

#include <readline/history.h>
#include <readline/readline.h>

std::string ReadLine::read(std::string prompt)
{
    std::string str;
    char* input = readline(prompt.c_str());
    add_history(input);
    str = input;
    delete input;
    return str;
}
