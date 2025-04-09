#pragma once

#include <vector>
#include <cassert>

namespace project {
namespace structures {

template <class T>
class Table {
public:
    Table() = default;
    Table(size_t h, size_t w);
    Table(size_t h, size_t w, const T& value);
    
    T Get(size_t x, size_t y) const;
    T& Get(size_t x, size_t y);
    void Fill(const T& value);
    void Assign(size_t h, size_t w, const T& value);

    size_t GetHeight() const;
    size_t GetWidth() const;

private:
    std::vector<T> data_;
    size_t height_ = 0;
    size_t width_ = 0;
};

template <class T>
Table<T>::Table(size_t h, size_t w) : height_(h), width_(w), data_(h * w) {
    assert(h > 0 && w > 0 && "Size must be positive");
}

template <class T>
Table<T>::Table(size_t h, size_t w, const T& value) : height_(h), width_(w), data_(h * w, value) {
    assert(h > 0 && w > 0 && "Size must be positive");
}

template <class T>
T Table<T>::Get(size_t x, size_t y) const {
    assert(x < height_ && y < width_ && "Incorrect indexes");
    return data_[x * width_ + y];
}

template <class T>
T& Table<T>::Get(size_t x, size_t y) {
    assert(x < height_ && y < width_ && "Incorrect indexes");
    return data_[x * width_ + y];
}

template <class T>
void Table<T>::Assign(size_t h, size_t w, const T& value) {
    data_.assign(h * w, value);
}

template <class T>
void Table<T>::Fill(const T& value) {
    data_.assign(height_ * width_, value);
}

template <class T>
size_t Table<T>::GetHeight() const {
    return height_;
}

template <class T>
size_t Table<T>::GetWidth() const {
    return width_;
}

}  // namespace structures
}  // namespace project
