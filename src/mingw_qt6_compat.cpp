// MinGW compatibility for Qt6 entry point
// Provides missing symbols that Qt6EntryPoint expects

extern "C" {
    // Provide the missing argc/argv symbols that Qt6 expects
    int __argc = 0;
    char** __argv = nullptr;
    
    // These symbols are referenced by Qt6EntryPoint but not defined in MinGW
    extern int* __p___argc() { return &__argc; }
    extern char*** __p___argv() { return &__argv; }
}
