#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "Generator.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
    public:
        MyActionInitialization();
        ~MyActionInitialization();

        virtual void Build() const;
};

#endif