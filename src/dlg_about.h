//==========================================================================
// Name:            dlg_about.h
//
// Purpose:         Subclasses dialog GUI for Help/About.
//                 (DlgAbout from topFrame.h)
//
// Date:            May 05 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __ABOUT_DIALOG__
#define __ABOUT_DIALOG__

#include "topFrame.h"

class AboutDlg : public DlgAbout
{
    protected:
        // Handlers for DlgAbout events.
        void OnInitDialog( wxInitDialogEvent& event );

    public:
        /** Constructor */
        AboutDlg( wxWindow* parent );

};

#endif // __ABOUT_DIALOG__