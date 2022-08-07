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

class Value;

class Array
{
public:
    class Data;
    class iterator;
    class const_iterator;

    Array();
    Array(Data data);
    Array(const Array &other);
    Array(Array &&other) noexcept;
    Array &operator=(const Array &other);
    Array &operator=(Array &&other) noexcept;
    ~Array();

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

    const Value &at(size_t index) const;

    bool empty() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    void append(Value v);
    void push_back(Value v);

    iterator insert(iterator it, Value v);
    template<typename It>
    iterator insert(iterator it, It begin, It end);

    Value &operator[](size_t index) noexcept;
    const Value &operator[](size_t index) const noexcept;

private:
    // std::vector is 3 pointers and it's size and alignment *should not* depend on the type
    using FakeType = std::vector<void*>;
    FastPimpl<Data, sizeof(FakeType), alignof(FakeType)> d;
};

class Object
{
public:
    class Data;
    class iterator;
    class const_iterator;

    Object();
    Object(Data data);
    Object(const Object &other);
    Object(Object &&other);
    Object &operator=(const Object &other);
    Object &operator=(Object &&other);
    ~Object();

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

    const Value &at(const QString &key) const;

    bool empty() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    std::pair<iterator, bool> insert(std::pair<QString, Value>);
    // template<typename It>
    // iterator insert(iterator it, It begin, It end);

    Value &operator[](const QString &key);

private:
    using FakeType = std::unordered_map<void *, void *>;
    FastPimpl<Data, sizeof(FakeType), alignof(FakeType)> d;
};

using ValueBase = std::variant<
    std::monostate,
    bool,
    int,
    uint,
    double,
    QString,
    QStringList,
    Array,
    Object
>;

class Value: public ValueBase
{
public:
    enum class Type {
        Null,
        Bool,
        Int,
        UInt,
        Double,
        String,
        StringList,
        Array,
        Object
    };

    using ValueBase::ValueBase;
    Value();
    Value(ValueBase v) noexcept;
    ~Value();
    Value(const Value &other);
    Value(Value &&other);

    Value &operator=(const Value &other);
    Value &operator=(Value &&other);

    bool isNull() const noexcept { return index() == 0; }
    Type type() const noexcept { return Type(index()); }

    void clear() { *this = {}; }

    template<typename T>
    const T *getIf() const noexcept { return std::get_if<T>(this); }

    template<typename T>
    T value(T defaultValue = {}) const
    {
        auto result = getIf<T>();
        if (!result)
            return std::move(defaultValue);
        return *result;
    }

    QVariant toQVariant() const;
    static Value fromQVariant(const QVariant &v);
};

class Array::Data : public std::vector<Value>
{
public:
    using Base = std::vector<Value>;
    using Base::Base;
};

class Array::const_iterator
{
public:
    using Data = std::vector<Value>::const_iterator;

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
    friend class Array::iterator;
    Data d{};
};

