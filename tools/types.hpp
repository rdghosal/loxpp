#include <cstdint>

enum class ExitCode : uint8_t {
    OK = 0,
    IO_ERROR = 63,
    USER_ERROR = 64,
    RUNTIME_ERROR = 65,
};
