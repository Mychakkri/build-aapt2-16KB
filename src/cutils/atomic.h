#pragma once
#include <cstdint>

// android_atomic_dec ต้องรับ volatile int32_t*
static inline int android_atomic_dec(volatile int32_t* p) {
    return --(*p);
}

// android_atomic_inc (บางไฟล์อาจใช้ด้วย)
static inline int android_atomic_inc(volatile int32_t* p) {
    return ++(*p);
}
