#ifndef TEAM_H
#define TEAM_H

#include <QMetaType>

enum class Team {
    NONE,
    BLACK,
    WHITE
};

Q_DECLARE_METATYPE(Team);

#endif // TEAM_H
