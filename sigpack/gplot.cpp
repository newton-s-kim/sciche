#include "gplot.hpp"

#include <stdexcept>
#include <string>

gPlot::gPlot()
{
}

gPlot::~gPlot()
{
}

Value gPlot::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;
    if ("window" == name) {
        if (6 == argc) {
            if (!IS_NUMBER(argv[0]))
                std::runtime_error("number is expected.");
            if (!IS_STRING(argv[1]))
                std::runtime_error("string is expected.");
            if (!IS_NUMBER(argv[2]))
                std::runtime_error("number is expected.");
            if (!IS_NUMBER(argv[3]))
                std::runtime_error("number is expected.");
            if (!IS_NUMBER(argv[4]))
                std::runtime_error("number is expected.");
            if (!IS_NUMBER(argv[5]))
                std::runtime_error("number is expected.");
            m_gplot.window(AS_NUMBER(argv[0]), AS_STRING(argv[1])->chars.c_str(), AS_NUMBER(argv[2]),
                           AS_NUMBER(argv[3]), AS_NUMBER(argv[4]), AS_NUMBER(argv[5]));
        }
        else if (5 == argc) {
            if (!IS_STRING(argv[0]))
                std::runtime_error("string is expected.");
            if (!IS_NUMBER(argv[1]))
                std::runtime_error("number is expected.");
            if (!IS_NUMBER(argv[2]))
                std::runtime_error("number is expected.");
            if (!IS_NUMBER(argv[3]))
                std::runtime_error("number is expected.");
            if (!IS_NUMBER(argv[4]))
                std::runtime_error("number is expected.");
            m_gplot.window(AS_STRING(argv[0])->chars.c_str(), AS_NUMBER(argv[1]), AS_NUMBER(argv[2]),
                           AS_NUMBER(argv[3]), AS_NUMBER(argv[4]));
        }
        else {
            throw std::runtime_error("invalid number of arguments");
        }
    }
    else if ("xlabel" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_STRING(argv[0]))
            std::runtime_error("string is expected.");
        m_gplot.xlabel(AS_STRING(argv[0])->chars.c_str());
    }
    else if ("ylabel" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_STRING(argv[0]))
            std::runtime_error("string is expected.");
        m_gplot.ylabel(AS_STRING(argv[0])->chars.c_str());
    }
    else if ("send2gp" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_STRING(argv[0]))
            std::runtime_error("string is expected.");
        m_gplot.send2gp(AS_STRING(argv[0])->chars.c_str());
    }
    else if ("plot_add" == name) {
        if (4 == argc) {
            if (!IS_COL(argv[0]))
                throw std::runtime_error("col is expected.");
            if (!IS_COL(argv[1]))
                throw std::runtime_error("col is expected.");
            if (!IS_STRING(argv[2]))
                std::runtime_error("string is expected.");
            if (!IS_STRING(argv[3]))
                std::runtime_error("string is expected.");
            m_gplot.plot_add(AS_COL(argv[0])->value, AS_COL(argv[1])->value, AS_STRING(argv[2])->chars,
                             AS_STRING(argv[3])->chars);
        }
        else if (3 == argc) {
            if (!IS_COL(argv[0]))
                throw std::runtime_error("col is expected.");
            if (!IS_COL(argv[1]))
                throw std::runtime_error("col is expected.");
            if (!IS_STRING(argv[2]))
                std::runtime_error("string is expected.");
            m_gplot.plot_add(AS_COL(argv[0])->value, AS_COL(argv[1])->value, AS_STRING(argv[2])->chars);
        }
        else if (2 == argc) {
            if (IS_MAT(argv[0])) {
                if (!IS_STRING(argv[1]))
                    std::runtime_error("string is expected.");
                m_gplot.plot_add(AS_MAT(argv[0])->value, AS_STRING(argv[1])->chars);
            }
            else if (IS_COL(argv[0])) {
                if (!IS_STRING(argv[1]))
                    std::runtime_error("string is expected.");
                m_gplot.plot_add(AS_COL(argv[0])->value, AS_STRING(argv[1])->chars);
            }
            else {
                throw std::runtime_error("mat or vec is expected.");
            }
        }
        else {
            throw std::runtime_error("invalid number of arguments");
        }
    }
    else if ("plot_add_mat" == name) {
        if (2 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            throw std::runtime_error("mat is expected.");
        if (!IS_STRING(argv[1]))
            std::runtime_error("string is expected.");
        m_gplot.plot_add_mat(AS_MAT(argv[0])->value, AS_STRING(argv[1])->chars);
    }
    else if ("plot_show" == name) {
        m_gplot.plot_show();
    }
    else if ("image" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            std::runtime_error("matrix is expected.");
        m_gplot.image(AS_MAT(argv[0])->value);
    }
    else if ("figure" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_NUMBER(argv[0]))
            std::runtime_error("number is expected.");
        m_gplot.figure(AS_NUMBER(argv[0]));
    }
    else if ("grid_on" == name) {
        m_gplot.grid_on();
    }
    else {
        throw std::runtime_error("non-existent method");
    }
    return ret;
}
