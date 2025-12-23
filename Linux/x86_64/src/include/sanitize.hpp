#ifndef SANITIZE_HPP
#define SANITIZE_HPP
// 3rd Party Libraries
#include <sanitizer/lsan_interface.h>

// Platform Libraries

// Project Libraries

// Standard Libraries

#ifdef NEO_LSAN_ENABLED
#define NEO_LSAN_DISABLE __lsan_disable()
#define NEO_LSAN_ENABLE  __lsan_enable()
#else
// Expand to nothing in non-sanitize builds
#define NEO_LSAN_DISABLE do {} while(0)
#define NEO_LSAN_ENABLE  do {} while(0)
#endif // NEO_LSAN_ENABLED

#endif // SANITIZE_HPP
