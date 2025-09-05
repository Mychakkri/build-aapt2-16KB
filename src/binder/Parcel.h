#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Parcel {
public:
    Parcel() = default;
    ~Parcel() = default;

    // Dummy read/write functions
    int writeInt32(int32_t val) { data_.push_back(std::to_string(val)); return 0; }
    int readInt32(int32_t* out) { 
        if (pos_ < data_.size()) {
            *out = std::stoi(data_[pos_++]);
            return 0;
        }
        return -1;
    }

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

    void reset() { pos_ = 0; data_.clear(); }

private:
    std::vector<std::string> data_;
    size_t pos_ = 0;
};
