//==========================================================================
// Name:            dlg_about.cpp
// Purpose:         Creates simple about dialog.
// Date:            Dec 06 2012
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
#include <wx/ffile.h>
#include <wx/dcmemory.h>
#include "dlg_about.h"
//#include "contributers.h"

AboutDlg::AboutDlg( wxWindow* parent, 
                    wxWindowID id, 
                    const wxString& title, 
                    const wxPoint& pos, 
                    const wxSize& size, 
                    long style) : wxDialog(parent, id, title, pos, size, style)
{
      this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* mainSizer;
    mainSizer = new wxBoxSizer(wxVERTICAL);
    
    m_notebook1 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    m_panel1 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);
    
//    m_bitmap = new wxStaticBitmap(m_panel1, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
//    bSizer3->Add(m_bitmap, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
    
    m_panel1->SetSizer(bSizer3);
    m_panel1->Layout();
    bSizer3->Fit(m_panel1);
    m_notebook1->AddPage(m_panel1, _("About"), true);
    m_panel4 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer(wxVERTICAL);
    
    m_htmlWin3 = new wxHtmlWindow(m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
    bSizer4->Add(m_htmlWin3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5);
    
    m_panel4->SetSizer(bSizer4);
    m_panel4->Layout();
    bSizer4->Fit(m_panel4);
    m_notebook1->AddPage(m_panel4, _("Credits"), false);
    m_panel3 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer5;
    bSizer5 = new wxBoxSizer(wxVERTICAL);
    
    m_textCtrlLicense = new wxTextCtrl(m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2);
    bSizer5->Add(m_textCtrlLicense, 1, wxEXPAND, 5);
    
    m_panel3->SetSizer(bSizer5);
    m_panel3->Layout();
    bSizer5->Fit(m_panel3);
    m_notebook1->AddPage(m_panel3, _("License"), false);
    
    mainSizer->Add(m_notebook1, 1, wxEXPAND | wxALL, 5);
    
    wxStaticBoxSizer* sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxEmptyString), wxVERTICAL);
    
    wxFlexGridSizer* fgSizer1;
    fgSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
    fgSizer1->SetFlexibleDirection(wxBOTH);
    fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    
    m_staticTextHomePage = new wxStaticText(this, wxID_ANY, _("Home Page:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticTextHomePage->Wrap(-1);
    m_staticTextHomePage->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString));
    
    fgSizer1->Add(m_staticTextHomePage, 1, wxALL|wxEXPAND, 5);
    
    m_hyperlink1 = new wxHyperlinkCtrl(this, wxID_ANY, _("http://freedv.org"), wxT("http://freedv.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    fgSizer1->Add(m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    m_staticText2 = new wxStaticText(this, wxID_ANY, _("freedv Forum:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    m_staticText2->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString));
    
    fgSizer1->Add(m_staticText2, 0, wxALL, 5);
    
    m_hyperlink2 = new wxHyperlinkCtrl(this, wxID_ANY, _("Google Group"), wxT("https://groups.google.com/forum/?fromgroups=#!forum/digitalvoice"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    fgSizer1->Add(m_hyperlink2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    m_staticText3 = new wxStaticText(this, wxID_ANY, _("Version:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText3->Wrap(-1);
    m_staticText3->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString));
    
    fgSizer1->Add(m_staticText3, 0, wxALL, 5);
    
    m_staticTextInformation = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_staticTextInformation->Wrap(-1);
    fgSizer1->Add(m_staticTextInformation, 0, wxALL, 5);
    
//    m_bitmapPayPal = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
//    fgSizer1->Add(m_bitmapPayPal, 0, wxALL, 5);
    
    m_hyperlink3 = new wxHyperlinkCtrl(this, wxID_ANY, _("Donate via PayPal"), wxT("https://sourceforge.net/donate/index.php?group_id=202033"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    fgSizer1->Add(m_hyperlink3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    sbSizer1->Add(fgSizer1, 1, wxEXPAND, 5);
    
    mainSizer->Add(sbSizer1, 0, wxEXPAND|wxALL, 5);
    
    wxBoxSizer* buttonSizer;
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_buttonOk = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0);
    m_buttonOk->SetDefault(); 
    buttonSizer->Add(m_buttonOk, 0, wxALL, 5);
    
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    
    this->SetSizer(mainSizer);
    this->Layout();
    mainSizer->Fit(this);
    this->Centre(wxBOTH);
    wxString svnLatestRev("Can't determine latest SVN revision.");

    // Try to determine current SVN revision from the Internet
    wxURL url(wxT("http://freetel.svn.sourceforge.net/svnroot/freetel/fdmdv2/"));
    
    if(url.GetError() == wxURL_NOERR)
    {
        wxString htmldata;
        wxInputStream *in = url.GetInputStream();
 
        if(in && in->IsOk())
        {
            //printf("In OK\n");
            wxStringOutputStream html_stream(&htmldata);
            in->Read(html_stream);
            //wxLogDebug(htmldata);
 
            wxString s("<h2>freetel - Revision ");
            int startIndex = htmldata.find(s) + s.Length();
            int endIndex = htmldata.find(wxT(": /fdmdv2</h2>"));
            svnLatestRev = wxT("Latest svn revision: ") + htmldata.SubString(startIndex, endIndex-1);
            //printf("startIndex: %d endIndex: %d\n", startIndex, endIndex);
       }
       delete in;
    }

    wxString msg;
    msg.Printf( wxT("FreeDV %s\n\n")
                wxT("Open Source Narrow Band Digital Voice over Radio\n\n")
                wxT("For Help and Support visit: http://freedv.org\n\n")
                wxT("GNU Public License V2.1\n\n")
                wxT("Copyright (c) David Witten KD0EAG and David Rowe VK5DGR\n\n")
                wxT("svn revision: %s\n") + svnLatestRev, FREEDV_VERSION, SVN_REVISION);

//    wxMessageBox(msg, wxT("About"), wxOK | wxICON_INFORMATION, this);
}

AboutDlg::~AboutDlg()
{
}
/*
AboutDlg::AboutDlg(wxWindow* parent, const wxString &mainTitle) : AboutDlgBase(parent)
{
    wxFileName splashscreen(ManagerST::Get()->GetStarupDirectory() +
                            wxFileName::GetPathSeparator() + 
                            wxT("images") + 
                            wxFileName::GetPathSeparator() + 
                            wxT("splashscreen.png"));

    m_bmp.LoadFile(splashscreen.GetFullPath(), wxBITMAP_TYPE_PNG);
    m_bitmap->SetBitmap(m_bmp);
    GetSizer()->Fit(this);
    
    BitmapLoader bmpLoader;
    m_bitmapPayPal->SetBitmap(bmpLoader.LoadBitmap(wxT("about/32/paypal")));
    
    // set the page content
    m_htmlWin3->SetPage(wxString::FromUTF8(about_hex));
    m_buttonOk->SetFocus();
    
    wxFileName license(ManagerST::Get()->GetInstallDir() + wxFileName::GetPathSeparator() + wxT("LICENSE"));
    wxString licenseFullname = license.GetFullPath();
    if(license.FileExists()) 
    {
        wxFFile fp(licenseFullname);
        if(fp.IsOpened()) 
        {
            wxString content;
            fp.ReadAll(&content, wxConvUTF8);
            fp.Close();
            
            m_textCtrlLicense->SetEditable(true);
            wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
            font.SetFamily(wxFONTFAMILY_TELETYPE);
            m_textCtrlLicense->SetFont(font);
            
            m_textCtrlLicense->ChangeValue(content);
            m_textCtrlLicense->SetEditable(false);
        }
    }
    CentreOnScreen();
}

AboutDlg::~AboutDlg()
{
}
*/

void AboutDlg::ExchangeData(int inout)
{
//    return m_staticTextInformation->GetLabelText();
}
