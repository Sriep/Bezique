#ifndef CARD_H
#define CARD_H
#include <QQuickItem>
#include <QJsonObject>

class Card : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString image READ getImage WRITE setImage NOTIFY cardChanged)
    Q_PROPERTY(int rank READ getRank WRITE setRank NOTIFY cardChanged)
    Q_PROPERTY(int suit READ getSuit WRITE setSuit NOTIFY cardChanged)

    Q_PROPERTY(bool canMeld READ getCanMeld
               WRITE setCanMeld NOTIFY canMeldChanged)
    Q_PROPERTY(bool canPlay READ getCanPlay
               WRITE setCanPlay NOTIFY canPlayChanged)
    Q_PROPERTY(bool hasMarried READ getHasMarried
               WRITE setHasMarried NOTIFY hasMarriedChanged)
    Q_PROPERTY(bool hasBeziqued READ getHasBeziqued
               WRITE setHasBeziqued NOTIFY hasBeziquedChanged)
    Q_PROPERTY(bool hasDoubleBeziqued READ getHasDoubleBeziqued
               WRITE setHasDoubleBeziqued NOTIFY hasDoubleBeziquedChanged)
    Q_PROPERTY(bool hasFlushed READ getHasFlushed
               WRITE setHasFlushed NOTIFY hasFlushedChanged)
    Q_PROPERTY(bool hasFourKinded READ getHasFourKinded
               WRITE setHasFourKinded NOTIFY hasFourKindedChanged)

public:
    //Q_INVOKABLE void cardPlayed(int index, int x, int y);
    friend class BeziqueHand;

    enum Rank { Seven = 0, Eight, Nine, Jack, Queen, King, Ten, Ace, NumRanks };
    enum Suit { Diamonds = 0, Clubs, Hearts, Spades, NumSuits };
    static const int EMPTY = 8;

    const QString rankStr[8]  {"07", "08", "09", "11", "12", "13", "10", "01"};
    const QString rankName[9]  {"7", "8", "9", "J", "Q", "K", "T", " A", "#"};
    const QString suitStr[5]  {"d", "c", "h", "s", "#"};
    const QString emptyBitmap = "content/gfx/onePixelGreen.png";
    const QString backBitmap = "content/gfx/tinydeck/back111.gif";
    const int maxId = 63;

    Card();
    explicit Card(int iCard, QQuickItem *parent = 0);
    explicit Card(const Card* card, QQuickItem *parent = 0);
    Card(const Card& card, QQuickItem *parent = 0);

    bool beats(const Card& c, int trumps) const;
    bool beatsEnd(const Card& c, int trumps) const;

    void setCard(int cardId, int newLink = EMPTY);
    void copyCard(const Card& card);
    int getCardId() const;
    void clearCard();
    bool isCleard();

    // qml Properties access
    int getRank() const;
    int getSuit() const;
    QString getName() const;
    void setRank(int value);
    void setSuit(int value);
    QString getImage() const;
    void setImage(QString image);
    bool getCanMeld() const;
    void setCanMeld(bool value);

    int getLink() const;
    void setLink(int value);

    void setHasMarried(bool value);
    void setHasBeziqued(bool value);
    void setHasDoubleBeziqued(bool value);
    void setHasFlushed(bool value);
    void setHasFourKinded(bool value);

    bool getHasMarried() const;
    bool getHasBeziqued() const;
    bool getHasDoubleBeziqued() const;
    bool getHasFlushed() const;
    bool getHasFourKinded() const;

    void dump();

    void setCanSeven(bool value);
    void setCanMarry(bool value);
    void setCanFlush(bool value);
    void setCanBezique(bool value);
    void setCanDoubleBezique(bool value);
    void setCanFourKind(bool value);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool getCanPlay() const;
    void setCanPlay(bool value);

signals:
    void cardChanged();
    void canMeldChanged();
    void canPlayChanged();

    void hasMarriedChanged();
    void hasBeziquedChanged();
    void hasDoubleBeziquedChanged();
    void hasFlushedChanged();
    void hasFourKindedChanged();

public slots:
private:
    QString getFilename(int rank, int suit);
    void clearMeldStatus();
    void clearCanMeldStatus();

    // qml Properties
    int rank = Rank::NumRanks;
    int suit = Suit::NumSuits;
    QString imageFile = emptyBitmap;

    int link = EMPTY;
    int cardId;

    bool canMeld = false;
    bool canPlay = true;
    bool hasMarried = false;
    bool hasBeziqued = false;
    bool hasDoubleBeziqued = false;
    bool hasFlushed = false;
    bool hasFourKinded = false;

    bool canSeven = false;
    bool canMarry = false;
    bool canFlush = false;
    bool canBezique = false;
    bool canDoubleBezique = false;
    bool canFourKind = false;
};

#endif // CARD_H














































