#include <QtTest>

#include "variant.h"

class TestValue: public QObject
{
    Q_OBJECT
public:
    TestValue(QObject *parent = 0): QObject(parent) {}

private slots:
    void testValueSimple();
    void testNumbers();
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

void TestValue::testNumbers()
{
    // TODO: check unsigned
    {
        int numbers[] = {
            0,
            -1,
            1,
            (1<<26),
            (1<<27),
            (1<<28),
            -(1<<26),
            -(1<<27),
            -(1<<28),
            (1<<26) - 1,
            (1<<27) - 1,
            (1<<28) - 1,
            -((1<<26) - 1),
            -((1<<27) - 1),
            -((1<<28) - 1)
        };
        int n = sizeof(numbers)/sizeof(int);

        Array array;
        for (int i = 0; i < n; ++i)
            array.append(numbers[i]);

        auto array2 = array;

        QCOMPARE(array.size(), array2.size());
        for (size_t i = 0; i < array.size(); ++i) {
            QCOMPARE(array.at(i).type(), Value::Type::Int);
            QCOMPARE(array.at(i).value<int>(), numbers[i]);
            QCOMPARE(array2.at(i).type(), Value::Type::Int);
            QCOMPARE(array2.at(i).value<int>(), numbers[i]);
        }
    }

    {
        int64_t numbers[] = {
            0,
            -1,
            1,
            (1ll<<54),
            (1ll<<55),
            (1ll<<56),
            -(1ll<<54),
            -(1ll<<55),
            -(1ll<<56),
            (1ll<<54) - 1,
            (1ll<<55) - 1,
            (1ll<<56) - 1,
            -((1ll<<54) - 1),
            -((1ll<<55) - 1),
            -((1ll<<56) - 1)
        };
        int n = sizeof(numbers)/sizeof(qint64);

        Array array;
        for (int i = 0; i < n; ++i)
            array.append(numbers[i]);

        auto array2 = array;

        QCOMPARE(array.size(), array2.size());
        for (size_t i = 0; i < array.size(); ++i) {
            QCOMPARE(array.at(i).type(), Value::Type::Int64);
            QCOMPARE(array.at(i).value<int64_t>(), numbers[i]);
            QCOMPARE(array2.at(i).type(), Value::Type::Int64);
            QCOMPARE(array2.at(i).value<int64_t>(), numbers[i]);
        }
    }

    {
        double numbers[] = {
            0,
            -1,
            1,
            double(1ll<<54),
            double(1ll<<55),
            double(1ll<<56),
            double(-(1ll<<54)),
            double(-(1ll<<55)),
            double(-(1ll<<56)),
            double((1ll<<54) - 1),
            double((1ll<<55) - 1),
            double((1ll<<56) - 1),
            double(-((1ll<<54) - 1)),
            double(-((1ll<<55) - 1)),
            double(-((1ll<<56) - 1)),
            1.1,
            0.1,
            -0.1,
            -1.1,
            1e200,
            -1e200
        };
        int n = sizeof(numbers)/sizeof(double);

        Array array;
        for (int i = 0; i < n; ++i)
            array.append(numbers[i]);

        auto array2 = array;

        QCOMPARE(array.size(), array2.size());
        for (size_t i = 0; i < array.size(); ++i) {
            QCOMPARE(array.at(i).type(), Value::Type::Double);
            QCOMPARE(array.at(i).value<double>(), numbers[i]);
            QCOMPARE(array2.at(i).type(), Value::Type::Double);
            QCOMPARE(array2.at(i).value<double>(), numbers[i]);
        }
    }
}

QTEST_MAIN(TestValue)
#include "test_variant.moc"