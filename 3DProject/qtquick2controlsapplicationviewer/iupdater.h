#ifndef IUPDATER_H
#define IUPDATER_H

#include <boost/signals2.hpp>

class IUpdater
{
public:
    boost::signals2::signal<void(float)> update;
};

#endif // IUPDATER_H
