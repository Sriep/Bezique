#ifndef TEST_H
#define TEST_H
#include <QQuickItem>
#include <QObject>

class Test : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString testProperty READ getTest WRITE setTest)
public:
    enum Rank { Seven = 0, Eight, Nine, Jack, Queen, King, Ten, Ace };
    enum Suit { Clubs = 0, Diamonds, Hearts, Spades };

    int rank;
    int suit;

    Test(QQuickItem *parent = 0);

    //Test(int rank, int suit, QQuickItem *parent = 0);
    //Test(int iCard, QQuickItem *parent = 0);
    //Test(const Card& c);

    QString getTest() const;
    void setTest(const QString &value);

signals:


public slots:

private:
    QString test;
};

#endif // TEST_H
