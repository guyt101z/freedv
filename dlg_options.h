//==========================================================================
// Name:            dlg_options.h
//
// Purpose:         Subclasses dialog GUI for general program options.
//                  (DlgOptions from topFrame.h)
//
// Date:            May 05 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __OPTIONS_DIALOG__
#define __OPTIONS_DIALOG__

#include "topFrame.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class OptionsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class OptionsDlg : public DlgOptions
{
    protected:
        // Handlers for DlgOptions events.
        void OnClose( wxCloseEvent& event );
        void OnInitDialog( wxInitDialogEvent& event );
    public:
        /** Constructor */
        OptionsDlg( wxWindow* parent );
};

#endif // __OPTIONS_DIALOG__