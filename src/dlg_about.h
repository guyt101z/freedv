//==========================================================================
// Name:            dlg_about.h
//
// Purpose:         Subclasses dialog GUI for Help/About.
//                 (DlgAbout from topFrame.h)
//
// Date:            May 05 2012
// Authors:         David Rowe, David Witten
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#ifndef __ABOUT_DIALOG__
#define __ABOUT_DIALOG__

#include "fdmdv2_main.h"

class AboutDlg : public DlgAbout
{
    protected:
        // Handlers for DlgAbout events.
        void OnInitDialog( wxInitDialogEvent& event );

    public:
        /** Constructor */
        AboutDlg( wxWindow* parent );
        void ExchangeData(int inout);
        void OnOK( wxCommandEvent& event );
};

#endif // __ABOUT_DIALOG__
