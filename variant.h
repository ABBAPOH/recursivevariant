#pragma once

#include "fastpimpl.h"
#include "utils.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/qhashfunctions.h>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <unordered_map>
#include <vector>
#include <memory>

class Variant;

class VariantArray
{
public:
    class Data;
    // using Iterator = std::vector<Variant>::iterator;
    // using ConstIterator = std::vector<Variant>::const_iterator;

    class Iterator;
    class ConstIterator;

    VariantArray();
    VariantArray(Data data);
    VariantArray(const VariantArray &other);
    VariantArray(VariantArray &&other);
    VariantArray &operator=(const VariantArray &other);
    VariantArray &operator=(VariantArray &&other);
    ~VariantArray();

    Data &data() noexcept { return *d.Ptr(); }
    const Data &data() const noexcept { return *d.Ptr(); }

    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator constBegin() const noexcept;

    Iterator end() noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cend() const noexcept;
    ConstIterator constEnd() const noexcept;

    const Variant &at(size_t index) const;

    bool empty() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    void append(Variant v);
    void push_back(Variant v);

    Iterator insert(Iterator it, Variant v);
    template<typename It>
    Iterator insert(Iterator it, It begin, It end);

    Variant &operator[](size_t index) noexcept;
    const Variant &operator[](size_t index) const noexcept;

private:
    using FakeType = std::vector<void*>;
    FastPimpl<Data, sizeof(FakeType), alignof(FakeType)> d;
};

class VariantObject
{
public:
    class Data;
    class Iterator;// = std::unordered_map<QString, Variant>::iterator;
    class ConstIterator;// = std::unordered_map<QString, Variant>::const_iterator;

    VariantObject();
    VariantObject(Data data);
    VariantObject(const VariantObject &other);
    VariantObject(VariantObject &&other);
    VariantObject &operator=(const VariantObject &other);
    VariantObject &operator=(VariantObject &&other);
    ~VariantObject();

    Data &data() noexcept { return *d.Ptr(); }
    const Data &data() const noexcept { return *d.Ptr(); }

    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator constBegin() const noexcept;

    Iterator end() noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cend() const noexcept;
    ConstIterator constEnd() const noexcept;

    const Variant &at(const QString &key) const;

    bool empty() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

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
    Variant();
    Variant(VariantBase v) noexcept;
    ~Variant();
    Variant(const Variant &other);
    Variant(Variant &&other);

