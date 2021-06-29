#ifndef UTILS_H
#define UTILS_H

#include <QStringList>

#include <unordered_map>

// based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0814r0.pdf
template<typename T>
void hashCombineHelper(size_t &seed, const T &val)
{
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename... Types>
size_t hashCombine(const Types &... args)
{
    size_t seed = 0;
    (hashCombineHelper(seed, args), ...); // create hash value with seed over all args return seed;
    return seed;
}

template<typename It>
size_t hashRange(It first, It last)
{
    size_t seed = 0;
    for (; first != last; ++first)
        hashCombineHelper(seed, *first);

    return seed;
}

template<typename R>
size_t hashRange(R &&range)
{
    return hashRange(std::begin(range), std::end(range));
}

namespace std {

template<> struct hash<QStringList>
{
    std::size_t operator()(const QStringList &s) const noexcept
    {
        return hashRange(s);
    }
};

template<typename T1, typename T2> struct hash<std::pair<T1, T2>>
{
    size_t operator()(const pair<T1, T2> &x) const
    {
        return hashCombine(x.first, x.second);
    }
};

} // namespace std

#endif // UTILS_H
