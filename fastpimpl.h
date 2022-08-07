#ifndef FASTPIMPL_H
#define FASTPIMPL_H

#include <utility>

template <class T, std::size_t Size, std::size_t Alignment>
class FastPimpl {
public:
    template <class... Args>
    explicit FastPimpl(Args&&... args) {
        new (Ptr()) T(std::forward<Args>(args)...);
    }

    FastPimpl(const FastPimpl &other)
    {
        new (Ptr()) T(*other.Ptr());
    }

    FastPimpl(FastPimpl &&other) noexcept
    {
        new (Ptr()) T(std::move(*other.Ptr()));
    }

    FastPimpl& operator=(const FastPimpl& rhs) {
        *Ptr() = *rhs.Ptr();
        return *this;
    }

    FastPimpl& operator=(FastPimpl&& rhs) noexcept {
        *Ptr() = std::move(*rhs.Ptr());
        return *this;
    }

    ~FastPimpl() noexcept {
      validate<sizeof(T), alignof(T)>();
      Ptr()->~T();
    }

    T *Ptr() { return reinterpret_cast<T*>(&data); }
    const T *Ptr() const { return reinterpret_cast<const T*>(&data); }

    T* operator->() noexcept { return Ptr(); }
    const T* operator->() const noexcept { return Ptr(); }
    T& operator*() noexcept { return *Ptr(); }
    const T& operator*() const noexcept { return *Ptr(); }

private:
    template <std::size_t ActualSize, std::size_t ActualAlignment>
    static void validate() noexcept {
        static_assert(Size == ActualSize, "Size and sizeof(T) mismatch");
        static_assert(Alignment == ActualAlignment,  "Alignment and alignof(T) mismatch");
    }

    std::aligned_storage_t<Size, Alignment> data;
};

#endif // FASTPIMPL_H
