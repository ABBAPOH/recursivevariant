#ifndef VARIANT_H
#define VARIANT_H

#include "fastpimpl.h"
#include "utils.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/qhashfunctions.h>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <unordered_map>
#include <vector>
#include <memory>

//template <typename T> struct recursive_wrapper {
//  // construct from an existing object
//  recursive_wrapper(T t_) { t = std::make_unique<T>(std::move(t_)); }
//  // cast back to wrapped type
//  operator const T &() const { return *t.get(); }
//  // store the value
//  std::unique_ptr<T> t;
//};

//template <typename T, size_t Size = sizeof(T), size_t Alignment = alignof(T)>
//struct RecursiveWrapper
//{
//    // construct from an existing object
//    RecursiveWrapper(T t) : t(std::move(t)) {}
//    // cast back to wrapped type
//    operator const T &() const { return get(); }
//    const T &get() const noexcept { return *t.Ptr(); }

//private:
//  // store the value
//  FastPimpl<T, Size, Alignment> t;
//};

class Variant;

class VariantArray
{
public:
    using Data = std::vector<Variant>;

    VariantArray();
    VariantArray(Data data);
    VariantArray(std::initializer_list<Data::value_type> lst) : d(lst) {}
//    VariantArray(const VariantArray &other);
//    VariantArray(VariantArray &&other);
//    VariantArray &operator=(const VariantArray &other);
//    VariantArray &operator=(VariantArray &&other);
//    ~VariantArray();

    Data &data() noexcept { return *d.Ptr(); }
    const Data &data() const noexcept { return *d.Ptr(); }

private:
    using FakeType = std::vector<void*>;
    FastPimpl<Data, sizeof(FakeType), alignof(FakeType)> d;
};

class VariantObject
{
public:
    using Data = std::unordered_map<QString, Variant>;

    VariantObject();
    VariantObject(Data data);
    VariantObject(std::initializer_list<Data::value_type> lst) : d(lst) {}
//    VariantArray(const VariantArray &other);
//    VariantArray(VariantArray &&other);
//    VariantArray &operator=(const VariantArray &other);
//    VariantArray &operator=(VariantArray &&other);
//    ~VariantArray();

    Data &data() noexcept { return *d.Ptr(); }
    const Data &data() const noexcept { return *d.Ptr(); }

private:
    using FakeType = std::unordered_map<void *, void *>;
    FastPimpl<Data, sizeof(FakeType), alignof(FakeType)> d;
};

using VariantBase = std::variant<
    std::monostate,
    int,
    uint,
    QString,
    QStringList,
//    recursive_wrapper<QbsVariantVector>,
//    recursive_wrapper<QbsVariantHash>
    VariantArray,
    VariantObject
>;

class Variant: public VariantBase
{
public:
    enum class Type {
        Null,
        Int,
        UInt,
        String,
        StringList,
        Vector,
        Hash
    };

    using VariantBase::VariantBase;
    Variant(VariantBase v) noexcept : VariantBase(std::move(v)) {}
    Variant(const Variant &other) = default;

    bool isNull() const noexcept { return index() == 0; }
    Type type() const noexcept { return Type(index()); }

    void clear() { *this = {}; }

    template<typename T>
    T *getIf() const noexcept { return std::get_if<T>(*this); }

    template<typename T>
    T value(T defaultValue = {}) const
    {
        auto result = getIf<T>();
        if (!result)
            return std::move(defaultValue);
        return *result;
    }

    QVariant toQVariant() const;
    static Variant fromQVariant(const QVariant &v);
};

inline bool operator==(const VariantArray &lhs, const VariantArray &rhs)
{
    return lhs.data() == rhs.data();
}

inline bool operator!=(const VariantArray &lhs, const VariantArray &rhs)
{
    return lhs.data() != rhs.data();
}

inline bool operator==(const VariantObject &lhs, const VariantObject &rhs)
{
    return lhs.data() == rhs.data();
}

inline bool operator!=(const VariantObject &lhs, const VariantObject &rhs)
{
    return lhs.data() != rhs.data();
}

inline bool operator==(const Variant &lhs, const Variant &rhs)
{
    return static_cast<const VariantBase &>(lhs) == static_cast<const VariantBase &>(rhs);
}

inline bool operator!=(const Variant &lhs, const Variant &rhs)
{
    return static_cast<const VariantBase &>(lhs) != static_cast<const VariantBase &>(rhs);
}

// unordered map is not fwddecl'able
//class QbsVariantHash : public std::unordered_map<QString, QbsVariant>
//{
//};

//class VariantArray::Data : public std::vector<QbsVariant>
//{

//};

inline VariantArray::VariantArray() = default;
inline VariantArray::VariantArray(Data data) : d(std::move(data))
{
}

inline VariantObject::VariantObject() = default;
inline VariantObject::VariantObject(Data data) : d(std::move(data))
{
}

//inline VariantArray::VariantArray(VariantArray &&other) = default;
//inline VariantArray::VariantArray(const VariantArray &other) = default;
//inline VariantArray &VariantArray::operator=(const VariantArray &other) = default;
//inline VariantArray &VariantArray::operator=(VariantArray &&other) = default;
//inline VariantArray::~VariantArray() = default;

namespace std {

//template<typename T> struct hash<qbs::Internal::recursive_wrapper<T>>
//{
//    std::size_t operator()(const qbs::Internal::recursive_wrapper<T> &w) const noexcept
//    {
//        return std::hash<T>()(w);
//    }
//};

template<> struct hash<VariantObject>
{
    std::size_t operator()(const VariantObject &s) const noexcept
    {
        return hashRange(s.data());
    }
};

template<> struct hash<VariantArray>
{
    std::size_t operator()(const VariantArray &s) const noexcept
    {
        return hashRange(s.data());
    }
};

template<> struct hash<Variant>
{
    std::size_t operator()(const Variant &s) const noexcept
    {
        return std::hash<VariantBase>()(s);
    }
};

} // namespace std

#endif // VARIANT_H
