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
    class iterator;
    class const_iterator;

    VariantArray();
    VariantArray(Data data);
    VariantArray(const VariantArray &other);
    VariantArray(VariantArray &&other) noexcept;
    VariantArray &operator=(const VariantArray &other);
    VariantArray &operator=(VariantArray &&other) noexcept;
    ~VariantArray();

    Data &data() noexcept { return *d.Ptr(); }
    const Data &data() const noexcept { return *d.Ptr(); }

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator constBegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator constEnd() const noexcept;

    const Variant &at(size_t index) const;

    bool empty() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    void append(Variant v);
    void push_back(Variant v);

    iterator insert(iterator it, Variant v);
    template<typename It>
    iterator insert(iterator it, It begin, It end);

    Variant &operator[](size_t index) noexcept;
    const Variant &operator[](size_t index) const noexcept;

private:
    // std::vector is 3 pointers and it's size and alignment *should not* depend on the type
    using FakeType = std::vector<void*>;
    FastPimpl<Data, sizeof(FakeType), alignof(FakeType)> d;
};

class VariantObject
{
public:
    class Data;
    class iterator;
    class const_iterator;
    // using value_type = std::pair<const QString, Variant>;

    VariantObject();
    VariantObject(Data data);
    VariantObject(const VariantObject &other);
    VariantObject(VariantObject &&other);
    VariantObject &operator=(const VariantObject &other);
    VariantObject &operator=(VariantObject &&other);
    ~VariantObject();

    Data &data() noexcept { return *d.Ptr(); }
    const Data &data() const noexcept { return *d.Ptr(); }

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator constBegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator constEnd() const noexcept;

    const Variant &at(const QString &key) const;

    bool empty() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    Variant &operator[](const QString &key);

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

class VariantArray::const_iterator
{
public:
    using Data = std::vector<Variant>::const_iterator;

    using iterator_category = Data::iterator_category;
    using difference_type = Data::difference_type;
    using value_type = Data::value_type;
    using reference = Data::reference;
    using pointer = Data::pointer;

    inline const_iterator() noexcept = default;
    inline const_iterator(const Data &dd) noexcept: d(dd) {}

    inline Data data() const { return d; }

    inline reference operator*() const { return d.operator*(); }
    inline pointer operator->() const { return d.operator->(); }
    inline reference operator[](size_t j) const { return d[j]; }

    inline bool operator==(const const_iterator &o) const noexcept { return d == o.d; }
    inline bool operator!=(const const_iterator &o) const noexcept { return d != o.d; }
    inline bool operator<(const const_iterator& other) const noexcept { return d < other.d; }
    inline bool operator<=(const const_iterator& other) const noexcept { return d <= other.d; }
    inline bool operator>(const const_iterator& other) const noexcept { return d > other.d; }
    inline bool operator>=(const const_iterator& other) const noexcept { return d >= other.d; }
    inline const_iterator &operator++() noexcept { ++d; return *this; }
    inline const_iterator operator++(int) noexcept { const_iterator n = *this; ++d; return n; }
    inline const_iterator &operator--() noexcept { d--; return *this; }
    inline const_iterator operator--(int) noexcept { const_iterator n = *this; d--; return n; }
    inline const_iterator &operator+=(difference_type j) noexcept { d+=j; return *this; }
    inline const_iterator &operator-=(difference_type j) noexcept { d-=j; return *this; }
    inline const_iterator operator+(difference_type j) const noexcept
    { return const_iterator(d+j); }
    inline const_iterator operator-(difference_type j) const noexcept
    { return const_iterator(d-j); }
    inline int operator-(const_iterator o) const noexcept { return d - o.d; }

private:
    friend class VariantArray::iterator;
    Data d{};
};

class VariantArray::iterator
{
public:
    using Data = std::vector<Variant>::iterator;

    using iterator_category = Data::iterator_category;
    using difference_type = Data::difference_type;
    using value_type = Data::value_type;
    using reference = Data::reference;
    using pointer = Data::pointer;

    inline iterator() noexcept = default;
    inline iterator(const Data &dd) noexcept: d(dd) {}

    inline Data data() const noexcept { return d; }

    inline reference operator*() const noexcept { return *d; }
    inline pointer operator->() const noexcept { return d.operator->(); }
    inline reference operator[](size_t j) const noexcept { return d[j]; }

    inline bool operator==(const iterator &o) const noexcept { return d == o.d; }
    inline bool operator!=(const iterator &o) const noexcept { return d != o.d; }
    inline bool operator<(const iterator& other) const noexcept { return d < other.d; }
    inline bool operator<=(const iterator& other) const noexcept { return d <= other.d; }
    inline bool operator>(const iterator& other) const noexcept { return d > other.d; }
    inline bool operator>=(const iterator& other) const noexcept { return d >= other.d; }
    inline bool operator==(const const_iterator &o) const noexcept { return d == o.d; }
    inline bool operator!=(const const_iterator &o) const noexcept { return d != o.d; }
    inline bool operator<(const const_iterator& other) const noexcept { return d < other.d; }
    inline bool operator<=(const const_iterator& other) const noexcept { return d <= other.d; }
    inline bool operator>(const const_iterator& other) const noexcept { return d > other.d; }
    inline bool operator>=(const const_iterator& other) const noexcept { return d >= other.d; }
    inline iterator &operator++() noexcept { ++d; return *this; }
    inline iterator operator++(int) noexcept { iterator n = *this; ++d; return n; }
    inline iterator &operator--() noexcept { d--; return *this; }
    inline iterator operator--(int) noexcept { iterator n = *this; d--; return n; }
    inline iterator &operator+=(difference_type j) noexcept { d+=j; return *this; }
    inline iterator &operator-=(difference_type j) noexcept { d-=j; return *this; }
    inline iterator operator+(difference_type j) const noexcept { return iterator(d+j); }
    inline iterator operator-(difference_type j) const noexcept { return iterator(d-j); }
    inline int operator-(iterator o) const noexcept { return d - o.d; }

private:
    Data d{};
};

class VariantObject::Data : public std::unordered_map<QString, Variant>
{
public:
    using Base = std::unordered_map<QString, Variant>;
    using Base::Base;
};

class VariantObject::const_iterator
{
public:
    using Data = std::unordered_map<QString, Variant>::const_iterator;

