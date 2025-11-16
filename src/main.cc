#include "compiler.h"
#include "console_streamer.h"
#include "vm.h"

int main()
{
    sciche::virtual_machine vm;
    sciche::console_streamer streamer;
    sciche::compiler compiler(&vm);

    while (true) {
        compiler.compile(&streamer);
        vm.run();
    }

    return 0;
}
