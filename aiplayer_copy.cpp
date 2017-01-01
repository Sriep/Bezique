#include "aiplayer.h"

AiPlayer::AiPlayer()// QQuickItem *parent)
    : Player()
{
    init();
}

bool AiPlayer::isControlled() const
{
    return false;
}

void AiPlayer::init()
{
   // hand = new BeziqueHand(true);
}
