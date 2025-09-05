#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cstdlib>

class Parcel {
public:
    Parcel() = default;
    ~Parcel() {
        for (auto* p : buffers_) free(p);
    }

    // ========= Write methods =========
    int writeInt32(int32_t val)   { return push(std::to_string(val)); }
    int writeInt64(int64_t val)   { return push(std::to_string(val)); }
    int writeFloat(float val)     { return push(std::to_string(val)); }
    int writeDouble(double val)   { return push(std::to_string(val)); }
    int writeBool(bool val)       { return push(val ? "1" : "0"); }
    int writeCString(const char* str) {
        data_.push_back(str ? str : "");
        return 0;
    }
    int writeString16(const std::u16string& str) {
        data_.push_back(std::string(str.begin(), str.end()));
        return 0;
    }
    int writeString16(const char16_t* str, size_t len) {
        data_.push_back(std::string(str, str + len));
        return 0;
    }
    int writeNoException() { return 0; }

    // ========= Read methods =========
    int readInt32(int32_t* out)   { return pop(out); }
    int readInt64(int64_t* out)   { return pop(out); }
    int readFloat(float* out)     { return pop(out); }
    int readDouble(double* out)   { return pop(out); }
    bool readBool() {
        if (pos_ < data_.size()) return data_[pos_++] == "1";
        return false;
    }
    const char* readCString() {
        if (pos_ < data_.size()) return data_[pos_++].c_str();
        return "";
    }
    std::u16string readString16() {
        if (pos_ < data_.size()) {
            std::string s = data_[pos_++];
            return std::u16string(s.begin(), s.end());
        }
        return u"";
    }

    // ========= Raw data =========
    int write(const void* buf, size_t len) {
        const char* cbuf = static_cast<const char*>(buf);
        data_.emplace_back(cbuf, cbuf + len);
        return 0;
    }
    void* writeInplace(size_t size) {
        void* buf = malloc(size);
        buffers_.push_back(buf);
        return buf;
    }

    // ========= Position control =========
    void setDataPosition(size_t pos) { pos_ = pos; }
    size_t dataPosition() const { return pos_; }
    void reset() {
        pos_ = 0;
        data_.clear();
        for (auto* p : buffers_) free(p);
        buffers_.clear();
    }

private:
    std::vector<std::string> data_;
    size_t pos_ = 0;
    std::vector<void*> buffers_;

    int push(const std::string& s) {
        data_.push_back(s);
        return 0;
    }

    template<typename T>
    int pop(T* out) {
        if (pos_ < data_.size()) {
            *out = static_cast<T>(std::stod(data_[pos_++]));
            return 0;
        }
        return -1;
    }
};
