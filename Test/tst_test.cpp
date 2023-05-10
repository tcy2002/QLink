#include <QtTest>
#include "calculationmap.h"

// add necessary includes here

/*
 * 测试类
 * @路径覆盖
 */
class Test : public QObject
{
    Q_OBJECT

public:
    Test();
    ~Test();

private slots:
    /*
     * testMN
     * M: eliminateHorizonal输出点；
     * N: eliminateVertical输出点；
     * 每个测试用例分别对应一种条件执行路径。
     */
    void test11();
    void test12();
    void test13();
    void test14();
    void test15();
    void test2_();
    void test3_();
    void test4_();
    void test51();
    void test52();
    void test53();
    void test54();
    void test55();

};

Test::Test()
{

}

Test::~Test()
{

}

void Test::test11()
{
    CalculationMap map(2, 2);
    QVERIFY(map.eliminate(1, 0, 0, 1) == nullptr);
    QVERIFY(map.eliminate(0, 1, 1, 0) == nullptr);
}

void Test::test12()
{
    CalculationMap map(3, 3);
    map.setEmpty(0, 1);
    map.setEmpty(1, 1);
    map.setEmpty(2, 1);
    QVERIFY(map.eliminate(0, 0, 2, 2) != nullptr);
    QVERIFY(map.eliminate(2, 2, 0, 0) != nullptr);
}

void Test::test13()
{
    CalculationMap map(3, 2);
    map.setEmpty(2, 0);
    QVERIFY(map.eliminate(0, 0, 2, 1) != nullptr);
    QVERIFY(map.eliminate(2, 1, 0, 0) != nullptr);
}

void Test::test14()
{
    CalculationMap map(3, 2);
    map.setEmpty(2, 1);
    QVERIFY(map.eliminate(2, 0, 0, 1) != nullptr);
    QVERIFY(map.eliminate(0, 1, 2, 0) != nullptr);
}

void Test::test15()
{
    CalculationMap map(3, 3);
    QVERIFY(map.eliminate(0, 1, 2, 1) == nullptr);
    QVERIFY(map.eliminate(2, 1, 0, 1) == nullptr);
}

void Test::test2_()
{
    CalculationMap map(3, 2);
    map.setEmpty(1, 0);
    map.setEmpty(1, 1);
    QVERIFY(map.eliminate(0, 0, 2, 1) != nullptr);
    QVERIFY(map.eliminate(2, 1, 0, 0) != nullptr);
}

void Test::test3_()
{
    CalculationMap map(2, 3);
    map.setEmpty(0, 2);
    QVERIFY(map.eliminate(0, 0, 1, 2) != nullptr);
    QVERIFY(map.eliminate(1, 2, 0, 0) != nullptr);
}

void Test::test4_()
{
    CalculationMap map(2, 3);
    map.setEmpty(1, 2);
    QVERIFY(map.eliminate(1, 0, 0, 2) != nullptr);
    QVERIFY(map.eliminate(0, 2, 1, 0) != nullptr);
}

void Test::test51()
{
    CalculationMap map(3, 3);
    QVERIFY(map.eliminate(1, 0, 0, 1) == nullptr);
    QVERIFY(map.eliminate(0, 1, 1, 0) == nullptr);
}

void Test::test52()
{
    CalculationMap map(3, 4);
    map.setEmpty(1, 0);
    map.setEmpty(0, 1);
    map.setEmpty(2, 1);
    map.setEmpty(0, 2);
    map.setEmpty(1, 2);
    map.setEmpty(2, 2);
    map.setEmpty(1, 3);
    QVERIFY(map.eliminate(0, 0, 2, 3) != nullptr);
    QVERIFY(map.eliminate(2, 3, 0, 0) != nullptr);
}

void Test::test53()
{
    CalculationMap map(4, 3);
    map.setEmpty(1, 0);
    map.setEmpty(3, 0);
    map.setEmpty(0, 1);
    map.setEmpty(3, 1);
    map.setEmpty(1, 2);
    map.setEmpty(2, 2);
    QVERIFY(map.eliminate(0, 0, 3, 2) != nullptr);
    QVERIFY(map.eliminate(3, 2, 0, 0) != nullptr);
}

void Test::test54()
{
    CalculationMap map(4, 3);
    map.setEmpty(1, 0);
    map.setEmpty(2, 0);
    map.setEmpty(0, 1);
    map.setEmpty(3, 1);
    map.setEmpty(1, 2);
    map.setEmpty(3, 2);
    QVERIFY(map.eliminate(3, 0, 0, 2) != nullptr);
    QVERIFY(map.eliminate(0, 2, 3, 0) != nullptr);
}

void Test::test55()
{
    CalculationMap map(3, 3);
    map.setEmpty(1, 0);
    map.setEmpty(0, 1);
    map.setEmpty(2, 1);
    map.setEmpty(1, 2);
    QVERIFY(map.eliminate(0, 0, 2, 2) == nullptr);
    QVERIFY(map.eliminate(2, 2, 0, 0) == nullptr);
}

QTEST_APPLESS_MAIN(Test)

#include "tst_test.moc"
