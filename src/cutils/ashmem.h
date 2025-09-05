#pragma once
#include <cstdint>
#include <sys/mman.h>

#ifdef __cplusplus
extern "C" {
#endif

// Fake flags
#define ASHMEM_NAME_LEN         256
#define ASHMEM_NOT_PURGEABLE    0
#define ASHMEM_PURGEABLE        1

// Dummy API
static inline int ashmem_create_region(const char* name, size_t size) {
    (void)name; (void)size;
    return -1; // stub: always fail
}

static inline int ashmem_set_prot_region(int fd, int prot) {
    (void)fd; (void)prot;
    return 0; // stub: pretend success
}

static inline int ashmem_pin_region(int fd, size_t offset, size_t len) {
    (void)fd; (void)offset; (void)len;
    return 0;
}

static inline int ashmem_unpin_region(int fd, size_t offset, size_t len) {
    (void)fd; (void)offset; (void)len;
    return 0;
}

#ifdef __cplusplus
}
#endif
