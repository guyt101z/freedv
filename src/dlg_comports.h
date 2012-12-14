//==========================================================================
// Name:            dlg_comports.h
// Purpose:         Subclasses dialog GUI for PTT Config.
//                  
// Created:         May. 11, 2012
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
#ifndef __COMPORTS_DIALOG__
#define __COMPORTS_DIALOG__

#include "fdmdv2_main.h"
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#include <wx/button.h>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class ComPortsDlg : public wxDialog
{
     public:
        ComPortsDlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("PTT Config"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(450,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
        virtual ~ComPortsDlg();
        void    ExchangeData(int inout);

    protected:
        wxCheckBox* m_ckHalfDuplex;
        wxCheckBox* m_ckUsePTTRtChan;
        wxListBox* m_listCtrlPorts;
        wxCheckBox* m_ckUseSerialPTT;
        wxStaticText* m_staticText12;
        wxTextCtrl *m_txtCtlDevicePath;
        wxRadioButton* m_rbUseDTR;
        wxCheckBox* m_ckRTSPos;
        wxRadioButton* m_rbUseRTS;
        wxCheckBox* m_ckDTRPos;
        wxButton* m_buttonOK;
        wxButton* m_buttonCancel;
        wxButton* m_buttonApply;

protected:
        void ListAllPorts();

        void populatePortList(void);
        void PTTAudioClick(wxCommandEvent& event);
//        void PTTPortSlelcted(wxCommandEvent& event);
        void PTTUseSerialClicked(wxCommandEvent& event);
        void UseDTRCliched(wxCommandEvent& event);
        //void DTRVPlusClicked(wxCommandEvent& event);
        void UseRTSClicked(wxCommandEvent& event);
        //void RTSVPlusClicked(wxCommandEvent& event);
        void OnOK(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnApply(wxCommandEvent& event);
        virtual void OnInitDialog(wxInitDialogEvent& event);
};

#endif // __COMPORTS_DIALOG__
