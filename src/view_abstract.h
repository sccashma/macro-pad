/*
    view.h
    Author: Scott Cashman
    Date: 5/4/2025
*/

#ifndef _VIEW_ABSTRACT_H_
#define _VIEW_ABSTRACT_H_

#include "macro.h"
#include "presenter_abstract.h"


namespace view
{
using buttonCallback = void (*)(void*);

class view_abstract_c
{
public:
    virtual void run() = 0;
    virtual void createHomeScreenMacroButtons(size_t const, macro::macro_c const *, String const *, String const *) = 0;
    virtual void setPresenter(presenter::presenter_abstract_c *) = 0;
    virtual void renderTestScreen() = 0;
    virtual void setCallback(buttonCallback, void*) = 0;
};

} // namespace view
#endif // _VIEW_ABSTRACT_H_
