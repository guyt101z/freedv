//==========================================================================
// Name:            dlg_about.h
// Purpose:         Provides an 'about' dialog.
// Created:         Dec 06, 2012
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
#ifndef __AboutDlg__
#define __AboutDlg__

#include "fdmdv2_main.h"
#include "wx/bitmap.h"
#include <wx/xrc/xmlres.h>
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/html/htmlwin.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/bitmap.h>

/** Implementing AboutDlg */
class AboutDlg : public wxDialog
{
   protected:
        wxNotebook* m_notebook1;
        wxPanel* m_panel1;
        wxStaticBitmap* m_bitmap;
        wxPanel* m_panel4;
        wxHtmlWindow* m_htmlWin3;
        wxPanel* m_panel3;
        wxTextCtrl* m_textCtrlLicense;
        wxStaticText* m_staticTextHomePage;
        wxHyperlinkCtrl* m_hyperlink1;
        wxStaticText* m_staticText2;
        wxHyperlinkCtrl* m_hyperlink2;
        wxStaticText* m_staticText3;
        wxStaticText* m_staticTextInformation;
        wxStaticBitmap* m_bitmapPayPal;
        wxHyperlinkCtrl* m_hyperlink3;
        wxButton* m_buttonOk;
        wxBitmap m_bmp;
    
    public:
        /** Constructor */
        AboutDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About freeDV"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
        virtual ~AboutDlg();
        void ExchangeData(int inout);
};

#endif // __AboutDlg__
