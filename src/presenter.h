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

#include "model.h"
#include "view.h"

namespace presenter
{

class presenter_c
{
public:
    /// @brief Constructor
    /// @param model
    /// @param view
    presenter_c(model::model_c *model, view::view_c *view)
    {
        m_model = model;
        m_view = view;
    }

    /// @brief Run the application
    /// @details This invokes the main loop for the application
    void run()
    {
        m_view->setCallback(handleAvailableMacros, this);
        m_view->renderTestScreen();
        m_view->run();
    }

    /// @brief Handler to get the number of available macros
    static void handleAvailableMacros(void *obj)
    {
        if (obj) static_cast<presenter_c*>(obj)->_availableMacros();
    }
    
private:
    model::model_c *m_model;
    view::view_c *m_view;
    
    /// @brief Get the number of available macros
    void _availableMacros()
    {
        uint16_t count = m_model->availableMacros();
        String message = String("Available Macros: ") + count;
        m_view->displayMessage(message.c_str());
    }
};

} // namespace presenter
#endif // _PRESENTER_H_