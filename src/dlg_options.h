//==========================================================================
// Name:            dlg_options.h
//
// Purpose:         Subclasses dialog GUI for general program options.
//                  (DlgOptions from topFrame.h)
//
// Date:            May 05 2012
// Authors:         David Rowe, David Witten
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#ifndef __OPTIONS_DIALOG__
#define __OPTIONS_DIALOG__

#include "fdmdv2_main.h"

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
        void ExchangeData(int inout);
        void OnCancel( wxCommandEvent& event );
        void OnOK( wxCommandEvent& event );
        void OnApply( wxCommandEvent& event );
};

#endif // __OPTIONS_DIALOG__