    using iterator_category = Data::iterator_category;
    using difference_type = Data::difference_type;
    using value_type = Data::value_type;
    using reference = Data::reference;
    using pointer = Data::pointer;

    inline const_iterator() noexcept = default;
    inline const_iterator(const Data &dd) noexcept: d(dd) {}

    inline Data data() const noexcept { return d; }

    inline reference operator*() const noexcept { return d.operator*(); }
    inline pointer operator->() const noexcept { return d.operator->(); }

    inline bool operator==(const const_iterator &o) const noexcept { return d == o.d; }
    inline bool operator!=(const const_iterator &o) const noexcept { return d != o.d; }

    inline const_iterator &operator++() noexcept { ++d; return *this; }
    inline const_iterator operator++(int) noexcept { const_iterator n = *this; ++d; return n; }

private:
    friend class VariantObject::iterator;
    Data d{};
};

class VariantObject::iterator
{
public:
    using Data = std::unordered_map<QString, Variant>::iterator;

    using iterator_category = Data::iterator_category;
    using difference_type = Data::difference_type;
    using value_type = Data::value_type;
    using reference = Data::reference;
    using pointer = Data::pointer;

    inline iterator() noexcept = default;
    inline iterator(const Data &dd) noexcept: d(dd) {}

    inline Data data() const noexcept { return d; }

    inline reference operator*() const noexcept { return d.operator*(); }
    inline pointer operator->() const noexcept { return d.operator->(); }

    inline bool operator==(const iterator &o) const noexcept { return d == o.d; }
    inline bool operator!=(const iterator &o) const noexcept { return d != o.d; }

    inline bool operator==(const const_iterator &o) const noexcept { return d == o.d; }
    inline bool operator!=(const const_iterator &o) const noexcept { return d != o.d; }

    inline iterator &operator++() noexcept { ++d; return *this; }
    inline iterator operator++(int) noexcept { iterator n = *this; ++d; return n; }

private:
    Data d{};
};

// VariantArray implementation
inline VariantArray::VariantArray() = default;
inline VariantArray::VariantArray(Data data) : d(std::move(data))
{
}

inline VariantArray::VariantArray(const VariantArray &other) = default;
inline VariantArray::VariantArray(VariantArray &&other) noexcept = default;
inline VariantArray &VariantArray::operator=(const VariantArray &other) = default;
inline VariantArray &VariantArray::operator=(VariantArray &&other) noexcept = default;
inline VariantArray::~VariantArray() = default;

inline auto VariantArray::begin() noexcept -> iterator { return data().begin(); }
inline auto VariantArray::begin() const noexcept -> const_iterator { return data().cbegin(); }
inline auto VariantArray::cbegin() const noexcept -> const_iterator
{
    return data().cbegin();
}
inline auto VariantArray::constBegin() const noexcept -> const_iterator
{
    return data().cbegin();
}

inline auto VariantArray::end() noexcept -> iterator { return data().end(); }
inline auto VariantArray::end() const noexcept -> const_iterator { return data().cend(); }
inline auto VariantArray::cend() const noexcept -> const_iterator { return data().cend(); }
inline auto VariantArray::constEnd() const noexcept -> const_iterator { return data().cend(); }

inline const Variant & VariantArray::at(size_t index) const { return data().at(index); }
inline bool VariantArray::empty() const noexcept { return data().empty(); }
inline bool VariantArray::isEmpty() const noexcept { return empty(); }
inline size_t VariantArray::size() const noexcept { return data().size(); }

inline void VariantArray::append(Variant v) { data().push_back(std::move(v)); }
inline void VariantArray::push_back(Variant v) { data().push_back(std::move(v)); }
inline auto VariantArray::insert(iterator it, Variant v) -> iterator
{ return data().insert(it.data(), std::move(v)); }
template<typename It>
inline auto VariantArray::insert(iterator it, It begin, It end) -> iterator
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

inline auto VariantObject::begin() noexcept -> iterator { return data().begin(); }
inline auto VariantObject::begin() const noexcept -> const_iterator { return data().cbegin(); }
inline auto VariantObject::cbegin() const noexcept -> const_iterator
{
    return data().cbegin();
}
inline auto VariantObject::constBegin() const noexcept -> const_iterator
{
    return data().cbegin();
}

inline auto VariantObject::end() noexcept -> iterator { return data().end(); }
inline auto VariantObject::end() const noexcept -> const_iterator { return data().cend(); }
inline auto VariantObject::cend() const noexcept -> const_iterator { return data().cend(); }
inline auto VariantObject::constEnd() const noexcept -> const_iterator { return data().cend(); }

inline const Variant &VariantObject::at(const QString &key) const { return data().at(key); }
inline bool VariantObject::empty() const noexcept { return data().empty(); }
inline bool VariantObject::isEmpty() const noexcept { return empty(); }
inline size_t VariantObject::size() const noexcept { return data().size(); }

inline Variant &VariantObject::operator[](const QString &key) { return data()[key]; }

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
