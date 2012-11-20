//==========================================================================
// Name:            dlg_options.h
// Purpose:         Subclasses dialog GUI for general program options.
//                  (DlgOptions from topFrame.h)
// Created:         May. 11, 2012
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
#ifndef __COMPORTS_DIALOG__
#define __COMPORTS_DIALOG__

#include "fdmdv2_main.h"
#include "fdmdv2_hdw_ports.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class ComPortsDlg : public wxDialog
{
    public:
        /** Constructor */
//        ComPortsDlg(wxWindow* parent);
//        ~ComPortsDlg();
        ComPortsDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select Com Port"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 399,354 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~ComPortsDlg();

        void    ExchangeData(int inout);
        //ArrayOfPorts m_serialports;

    protected:
        // Handlers for events.
        void    OnListItemSelected(wxListEvent& event);
        void    OnCancel(wxCommandEvent& event);
        void    OnOK(wxCommandEvent& event);
        void    OnClose(wxCloseEvent& event);
        void    OnInitDialog(wxInitDialogEvent& event);
        void    OnApply(wxCommandEvent& event);

        wxStaticText* m_staticText8;
        wxListCtrl* m_listCtrlPortSelect;
        wxStaticText* m_staticText9;
        wxTextCtrl* m_textRigCtrlPort;
        wxStaticText* m_staticText91;
        wxTextCtrl* m_textRigCtrlBaud;
        wxStaticText* m_staticText911;
        wxTextCtrl* m_textRigCtrlDatabits;
        wxStaticText* m_staticText912;
        wxTextCtrl* m_textRigCtrlStopbits;
        wxStaticText* m_staticText913;
        wxTextCtrl* m_textRigCtrlParity;
        wxStdDialogButtonSizer* m_sdbSizer5;
        wxButton* m_sdbSizer5OK;
        wxButton* m_sdbSizer5Apply;
        wxButton* m_sdbSizer5Cancel;
};

#endif // __COMPORTS_DIALOG__
