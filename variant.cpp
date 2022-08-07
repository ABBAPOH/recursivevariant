#include "variant.h"

#include <stdexcept>
#include <variant>

//using StdVariant = QbsVariantBase;

Object fromVariantHash(const QVariantHash &map)
{
    Object result;
    for (auto it = map.cbegin(), end = map.cend(); it != end; ++it)
        result.data()[it.key()] = Value::fromQVariant(it.value());
    return result;
}

QVariantHash toVariantHash(const Object &map)
{
    QVariantHash result;
    for (const auto &item: map.data())
        result[item.first] = item.second.toQVariant();
    return result;
}

Object fromVariantMap(const QVariantMap &map)
{
    Object result;
    for (auto it = map.cbegin(), end = map.cend(); it != end; ++it)
        result.data()[it.key()] = Value::fromQVariant(it.value());
    return result;
}

Array fromVariantList(const QVariantList &list)
{
    Array result;
    auto &data = result.data();
    data.reserve(size_t(list.size()));
    for (const auto &item: list)
        data.push_back(Value::fromQVariant(item));
    return result;
}

QVariantList toVariantList(const Array &list)
{
    QVariantList result;
    const auto &data = list.data();
    result.reserve(size_t(data.size()));
    for (const auto &item: data)
        result.push_back(item.toQVariant());
    return result;
}

//class QbsVariantData : public QSharedData, public StdVariant
//{
//public:
//    explicit QbsVariantData(StdVariant v) : StdVariant(std::move(v)) {}
//};

QVariant Value::toQVariant() const
{
    auto visitor = [](auto&& value) -> QVariant {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, Object>)
            return toVariantHash(value);
        else if constexpr (std::is_same_v<T, Array>)
            return toVariantList(value);
        else
            return QVariant::fromValue(value);
    };
    return std::visit(visitor, static_cast<const ValueBase&>(*this));
}

Value Value::fromQVariant(const QVariant &v)
{
    switch (v.userType()) {
    case QMetaType::Int: return v.toInt();
    case QMetaType::UInt: return v.toUInt();
    case QMetaType::QString: return v.toString();
    case QMetaType::QStringList: return v.toStringList();
    case QMetaType::QVariantHash: return fromVariantHash(v.toHash());
    case QMetaType::QVariantMap: return fromVariantMap(v.toMap());
    case QMetaType::QVariantList: return fromVariantList(v.toList());
    default:
        throw std::runtime_error(std::string("Unsupported variant type: ") + v.typeName());
    }
    return {};
}
