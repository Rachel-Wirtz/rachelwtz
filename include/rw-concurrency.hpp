#ifndef RACHELWTZ__CONCURRENCY__HPP
#define RACHELWTZ__CONCURRENCY__HPP

#include "rw-core.hpp"

#define RACHELWTZ_CONCURRENCY_NAMESPACE concurrency
#define RACHELWTZ_CONCURRENCY_BEGIN     namespace RACHELWTZ_NAMESPACE::RACHELWTZ_CONCURRENCY_NAMESPACE {
#define RACHELWTZ_CONCURRENCY_END       }
#define USING_RACHELWTZ_CONCURRENCY     using namespace ::RACHELWTZ_NAMESPACE::RACHELWTZ_CONCURRENCY_NAMESPACE

#include <mutex>
#include <thread>

//
// Concurrency Libraries
//
#include "rw-concurrency-wrapper.hpp"

#endif