#include <cmath>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <streambuf>

#include "readline.hpp"
#include "vm.hpp"

#include "sigpack/sp.hpp"

void help(std::string programme)
{
    std::cout << "USAGE: " << programme << "[OPTION].. filepath" << std::endl;
    std::cout << "OPTION:" << std::endl;
    std::cout << "  -h, --help  prints this screen" << std::endl;
    std::cout << "  -m, --use-virtual-machine  use Byte code machine" << std::endl;
}

static Value printLnNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    ValueUtil util;
    for (int idx = 0; idx < argc; idx++) {
        util.print(args[idx]);
    }
    printf("\n");
    return 0;
}

static Value exitNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    int n = 0;
    if (1 == argc && IS_NUMBER(args[0]))
        n = args[0];
    exit(AS_NUMBER(n));
}

/*
static Value sinNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(sin(AS_NUMBER(args[0])));
    }
    return NUMBER_VAL(0);
}

static Value cosNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(cos(AS_NUMBER(args[0])));
    }
    return NUMBER_VAL(0);
}

static Value tanNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(tan(AS_NUMBER(args[0])));
    }
    return NUMBER_VAL(0);
}

static Value sqrtNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        double v = AS_NUMBER(args[0]);
        if (0 <= v) {
            return NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
        }
        else {
            ObjComplex* cmplx = factory->newComplex(std::complex<double>(0, sqrt(-AS_NUMBER(args[0]))));
            return OBJ_VAL(cmplx);
        }
    }
    return NUMBER_VAL(0);
}

static Value absNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(abs(AS_NUMBER(args[0])));
    }
    else if (IS_COMPLEX(args[0])) {
        return NUMBER_VAL(abs(AS_COMPLEX(args[0])->value));
    }
    return NUMBER_VAL(0);
}

static Value phaseNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    (void)args;
    if (IS_COMPLEX(args[0])) {
        return NUMBER_VAL(arg(AS_COMPLEX(args[0])->value));
    }
    return NUMBER_VAL(0);
}
*/

static Value vecNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;
    return OBJ_VAL(factory->newCol());
}

static Value rowVecNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;
    return OBJ_VAL(factory->newRow());
}

static Value matNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;
    return OBJ_VAL(factory->newMat());
}

/*
static Value fir_filtNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;

}
*/

int main(int argc, char const* argv[])
{
    int result = 0;
    std::string filepath;

    const char* short_opts = "hm";
    const option long_opts[] = {{"help", no_argument, nullptr, 'h'}, {"version", no_argument, nullptr, 'v'}};

    while (true) {
        const auto opt = getopt_long(argc, (char* const*)argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;
        switch (opt) {
        case 'h':
            help(argv[0]);
            return 0;
            break;
        case 'v':
            std::cout << "1.0" << std::endl;
            break;
        default:
            std::cerr << "unhandled option " << opt << std::endl;
            return 1;
        }
    }

    if (optind == argc - 1)
        filepath = argv[optind];

    VM vm;
    vm.defineNative("println", printLnNative);
    vm.defineNative("exit", exitNative);
    /*
    vm.defineNative("sin", sinNative);
    vm.defineNative("cos", cosNative);
    vm.defineNative("tan", tanNative);
    vm.defineNative("sqrt", sqrtNative);
    vm.defineNative("abs", absNative);
    vm.defineNative("phase", phaseNative);
    */
    vm.defineNative("vec", vecNative);
    vm.defineNative("rowvec", rowVecNative);
    vm.defineNative("mat", matNative);
    vm.defineNative("FIR_filt", firFiltNative);
    vm.defineNative("IIR_filt", iirFiltNative);
    vm.defineNative("fir1", fir1Native);
    vm.defineNative("Delay", delayNative);
    vm.defineNumber("pi", M_PI);
    std::string prompt;
    prompt = ">> ";
    // std::unique_ptr<ByteCodeMachine> runner = std::make_unique<ByteCodeMachine>();
    if (filepath.empty()) {
        std::string input;
        ReadLine rl;
        while (true) {
            input = rl.read(prompt);
            result = vm.interpret(input.c_str());
        }
    }
    else {
        std::ifstream ifs{filepath};
        if (!ifs.is_open()) {
            std::cerr << "Error: Cannot open file '" << argv[1] << "' to read!\n";
            return 1;
        }

        std::string source{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

        result = vm.interpret(source.c_str());

#ifdef CLOXX_GC_DEBUG
        LOX_ASSERT(LoxObject::instanceCount() == 0);
        LOX_ASSERT(Traceable::instanceCount() == 0);
#endif
    }
    return result;
}
