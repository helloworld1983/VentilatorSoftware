#include <QtTest/QtTest>
#include "testsuite.h"

class DummyTest : public TestSuite
{
    Q_OBJECT

private slots:
    void testAddition();
};

void DummyTest::testAddition()
{
   double result = 1 + 1;
   QCOMPARE(result, 2.0);
}

static DummyTest dummyTestInstance;
#include "dummy.moc"
