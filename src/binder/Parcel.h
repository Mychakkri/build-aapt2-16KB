#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>   // สำหรับ memcpy
#include <cstdlib>   // สำหรับ malloc/free

class Parcel {
public:
    Parcel() = default;
    ~Parcel() = default;

    // Dummy write/read int32
    int writeInt32(int32_t val) { 
        data_.push_back(std::to_string(val)); 
        return 0; 
    }

    int readInt32(int32_t* out) { 
        if (pos_ < data_.size()) {
            *out = std::stoi(data_[pos_++]);
            return 0;
        }
        return -1;
    }

    // Dummy write/read string16
    int writeString16(const std::u16string& str) { 
        data_.push_back(std::string(str.begin(), str.end())); 
        return 0;
    }

    std::u16string readString16() { 
        if (pos_ < data_.size()) {
            std::string s = data_[pos_++];
            return std::u16string(s.begin(), s.end());
        }
        return u"";
    }

    // Stub for writeInplace (แค่จอง buffer เฉย ๆ)
    void* writeInplace(size_t size) {
        void* buf = malloc(size);
        buffers_.push_back(buf); // เก็บไว้ free ทีหลัง
        return buf;
    }

    // Stub for write (copy data ลง buffer)
    int write(const void* data, size_t len) {
        const char* cdata = static_cast<const char*>(data);
        data_.emplace_back(cdata, cdata + len);
        return 0;
    }

    // Stub for setDataPosition (reset position)
    void setDataPosition(size_t pos) { pos_ = pos; }

    // Reset
    void reset() { 
        pos_ = 0; 
        data_.clear(); 
        for (auto* p : buffers_) free(p);
        buffers_.clear();
    }

private:
    std::vector<std::string> data_;
    size_t pos_ = 0;
    std::vector<void*> buffers_; // สำหรับเก็บ pointer ที่ malloc มา
};
