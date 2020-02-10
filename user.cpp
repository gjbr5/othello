#include "user.h"
#include <QDebug>

std::queue<size_t> User::inputQueue;
std::mutex User::inputMutex;
std::condition_variable User::cv;
std::atomic_bool User::userTurn = false;

User::User(BoardModel* bm)
    : bm(bm)
{
}

void User::pushIndex(size_t index)
{
    if (!userTurn)
        return;
    inputMutex.lock();
    inputQueue.emplace(index);
    inputMutex.unlock();

    cv.notify_all();
}

size_t User::nextDisk(const Board board)
{
    qDebug() << "User's Turn";
    while (!inputQueue.empty())
        inputQueue.pop();

    BitBoard placeable = board.allLegalMove(team);
    bm->setNextAvailable(placeable);

    userTurn = true;

    std::unique_lock<std::mutex> lk(inputMutex);
    size_t next;
    while (true) {
        cv.wait(lk, [&] { return !inputQueue.empty(); });

        next = inputQueue.front();
        inputQueue.pop();

        if (placeable.test(next))
            break;
    }
    userTurn = false;

    bm->setNextAvailable(BitBoard());
    return next;
}

User::~User() {}
