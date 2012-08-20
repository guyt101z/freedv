//==========================================================================
// Name:            dlg_options.h
// Purpose:         Subclasses dialog GUI for general program options.
//                  (DlgOptions from topFrame.h)
// Created:         May. 11, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __COMPORTS_DIALOG__
#define __COMPORTS_DIALOG__

#include "topFrame.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class ComPortsDlg : public DlgComPorts
{
    protected:
        // Handlers for events.
        void OnListItemSelected( wxListEvent& event );
    public:
        /** Constructor */
        ComPortsDlg( wxWindow* parent );
};

#endif // __COMPORTS_DIALOG__
