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

/// tmp ///
#include "macro.h"
///~tmp ///

namespace presenter
{

/// Store the current macro's
/// TODO: Store this to eeprom/SD and load it on startup.
uint16_t constexpr startup_macros[7] = {30, 8, 43, 47, 31, 32, 37};

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
        m_view->loadScreen();

        size_t num_startup_macros = MACRO_PLACE_OPTIONS;
        String macro_names[num_startup_macros];
        String macro_file_paths[num_startup_macros];
        macro::macro_c macros[num_startup_macros];
        size_t count = m_model->loadMacros(startup_macros, num_startup_macros, macro_names, macro_file_paths, macros);
        
        m_view->createHomeScreenMacroButtons(count, macros, macro_names, macro_file_paths);
        
        //////////////////////////////////////////////////////////////////////////
        // ******************************************************************** //
        // 2.1. What macros are needed by the view?                             //
        // 2.2. Get X macros info from model (by id???)                         //
        // 2.3. Create macro buttons in view                                    //
        // ******************************************************************** //
        //////////////////////////////////////////////////////////////////////////

        m_view->homeScreen();

        m_view->run();
    }

    /// @brief Handler to get the number of available macros
    /// @note: only used for testing purposes
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