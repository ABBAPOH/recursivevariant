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
    void testObjectSimple();
    void benchArray();
    void benchQVariantList();
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

void TestValue::testObjectSimple()
{
    Object object;
    object.insert({"number", 999.});
    QCOMPARE(object.value("number").type(), Value::Type::Double);
    QCOMPARE(object.value("number").value<double>(), 999.);
    object.insert({"string", QString::fromLatin1("test")});
    QCOMPARE(object.value("string").type(), Value::Type::String);
    QCOMPARE(object.value("string").value<QString>(), QString("test"));
    object.insert({"boolean", true});
    QCOMPARE(object.value("boolean").value<bool>(), true);

    QVERIFY2(object.contains("number"), "key number not found");
    QVERIFY2(object.contains("string"), "key string not found");
    QVERIFY2(object.contains("boolean"), "key boolean not found");

    // if we put a JsonValue into the JsonObject and retrieve
    // it, it should be identical.
    Value value(QLatin1String("foo"));
    object.insert({"value", value});
    QCOMPARE(object.value("value"), value);

    int size = object.size();
    object.erase("boolean");
    QCOMPARE(object.size(), size - 1);
    QVERIFY2(!object.contains("boolean"), "key boolean should have been removed");

    QString before = object.value("string").value<QString>();
    object["string"] = QString::fromLatin1("foo");
    QVERIFY2(object.value("string").value<QString>() != before, "value should have been updated");

    size = object.size();
    Object subobject;
    subobject.insert({"number", 42});
    subobject.insert({QLatin1String("string"), QLatin1String("foobar")});
    object.insert({"subobject", subobject});
    QCOMPARE(object.size(), size+1);
    Value &subvalue = object[QLatin1String("subobject")];
    try {
        QCOMPARE(subvalue.get<Object>(), subobject);
    } catch (const std::exception &ex) {
        QVERIFY2(false, "Can't get Object");
    }
    Object &subojectRef = subvalue.get<Object>();
    subojectRef.insert({"double", 999.});
    QCOMPARE(subojectRef.size(), subobject.size() + 1);
    QCOMPARE(subojectRef, object[QLatin1String("subobject")].value<Object>());
}

void TestValue::benchArray()
{
    QString data = QStringLiteral("42");
    Array array;
    QBENCHMARK {
        array.push_back(data);
    }
}

void TestValue::benchQVariantList()
{
    QString data = QStringLiteral("42");
    QVariantList list;
    QBENCHMARK {
        list.append(data);
    }
}

QTEST_MAIN(TestValue)
#include "test_variant.moc"