    Variant &operator=(const Variant &other);
    Variant &operator=(Variant &&other);

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

class VariantArray::Data : public std::vector<Variant>
{
public:
    using Base = std::vector<Variant>;
    using Base::Base;
};

class VariantArray::Iterator: public std::vector<Variant>::iterator
{
public:
    using Base = std::vector<Variant>::iterator;
    using Base::Base;
    Iterator(const Base &base): Base(base) {}
};

class VariantArray::ConstIterator: public std::vector<Variant>::const_iterator
{
public:
    using Base = std::vector<Variant>::const_iterator;
    using Base::Base;
    ConstIterator(const Base &base): Base(base) {}
};

class VariantObject::Data : public std::unordered_map<QString, Variant>
{
public:
    using Base = std::unordered_map<QString, Variant>;
    using Base::Base;
};

class VariantObject::Iterator: public std::unordered_map<QString, Variant>::iterator
{
public:
    using Base = std::unordered_map<QString, Variant>::iterator;
    using Base::Base;
    Iterator(const Base &base): Base(base) {}
};

class VariantObject::ConstIterator: public std::unordered_map<QString, Variant>::const_iterator
{
public:
    using Base = std::unordered_map<QString, Variant>::const_iterator;
    using Base::Base;
    ConstIterator(const Base &base): Base(base) {}
};

// VariantArray implementation
inline VariantArray::VariantArray() = default;
inline VariantArray::VariantArray(Data data) : d(std::move(data))
{
}

inline VariantArray::VariantArray(const VariantArray &other) = default;
inline VariantArray::VariantArray(VariantArray &&other) = default;
inline VariantArray &VariantArray::operator=(const VariantArray &other) = default;
inline VariantArray &VariantArray::operator=(VariantArray &&other) = default;
inline VariantArray::~VariantArray() = default;

inline auto VariantArray::begin() noexcept -> Iterator { return data().begin(); }
inline auto VariantArray::begin() const noexcept -> ConstIterator { return data().cbegin(); }
inline auto VariantArray::cbegin() const noexcept -> ConstIterator
{
    return data().cbegin();
}
inline auto VariantArray::constBegin() const noexcept -> ConstIterator
{
    return data().cbegin();
}

inline auto VariantArray::end() noexcept -> Iterator { return data().end(); }
inline auto VariantArray::end() const noexcept -> ConstIterator { return data().cend(); }
inline auto VariantArray::cend() const noexcept -> ConstIterator { return data().cend(); }
inline auto VariantArray::constEnd() const noexcept -> ConstIterator { return data().cend(); }

inline const Variant & VariantArray::at(size_t index) const { return data().at(index); }
inline bool VariantArray::empty() const noexcept { return data().empty(); }
inline bool VariantArray::isEmpty() const noexcept { return empty(); }
inline size_t VariantArray::size() const noexcept { return data().size(); }

inline void VariantArray::append(Variant v) { data().push_back(std::move(v)); }
inline void VariantArray::push_back(Variant v) { data().push_back(std::move(v)); }
inline auto VariantArray::insert(Iterator it, Variant v) -> Iterator
{ return data().insert(it, std::move(v)); }
template<typename It>
inline auto VariantArray::insert(Iterator it, It begin, It end) -> Iterator
{ return data().insert(it, begin, end); }

inline Variant &VariantArray::operator[](size_t index) noexcept { return data()[index]; }
inline const Variant &VariantArray::operator[](size_t index) const noexcept
{ return data()[index]; }

// VariantObject implementation
inline VariantObject::VariantObject() = default;
inline VariantObject::VariantObject(Data data) : d(std::move(data))
{}
inline VariantObject::VariantObject(const VariantObject &other) = default;
inline VariantObject::VariantObject(VariantObject &&other) = default;
inline VariantObject &VariantObject::operator=(const VariantObject &other) = default;
inline VariantObject &VariantObject::operator=(VariantObject &&other) = default;
inline VariantObject::~VariantObject() = default;

inline auto VariantObject::begin() noexcept -> Iterator { return data().begin(); }
inline auto VariantObject::begin() const noexcept -> ConstIterator { return data().cbegin(); }
inline auto VariantObject::cbegin() const noexcept -> ConstIterator
{
    return data().cbegin();
}
inline auto VariantObject::constBegin() const noexcept -> ConstIterator
{
    return data().cbegin();
}

inline auto VariantObject::end() noexcept -> Iterator { return data().end(); }
inline auto VariantObject::end() const noexcept -> ConstIterator { return data().cend(); }
inline auto VariantObject::cend() const noexcept -> ConstIterator { return data().cend(); }
inline auto VariantObject::constEnd() const noexcept -> ConstIterator { return data().cend(); }

inline const Variant &VariantObject::at(const QString &key) const { return data().at(key); }
inline bool VariantObject::empty() const noexcept { return data().empty(); }
inline bool VariantObject::isEmpty() const noexcept { return empty(); }
inline size_t VariantObject::size() const noexcept { return data().size(); }

inline Variant::Variant() = default;
inline Variant::Variant(VariantBase v) noexcept : VariantBase(std::move(v)) {}
inline Variant::Variant(const Variant &other) = default;
inline Variant::Variant(Variant &&other) = default;
inline Variant &Variant::operator=(const Variant &other) = default;
inline Variant &Variant::operator=(Variant &&other) = default;
inline Variant::~Variant() = default;

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

namespace std {

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
