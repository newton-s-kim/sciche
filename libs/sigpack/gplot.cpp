#include "gplot.hpp"

#include <stdexcept>
#include <string>

namespace sce {
static Value gplot_window(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    gPlot* gplot = (gPlot*)obj;
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
        gplot->m_gplot.window(AS_NUMBER(argv[0]), AS_STRING(argv[1])->chars.c_str(), AS_NUMBER(argv[2]),
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
        gplot->m_gplot.window(AS_STRING(argv[0])->chars.c_str(), AS_NUMBER(argv[1]), AS_NUMBER(argv[2]),
                              AS_NUMBER(argv[3]), AS_NUMBER(argv[4]));
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return NIL_VAL;
}

static Value gplot_xlabel(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    gPlot* gplot = (gPlot*)obj;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_STRING(argv[0]))
        std::runtime_error("string is expected.");
    gplot->m_gplot.xlabel(AS_STRING(argv[0])->chars.c_str());
    return NIL_VAL;
}

static Value gplot_ylabel(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    gPlot* gplot = (gPlot*)obj;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_STRING(argv[0]))
        std::runtime_error("string is expected.");
    gplot->m_gplot.ylabel(AS_STRING(argv[0])->chars.c_str());
    return NIL_VAL;
}

static Value gplot_send2gp(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    gPlot* gplot = (gPlot*)obj;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_STRING(argv[0]))
        std::runtime_error("string is expected.");
    gplot->m_gplot.send2gp(AS_STRING(argv[0])->chars.c_str());
    return NIL_VAL;
}

static Value gplot_plot_add(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    gPlot* gplot = (gPlot*)obj;
    if (4 == argc) {
        if (!IS_STRING(argv[2]))
            std::runtime_error("string is expected.");
        if (!IS_STRING(argv[3]))
            std::runtime_error("string is expected.");
        if (IS_COL(argv[0])) {
            if (IS_COL(argv[1])) {
                gplot->m_gplot.plot_add(AS_COL(argv[0])->value, AS_COL(argv[1])->value, AS_STRING(argv[2])->chars,
                                        AS_STRING(argv[3])->chars);
            }
            else if (IS_ROW(argv[1])) {
                gplot->m_gplot.plot_add(AS_COL(argv[0])->value, AS_ROW(argv[1])->value, AS_STRING(argv[2])->chars,
                                        AS_STRING(argv[3])->chars);
            }
            else {
                throw std::runtime_error("vec or rowvec is expected.");
            }
        }
        else if (IS_ROW(argv[0])) {
            if (IS_COL(argv[1])) {
                gplot->m_gplot.plot_add(AS_ROW(argv[0])->value, AS_COL(argv[1])->value, AS_STRING(argv[2])->chars,
                                        AS_STRING(argv[3])->chars);
            }
            else if (IS_ROW(argv[1])) {
                gplot->m_gplot.plot_add(AS_ROW(argv[0])->value, AS_ROW(argv[1])->value, AS_STRING(argv[2])->chars,
                                        AS_STRING(argv[3])->chars);
            }
            else {
                throw std::runtime_error("vec or rowvec is expected.");
            }
        }
        else {
            throw std::runtime_error("vec or rowvec is expected.");
        }
    }
    else if (3 == argc) {
        if (!IS_STRING(argv[2]))
            std::runtime_error("string is expected.");
        if (IS_COL(argv[0])) {
            if (IS_COL(argv[1])) {
                gplot->m_gplot.plot_add(AS_COL(argv[0])->value, AS_COL(argv[1])->value, AS_STRING(argv[2])->chars);
            }
            else if (IS_ROW(argv[1])) {
                gplot->m_gplot.plot_add(AS_COL(argv[0])->value, AS_ROW(argv[1])->value, AS_STRING(argv[2])->chars);
            }
            else {
                throw std::runtime_error("col is expected.");
            }
        }
        else if (IS_ROW(argv[0])) {
            if (IS_COL(argv[1])) {
                gplot->m_gplot.plot_add(AS_ROW(argv[0])->value, AS_COL(argv[1])->value, AS_STRING(argv[2])->chars);
            }
            else if (IS_ROW(argv[1])) {
                gplot->m_gplot.plot_add(AS_ROW(argv[0])->value, AS_ROW(argv[1])->value, AS_STRING(argv[2])->chars);
            }
            else {
                throw std::runtime_error("col is expected.");
            }
        }
        else {
            throw std::runtime_error("vec or rowvec is expected.");
        }
    }
    else if (2 == argc) {
        if (IS_MAT(argv[0])) {
            if (!IS_STRING(argv[1]))
                std::runtime_error("string is expected.");
            gplot->m_gplot.plot_add(AS_MAT(argv[0])->value, AS_STRING(argv[1])->chars);
        }
        else if (IS_COL(argv[0])) {
            if (!IS_STRING(argv[1]))
                std::runtime_error("string is expected.");
            gplot->m_gplot.plot_add(AS_COL(argv[0])->value, AS_STRING(argv[1])->chars);
        }
        else {
            throw std::runtime_error("mat or vec is expected.");
        }
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return NIL_VAL;
}

static Value gplot_plot_add_mat(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    if (!IS_STRING(argv[1]))
        std::runtime_error("string is expected.");
    gPlot* gplot = (gPlot*)obj;
    gplot->m_gplot.plot_add_mat(AS_MAT(argv[0])->value, AS_STRING(argv[1])->chars);
    return NIL_VAL;
}

static Value gplot_plot_show(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    gPlot* gplot = (gPlot*)obj;
    gplot->m_gplot.plot_show();
    return NIL_VAL;
}

static Value gplot_image(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        std::runtime_error("matrix is expected.");
    gPlot* gplot = (gPlot*)obj;
    gplot->m_gplot.image(AS_MAT(argv[0])->value);
    return NIL_VAL;
}

static Value gplot_figure(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_NUMBER(argv[0]))
        std::runtime_error("number is expected.");
    gPlot* gplot = (gPlot*)obj;
    gplot->m_gplot.figure(AS_NUMBER(argv[0]));
    return NIL_VAL;
}

static Value gplot_grid_on(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    gPlot* gplot = (gPlot*)obj;
    gplot->m_gplot.grid_on();
    return NIL_VAL;
}

// clang-format off
std::map<std::string, NativeObjectBoundFn> s_gplot_apis = {
    {"window", gplot_window},
    {"xlabel", gplot_xlabel},
    {"ylabel", gplot_ylabel},
    {"send2gp", gplot_send2gp},
    {"plot_add", gplot_plot_add},
    {"plot_add_mat", gplot_plot_add_mat},
    {"plot_show", gplot_plot_show},
    {"image", gplot_image},
    {"figure", gplot_figure},
    {"grid_on", gplot_grid_on}
};

std::map<std::string, NativeObjectBoundProperty> s_gplot_properties;
// clang-format on

gPlot::gPlot() : NativeObject(s_gplot_apis, s_gplot_properties)
{
}

gPlot::~gPlot()
{
}
} // namespace sce
