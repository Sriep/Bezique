#include "test.h"

Test::Test(QQuickItem *parent)
    : QQuickItem(parent)
{
}

QString Test::getTest() const
{
    return test;
}

void Test::setTest(const QString &value)
{
    test = value;
}

