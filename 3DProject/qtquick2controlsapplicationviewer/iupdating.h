#ifndef IUPDATING_H
#define IUPDATING_H

class IUpdating
{
public:
    IUpdating();
    virtual void Update(float tickcount) = 0;
};

#endif // IUPDATING_H