class Array::iterator
{
public:
    using Data = std::vector<Value>::iterator;

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

class Object::Data : public std::unordered_map<QString, Value>
{
public:
    using Base = std::unordered_map<QString, Value>;
    using Base::Base;
};

class Object::const_iterator
{
public:
    using Data = std::unordered_map<QString, Value>::const_iterator;

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
    friend class Object::iterator;
    Data d{};
};

class Object::iterator
{
public:
    using Data = std::unordered_map<QString, Value>::iterator;

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
inline Array::Array() = default;
inline Array::Array(Data data) : d(std::move(data))
{
}

inline Array::Array(const Array &other) = default;
inline Array::Array(Array &&other) noexcept = default;
inline Array &Array::operator=(const Array &other) = default;
inline Array &Array::operator=(Array &&other) noexcept = default;
inline Array::~Array() = default;

inline auto Array::begin() noexcept -> iterator { return data().begin(); }
inline auto Array::begin() const noexcept -> const_iterator { return data().cbegin(); }
inline auto Array::cbegin() const noexcept -> const_iterator
{
    return data().cbegin();
}
inline auto Array::constBegin() const noexcept -> const_iterator
{
    return data().cbegin();
}

inline auto Array::end() noexcept -> iterator { return data().end(); }
inline auto Array::end() const noexcept -> const_iterator { return data().cend(); }
inline auto Array::cend() const noexcept -> const_iterator { return data().cend(); }
inline auto Array::constEnd() const noexcept -> const_iterator { return data().cend(); }

inline const Value & Array::at(size_t index) const { return data().at(index); }
inline bool Array::empty() const noexcept { return data().empty(); }
inline bool Array::isEmpty() const noexcept { return empty(); }
inline size_t Array::size() const noexcept { return data().size(); }

inline void Array::append(Value v) { data().push_back(std::move(v)); }
inline void Array::push_back(Value v) { data().push_back(std::move(v)); }
inline auto Array::insert(iterator it, Value v) -> iterator
{ return data().insert(it.data(), std::move(v)); }
template<typename It>
inline auto Array::insert(iterator it, It begin, It end) -> iterator
{ return data().insert(it, begin, end); }

inline Value &Array::operator[](size_t index) noexcept { return data()[index]; }
inline const Value &Array::operator[](size_t index) const noexcept
{ return data()[index]; }

// VariantObject implementation
inline Object::Object() = default;
inline Object::Object(Data data) : d(std::move(data))
{}
inline Object::Object(const Object &other) = default;
inline Object::Object(Object &&other) = default;
inline Object &Object::operator=(const Object &other) = default;
inline Object &Object::operator=(Object &&other) = default;
inline Object::~Object() = default;

inline auto Object::begin() noexcept -> iterator { return data().begin(); }
inline auto Object::begin() const noexcept -> const_iterator { return data().cbegin(); }
inline auto Object::cbegin() const noexcept -> const_iterator
{
    return data().cbegin();
}
inline auto Object::constBegin() const noexcept -> const_iterator
{
    return data().cbegin();
}

inline auto Object::end() noexcept -> iterator { return data().end(); }
inline auto Object::end() const noexcept -> const_iterator { return data().cend(); }
inline auto Object::cend() const noexcept -> const_iterator { return data().cend(); }
inline auto Object::constEnd() const noexcept -> const_iterator { return data().cend(); }

inline const Value &Object::at(const QString &key) const { return data().at(key); }
inline bool Object::empty() const noexcept { return data().empty(); }
inline bool Object::isEmpty() const noexcept { return empty(); }
inline size_t Object::size() const noexcept { return data().size(); }

inline auto Object::insert(std::pair<QString, Value> value) -> std::pair<iterator, bool>
{
    return data().insert(std::move(value));
}

inline Value &Object::operator[](const QString &key) { return data()[key]; }

inline Value::Value() = default;
inline Value::Value(ValueBase v) noexcept : ValueBase(std::move(v)) {}
inline Value::Value(const Value &other) = default;
inline Value::Value(Value &&other) = default;
inline Value &Value::operator=(const Value &other) = default;
inline Value &Value::operator=(Value &&other) = default;
inline Value::~Value() = default;

inline bool operator==(const Array &lhs, const Array &rhs)
{
    return lhs.data() == rhs.data();
}

inline bool operator!=(const Array &lhs, const Array &rhs)
{
    return lhs.data() != rhs.data();
}

inline bool operator==(const Object &lhs, const Object &rhs)
{
    return lhs.data() == rhs.data();
}

inline bool operator!=(const Object &lhs, const Object &rhs)
{
    return lhs.data() != rhs.data();
}

inline bool operator==(const Value &lhs, const Value &rhs)
{
    return static_cast<const ValueBase &>(lhs) == static_cast<const ValueBase &>(rhs);
}

inline bool operator!=(const Value &lhs, const Value &rhs)
{
    return static_cast<const ValueBase &>(lhs) != static_cast<const ValueBase &>(rhs);
}

namespace std {

template<> struct hash<Object>
{
    std::size_t operator()(const Object &s) const noexcept
    {
        return hashRange(s.data());
    }
};

template<> struct hash<Array>
{
    std::size_t operator()(const Array &s) const noexcept
    {
        return hashRange(s.data());
    }
};

template<> struct hash<Value>
{
    std::size_t operator()(const Value &s) const noexcept
    {
        return std::hash<ValueBase>()(s);
    }
};

} // namespace std
