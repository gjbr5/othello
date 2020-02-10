#ifndef USER_H
#define USER_H

#include "boardmodel.h"
#include "player.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

class User : public Player {
    Q_OBJECT
private:
    static std::queue<size_t> inputQueue;
    static std::mutex inputMutex;
    static std::condition_variable cv;
    static std::atomic_bool userTurn;
    BoardModel* bm;

public:
    User(BoardModel* bm);
    ~User() override;
    size_t nextDisk(const Board board) override;

    static void pushIndex(size_t index);
};

#endif // USER_H
