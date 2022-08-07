#include <QtTest>

#include "variant.h"

class TestValue: public QObject
{
    Q_OBJECT
public:
    TestValue(QObject *parent = 0): QObject(parent) {}

private slots:
    void testValueSimple();
};

void TestValue::testValueSimple()
{
    Object object;
    object.insert({"number", 999.});
    Array array;
    for (int i = 0; i < 10; ++i)
        array.append((double)i);

    Value value(true);
    QCOMPARE(value.type(), Value::Type::Bool);
    QCOMPARE(value.value<double>(), 0.);
    QCOMPARE(value.value<QString>(), QString());
    QCOMPARE(value.value<bool>(), true);
    QCOMPARE(value.value<Object>(), Object());
    QCOMPARE(value.value<Array>(), Array());
    QCOMPARE(value.value<double>(99.), 99.);
    QCOMPARE(value.value<QString>(QString("test")), QString("test"));
    QCOMPARE(value.value<Object>(object), object);
    QCOMPARE(value.value<Array>(array), array);

    value = 999.;
    QCOMPARE(value.type(), Value::Type::Double);
    QCOMPARE(value.value<double>(), 999.);
    QCOMPARE(value.value<QString>(), QString());
    QCOMPARE(value.value<bool>(), false);
    QCOMPARE(value.value<bool>(true), true);
    QCOMPARE(value.value<Object>(), Object());
    QCOMPARE(value.value<Array>(), Array());

    value = QLatin1String("test");
    QCOMPARE(value.type(), Value::Type::String);
    QCOMPARE(value.value<double>(), 0.);
    QCOMPARE(value.value<QString>(), QLatin1String("test"));
    QCOMPARE(value.value<bool>(), false);
    QCOMPARE(value.value<Object>(), Object());
    QCOMPARE(value.value<Array>(), Array());

    value = true;
    QCOMPARE(value.type(), Value::Type::Bool);
    QCOMPARE(value.value<double>(), 0.);
    QCOMPARE(value.value<QString>(), QString());
    QCOMPARE(value.value<bool>(), true);
    QCOMPARE(value.value<Object>(), Object());
    QCOMPARE(value.value<Array>(), Array());

    value = 999.;
    QCOMPARE(value.type(), Value::Type::Double);
    QCOMPARE(value.value<double>(), 999.);
    QCOMPARE(value.value<QString>(), QString());
    QCOMPARE(value.value<bool>(), false);
    QCOMPARE(value.value<Object>(), Object());
    QCOMPARE(value.value<Array>(), Array());
}

QTEST_MAIN(TestValue)
#include "test_variant.moc"