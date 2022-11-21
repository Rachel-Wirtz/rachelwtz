#ifndef RACHELWTZ__CORE__DEBUG__HPP
#define RACHELWTZ__CORE__DEBUG__HPP

#include "rw.hpp"

#include <stdexcept>

RACHELWTZ_CORE_BEGIN

class assertion_failure : public std::exception {
public:
    using std::exception::exception;
};

RACHELWTZ_CORE_END

#define RACHELWTZ_ASSERT(x)     { if (!(x)) { throw ::RACHELWTZ_NAMESPACE::RACHELWTZ_CORE_NAMESPACE::assertion_failure("Assertion failure (" #x ")"); } }

#if defined(DEBUG) || defined(_DEBUG) || !(defined(NDEBUG) || defined(_NDEBUG))
    #define RACHELWTZ_DEBUG
    #define RACHELWTZ_DEBUG_ASSERT(x)   RACHELWTZ_ASSERT(x)
#else
    #define RACHELWTZ_DEBUG_ASSERT(x)
#endif

#endif