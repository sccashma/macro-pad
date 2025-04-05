/*
    presenter_abstract.h
    Author: Scott Cashman
    Date: 5/4/2025
*/

#ifndef _PRESENTER_ABSTRACT_H_
#define _PRESENTER_ABSTRACT_H_

namespace presenter
{

class presenter_abstract_c
{
public:
    virtual size_t handleQueryMacros(uint16_t *, String *) = 0;
    virtual size_t handleLoadMacros(uint16_t const *, size_t const, String *, String *, macro::macro_c *) = 0;
    virtual size_t handleGetMacroOptions(size_t const, uint16_t *, String *, uint16_t const = 0) = 0;
    virtual void handleMinMaxID(uint16_t *, uint16_t *) = 0;
    virtual int16_t handleGetMacroCount() = 0;
};

} // namespace presenter
#endif // _PRESENTER_ABSTRACT_H_