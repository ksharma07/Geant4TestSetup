#include "ActionInitialization.hh"
#include "PrimaryGenerator.hh"

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGenerator());
}