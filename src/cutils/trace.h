#pragma once

#ifndef DUMMY_TRACE_H
#define DUMMY_TRACE_H

#include <stdint.h>

// ATRACE TAGS
#define ATRACE_TAG_NEVER 0
#define ATRACE_TAG_ALWAYS 0
#define ATRACE_TAG_RESOURCES 0

#ifndef ATRACE_TAG
#define ATRACE_TAG ATRACE_TAG_RESOURCES
#endif

// Stub macros
#define ATRACE_BEGIN(name)
#define ATRACE_END()
#define ATRACE_INT(name, value)
#define ATRACE_ASYNC_BEGIN(name, cookie)
#define ATRACE_ASYNC_END(name, cookie)

// Stub functions (แทนพวก atrace_* ที่ Trace.h เรียก)
static inline void atrace_begin(uint64_t tag, const char* name) { (void)tag; (void)name; }
static inline void atrace_end(uint64_t tag) { (void)tag; }
static inline void atrace_async_begin(uint64_t tag, const char* name, int32_t cookie) { (void)tag; (void)name; (void)cookie; }
static inline void atrace_async_end(uint64_t tag, const char* name, int32_t cookie) { (void)tag; (void)name; (void)cookie; }
static inline void atrace_int(uint64_t tag, const char* name, int32_t value) { (void)tag; (void)name; (void)value; }

#endif // DUMMY_TRACE_H
