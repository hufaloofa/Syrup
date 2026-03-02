#include <emscripten.h>

#include "../include/syrup.h"

// Call Module.print directly from C, bypassing Emscripten's printChar buffer
// (which only flushes on '\n'). This lets `print` work without a trailing newline.
EM_JS(void, web_print, (const char* str), {
    Module._printNoNewline(UTF8ToString(str));
});

EM_JS(void, web_printerr, (const char* str), {
    err(UTF8ToString(str));
});

EMSCRIPTEN_KEEPALIVE
void run_source(const char* source) {
    run((char*)source);
}
