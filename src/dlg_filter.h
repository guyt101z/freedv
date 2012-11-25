//==========================================================================
// Name:            dlg_filter.h
// Purpose:         Dialog for controlling Codec audio filtering
// Created:         Nov 25 2012
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

#ifndef __FILTER_DIALOG__
#define __FILTER_DIALOG__

#include "fdmdv2_main.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class FilterDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class FilterDlg : public wxDialog
{
    public:
        FilterDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Filter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,250 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~FilterDlg();

        void    ExchangeData(int inout);

    protected:
        // Handlers for events.
        void    OnCancel(wxCommandEvent& event);
        void    OnOK(wxCommandEvent& event);
        void    OnClose(wxCloseEvent& event);
        void    OnInitDialog(wxInitDialogEvent& event);
        void    OnBetaScroll(wxScrollEvent& event);
        void    OnGammaScroll(wxScrollEvent& event);

        wxStaticText* m_staticText8;
        wxCheckBox*   m_codec2LPCPostFilterEnable;
        wxStaticText* m_staticText9;
        wxCheckBox*   m_codec2LPCPostFilterBassBoost;
        wxStaticText* m_staticText91;
        wxSlider*     m_codec2LPCPostFilterBeta;
        wxStaticText* m_staticTextBeta;
        wxStaticText* m_staticText911;
        wxSlider*     m_codec2LPCPostFilterGamma;
        wxStaticText* m_staticTextGamma;

        wxStdDialogButtonSizer* m_sdbSizer5;
        wxButton*     m_sdbSizer5OK;
        wxButton*     m_sdbSizer5Apply;
        wxButton*     m_sdbSizer5Cancel;
};

#endif // __FILTER_DIALOG__
