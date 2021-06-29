#include "variant.h"

#include <QCoreApplication>
#include <QDebug>

#include <unordered_map>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto foo = [](Variant v) { qDebug() << v.toQVariant(); };

    Variant v(10);
    Variant v2(v);
    v2 = v;

    QStringList stringList{"30", "40"};
    VariantObject complexKey1{{"key1", Variant("value 1")}};
    VariantObject complexKey2{{"key1", Variant("value 2")}};

    qInfo() << (complexKey1 == complexKey2);

    foo(10);
    foo("10");
    foo(stringList);
    foo(complexKey1);

    std::swap(v2, v);
    v2.swap(v);

    VariantArray arr;
    VariantArray arr2(std::vector<Variant>{});

    std::unordered_map<Variant, QString> map;

    map.insert({Variant(10), "int"});
    map.insert({Variant(15u), "uint"});
    map.insert({Variant("20"), "QString"});
    map.insert({Variant(stringList), "QStringList"});
    map.insert({Variant(complexKey1), "QbsVariantHash1"});
    map.insert({Variant(complexKey2), "QbsVariantHash2"});

    qDebug() << (v2 == v);
    qDebug() << (v2 != v);

    qDebug() << map.at(Variant(10));
    qDebug() << map.at(Variant(15u));
    qDebug() << map.at(Variant("20"));
    qDebug() << map.at(Variant(stringList));
    qDebug() << map.at(Variant(complexKey1));
    qDebug() << map.at(Variant(complexKey2));
}
