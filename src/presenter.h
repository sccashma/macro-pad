/*
    presenter.h
    Author: Scott Cashman
    Date: 3/4/2025
*/

////// Some notes on how the presenter should work ////
/*
* The presenter is the intermediary between the model and the view
    It should manage the flow of data between the model and the view

* For example: button is hit > view calls presenter > presenter calls model > model returns data to presenter > presenter calls view with the data

* Has knowledge of both the view and model
*/

#ifndef _PRESENTER_H_
#define _PRESENTER_H_

#include "presenter_abstract.h"
#include "view_abstract.h"

#include "macro.h"
#include "model.h"
#include "view.h"

namespace presenter
{

class presenter_c : public presenter_abstract_c
{
private:
    model::model_c *m_model;
    view::view_abstract_c *m_view;

public:
    /// @brief Constructor
    /// @param model
    /// @param view
    presenter_c(model::model_c *model, view::view_abstract_c *view)
    {
        m_model = model;
        m_view = view;
        m_view->setPresenter(this);
    }

    /// @brief Run the application
    /// @details This invokes the main loop for the application
    void run()
    {
        m_view->run();
    }

    size_t handleLoadMacros(
        uint16_t const *ids, 
        size_t const size, 
        String *names, 
        String *file_paths, 
        macro::macro_c *macros
    )
    {
        return m_model->loadMacros(ids, size, names, file_paths, macros);
    }

    size_t handleQueryMacros(uint16_t *ids, String *names)
    {
        return m_model->queryMacros(ids, names);
    }
};

} // namespace presenter
#endif // _PRESENTER_H_