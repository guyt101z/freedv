//==========================================================================
// Name:            dlg_comports.cpp
// Purpose:         Creates simple wxWidgets dialog GUI to select
//                  real/virtual Comm ports.
// Date:            May 11 2012
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
#include "dlg_comports.h"
#include "fdmdv2_hdw_ports.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
ComPortsDlg::ComPortsDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    wxStaticText* m_staticText8;
    wxStaticText* m_staticText9;
    wxStaticText* m_staticText91;
    wxStaticText* m_staticText911;
    wxStaticText* m_staticText912;
    wxStaticText* m_staticText913;
    wxStdDialogButtonSizer* m_sdbSizer5;

    wxBoxSizer* bSizer30;
    bSizer30 = new wxBoxSizer(wxVERTICAL);

    wxGridSizer* gSizer3;
    gSizer3 = new wxGridSizer(8, 2, 0, 0);

    m_staticText8 = new wxStaticText(this, wxID_ANY, _("Available Ports:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText8->Wrap(-1);
    gSizer3->Add(m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_listCtrlPorts = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    gSizer3->Add(m_listCtrlPorts, 2, wxALL|wxEXPAND, 2);

/*
    m_staticText9 = new wxStaticText(this, wxID_ANY, _("Use Port:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText9->Wrap(-1);
    gSizer3->Add(m_staticText9, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_textRigCtrlPort = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer3->Add(m_textRigCtrlPort, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);
*/

    m_staticText91 = new wxStaticText(this, wxID_ANY, _("Buad Rate:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText91->Wrap(-1);
    gSizer3->Add(m_staticText91, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_listCtrlBaudrates = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    gSizer3->Add(m_listCtrlBaudrates, 2, wxALL|wxEXPAND, 2);

    m_staticText911 = new wxStaticText(this, wxID_ANY, _("Data Bits:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText911->Wrap(-1);
    gSizer3->Add(m_staticText911, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_textRigCtrlDatabits = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer3->Add(m_textRigCtrlDatabits, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_staticText912 = new wxStaticText(this, wxID_ANY, _("Stop Bits:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText912->Wrap(-1);
    gSizer3->Add(m_staticText912, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_textRigCtrlStopbits = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer3->Add(m_textRigCtrlStopbits, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);
 
    m_staticText913 = new wxStaticText(this, wxID_ANY, _("Parity:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText913->Wrap(-1);
    gSizer3->Add(m_staticText913, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_textRigCtrlParity = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer3->Add(m_textRigCtrlParity, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    bSizer30->Add(gSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);

//    wxStaticBoxSizer* polaritySizer;
//    polaritySizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("")), wxHORIZONTAL);
//    gSizer3->Add(polaritySizer, 0, wxALIGN_CENTER|wxALL, 1);

    m_rbUseRTS = new wxRadioButton( this, wxID_ANY, wxT("Use RTS"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add(m_rbUseRTS, 0, wxALIGN_CENTER|wxALL, 1);

    m_ckRTSPos = new wxCheckBox( this, wxID_ANY, wxT("RTS = +V"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add(m_ckRTSPos, 0, wxALIGN_CENTER|wxALL, 1);

    m_rbUseDTR = new wxRadioButton( this, wxID_ANY, wxT("Use DTR"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add(m_rbUseDTR, 0, wxALIGN_CENTER|wxALL, 1);

    m_ckDTRPos = new wxCheckBox( this, wxID_ANY, wxT("DTR = +V"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add(m_ckDTRPos, 0, wxALIGN_CENTER|wxALL, 1);


    m_sdbSizer5 = new wxStdDialogButtonSizer();
    m_sdbSizer5OK = new wxButton(this, wxID_OK);
    m_sdbSizer5->AddButton(m_sdbSizer5OK);
    m_sdbSizer5Apply = new wxButton(this, wxID_APPLY);
    m_sdbSizer5->AddButton(m_sdbSizer5Apply);
    m_sdbSizer5Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer5->AddButton(m_sdbSizer5Cancel);
    m_sdbSizer5->Realize();

    bSizer30->Add(m_sdbSizer5, 0, wxEXPAND, 5);

    this->SetSizer(bSizer30);
    this->Layout();

    this->Centre(wxBOTH);
    this->Centre(wxBOTH);

    // Connect Events
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(ComPortsDlg::OnInitDialog));
    m_sdbSizer5Apply->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnApply), NULL, this);
    m_sdbSizer5Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnOK), NULL, this);
    m_listCtrlPorts->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ComPortsDlg::OnPortItemSelected), NULL, this);
    m_listCtrlBaudrates->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ComPortsDlg::OnBaudrateItemSelected), NULL, this);
/*
    void    OnApply(wxCommandEvent& event);
    void    OnRTSClick(wxCommandEvent& event);
    void    OnRTSPosClick(wxCommandEvent& event);
    void    OnDTRClick(wxCommandEvent& event);
    void    OnDTRPosClick(wxCommandEvent& event);
*/
}

//-------------------------------------------------------------------------
// ~ComPortsDlg()
//-------------------------------------------------------------------------
ComPortsDlg::~ComPortsDlg()
{
    // Disconnect Events
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(ComPortsDlg::OnInitDialog));
    m_listCtrlPorts->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ComPortsDlg::OnPortItemSelected), NULL, this);
    m_listCtrlBaudrates->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ComPortsDlg::OnBaudrateItemSelected), NULL, this);
    m_sdbSizer5Apply->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnApply), NULL, this);
    m_sdbSizer5Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnOK), NULL, this);
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void ComPortsDlg::OnInitDialog(wxInitDialogEvent& event)
{
    populatePortList();
    ExchangeData(EXCHANGE_DATA_IN);
}

//-------------------------------------------------------------------------
// populatePortList()
//-------------------------------------------------------------------------
void ComPortsDlg::populatePortList()
{
    int i = 0;
    wxListItem inf;
    wxString buf;
    long idx;
    long baudrates[] = { 110, 300, 1200, 2400, 9600, 19200, 38400, 57600, 115200 };
    
    m_listCtrlPorts->Clear();
    m_listCtrlBaudrates->Clear();
    for(i = 0; i < 9; i++)
    {
        buf.Printf("%u", baudrates[i]);
        m_listCtrlBaudrates->Append(buf);
    }
        
#ifdef  __WXMSW__
#define PREDICATE   
    for(i = 1; i <= 32; i++)
    {
        buf.Printf("COM%u:", i);
        idx = m_listCtrlPorts->Append(buf);
    }
    for(i = 33; i <= 128; i++)
    {
        buf.Printf("\\\\.\\com%u:", i);
        idx = m_listCtrlPorts->Append(buf);
    }

#endif

#ifdef  __WXGTK__
    for(i = 0; i < 32; i++)
    {
        buf.Printf("/dev/ttyS%u", i);
        idx = m_listCtrlPorts->Append(buf);
    }
    for(i = 0; i < 32; i++)
    {
        buf.Printf("/dev/ttyUSB%u", i);
        idx = m_listCtrlPorts->Append(buf);
    }
#endif

#ifdef __WXOSX__
#define MAXPORTS    20
#endif

}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void ComPortsDlg::ExchangeData(int inout)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString str;
    long l;
    int i;
    
    if(inout == EXCHANGE_DATA_IN)
    {
        str = wxGetApp().m_strRigCtrlPort;
        m_listCtrlPorts->SetStringSelection(str);

        str = wxGetApp().m_strRigCtrlBaud;
        m_listCtrlBaudrates->SetStringSelection(str);

        m_textRigCtrlDatabits->SetValue(wxGetApp().m_strRigCtrlDatabits);
        m_textRigCtrlStopbits->SetValue(wxGetApp().m_strRigCtrlStopbits);
        m_textRigCtrlParity->SetValue(wxGetApp().m_strRigCtrlParity);

        m_rbUseRTS->SetValue(wxGetApp().m_boolUseRTS);
        m_ckRTSPos->SetValue(wxGetApp().m_boolRTSPos);
        m_rbUseDTR->SetValue(wxGetApp().m_boolUseDTR);
        m_ckDTRPos->SetValue(wxGetApp().m_boolDTRPos);
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
        wxGetApp().m_strRigCtrlPort             = m_listCtrlPorts->GetStringSelection();
        wxGetApp().m_strRigCtrlBaud             = m_listCtrlBaudrates->GetStringSelection();

        wxGetApp().m_strRigCtrlDatabits         = m_textRigCtrlDatabits->GetValue();
        wxGetApp().m_strRigCtrlStopbits         = m_textRigCtrlStopbits->GetValue();
        wxGetApp().m_strRigCtrlParity           = m_textRigCtrlParity->GetValue();

        wxGetApp().m_boolUseRTS                  = m_rbUseRTS->GetValue();
        wxGetApp().m_boolRTSPos                  = m_ckRTSPos->GetValue();
        wxGetApp().m_boolUseDTR                  = m_rbUseDTR->GetValue();
        wxGetApp().m_boolDTRPos                  = m_ckDTRPos->GetValue();

        pConfig->Write(wxT("/Rig/Port"),        wxGetApp().m_strRigCtrlPort);
        pConfig->Write(wxT("/Rig/Baud"),        wxGetApp().m_strRigCtrlBaud);
        pConfig->Write(wxT("/Rig/DataBits"),    wxGetApp().m_strRigCtrlDatabits);
        pConfig->Write(wxT("/Rig/StopBits"),    wxGetApp().m_strRigCtrlStopbits);
        pConfig->Write(wxT("/Rig/Parity"),      wxGetApp().m_strRigCtrlParity);
        //m_textRigCtrlFlowControl

        pConfig->Write(wxT("/Rig/UseRTS"),      wxGetApp().m_boolUseRTS);
        pConfig->Write(wxT("/Rig/RTSPolarity"), wxGetApp().m_boolRTSPos);
        pConfig->Write(wxT("/Rig/UseDTR"),      wxGetApp().m_boolUseDTR);
        pConfig->Write(wxT("/Rig/DTRPolarity"), wxGetApp().m_boolDTRPos);

        pConfig->Flush();
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
}
/*
    void    OnApply(wxCommandEvent& event);
    void    OnRTSClick(wxCommandEvent& event);
    void    OnRTSPosClick(wxCommandEvent& event);
    void    OnDTRClick(wxCommandEvent& event);
    void    OnDTRPosClick(wxCommandEvent& event);
*/
//-------------------------------------------------------------------------
// OnPortItemSelected()
//-------------------------------------------------------------------------
void ComPortsDlg::OnPortItemSelected(wxListEvent& event)
{
    // TODO: Implement OnListItemSelected
}

//-------------------------------------------------------------------------
// OnBaudrateItemSelected()
//-------------------------------------------------------------------------
void ComPortsDlg::OnBaudrateItemSelected(wxListEvent& event)
{
    // TODO: Implement OnListItemSelected
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void ComPortsDlg::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnOK()
//-------------------------------------------------------------------------
void ComPortsDlg::OnOK(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnApply()
//-------------------------------------------------------------------------
void ComPortsDlg::OnApply(wxCommandEvent& event)
{
    ExchangeData(EXCHANGE_DATA_OUT);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void ComPortsDlg::OnClose(wxCloseEvent& event)
{
    this->EndModal(wxID_OK);
}

/*
//-------------------------------------------------------------------------
// serial_parity_handler()
//-------------------------------------------------------------------------
static void serial_parity_handler(union control *ctrl, void *dlg, void *data, int event)
{
    static const struct
    {
        const char *name;
        int val;
    }
    parities[] =
    {
        {"None",  SER_PAR_NONE},
        {"Odd",   SER_PAR_ODD},
        {"Even",  SER_PAR_EVEN},
        {"Mark",  SER_PAR_MARK},
        {"Space", SER_PAR_SPACE},
    };
    int mask = ctrl->listbox.context.i;
    int i, j;
    Config *cfg = (Config *)data;

    if (event == EVENT_REFRESH)
    {
        int oldparity = cfg->serparity; // preserve past reentrant calls
        dlg_update_start(ctrl, dlg);
        dlg_listbox_clear(ctrl, dlg);
        for (i = 0; i < lenof(parities); i++)
        {
            if (mask & (1 << i))
            dlg_listbox_addwithid(ctrl, dlg, parities[i].name, parities[i].val);
        }
        for (i = j = 0; i < lenof(parities); i++)
        {
            if (mask & (1 << i))
            {
                if (oldparity == parities[i].val)
                {
                    dlg_listbox_select(ctrl, dlg, j);
                    break;
                }
                j++;
            }
        }
        if (i == lenof(parities))
        {
            // an unsupported setting was chosen
            dlg_listbox_select(ctrl, dlg, 0);
            oldparity = SER_PAR_NONE;
        }
        dlg_update_done(ctrl, dlg);
        cfg->serparity = oldparity;
        // restore
    }
    else if (event == EVENT_SELCHANGE)
    {
        int i = dlg_listbox_index(ctrl, dlg);
        if (i < 0)
        {
            i = SER_PAR_NONE;
        }
        else
        {
            i = dlg_listbox_getid(ctrl, dlg, i);
        }
        cfg->serparity = i;
    }
}

//-------------------------------------------------------------------------
// serial_flow_handler()
//-------------------------------------------------------------------------
static void serial_flow_handler(union control *ctrl, void *dlg,	void *data, int event)
{
    static const struct
    {
        const char *name;
        int val;
    } flows[] =
    {
        {"None", SER_FLOW_NONE},
        {"XON/XOFF", SER_FLOW_XONXOFF},
        {"RTS/CTS", SER_FLOW_RTSCTS},
        {"DSR/DTR", SER_FLOW_DSRDTR},
    };
    int mask = ctrl->listbox.context.i;
    int i, j;
    Config *cfg = (Config *)data;

    if (event == EVENT_REFRESH)
    {
        int oldflow = cfg->serflow;    // preserve past reentrant calls
        dlg_update_start(ctrl, dlg);
        dlg_listbox_clear(ctrl, dlg);
        for (i = 0; i < lenof(flows); i++)
        {
            if (mask & (1 << i))
            dlg_listbox_addwithid(ctrl, dlg, flows[i].name, flows[i].val);
        }
        for (i = j = 0; i < lenof(flows); i++)
        {
            if (mask & (1 << i))
            {
                if (oldflow == flows[i].val)
                {
                    dlg_listbox_select(ctrl, dlg, j);
                    break;
                }
            j++;
            }
        }
        if (i == lenof(flows))
        {
            // an unsupported setting was chosen
            dlg_listbox_select(ctrl, dlg, 0);
            oldflow = SER_FLOW_NONE;
        }
        dlg_update_done(ctrl, dlg);
        cfg->serflow = oldflow;	       // restore
    }
    else if (event == EVENT_SELCHANGE)
    {
        int i = dlg_listbox_index(ctrl, dlg);
        if (i < 0)
        {
            i = SER_FLOW_NONE;
        }
        else
        {
            i = dlg_listbox_getid(ctrl, dlg, i);
        }
        cfg->serflow = i;
    }
}

//-------------------------------------------------------------------------
// ser_setup_config_box()
//-------------------------------------------------------------------------
void ser_setup_config_box(struct controlbox *b, int midsession, int parity_mask, int flow_mask)
{
    struct controlset *s;
    union control *c;

    if (!midsession)
    {
        int i;
        extern void config_protocolbuttons_handler(union control *, void *, void *, int);
        //
        // Add the serial back end to the protocols list at the
        // top of the config box.
        //
        s = ctrl_getset(b, "Session", "hostport", "Specify the destination you want to connect to");
        for (i = 0; i < s->ncontrols; i++)
        {
            c = s->ctrls[i];
            if (c->generic.type == CTRL_RADIO && c->generic.handler == config_protocolbuttons_handler)
            {
                c->radio.nbuttons++;
                c->radio.ncolumns++;
                c->radio.buttons = sresize(c->radio.buttons, c->radio.nbuttons, char *);
                c->radio.buttons[c->radio.nbuttons-1] = dupstr("Serial");
                c->radio.buttondata = sresize(c->radio.buttondata, c->radio.nbuttons, intorptr);
                c->radio.buttondata[c->radio.nbuttons-1] = I(PROT_SERIAL);
                if (c->radio.shortcuts)
                {
                    c->radio.shortcuts = sresize(c->radio.shortcuts, c->radio.nbuttons, char);
                    c->radio.shortcuts[c->radio.nbuttons-1] = 'r';
                }
            }
        }
    }
    //
    // Entirely new Connection/Serial panel for serial port
    // configuration.
    //
    ctrl_settitle(b, "Connection/Serial", "Options controlling local serial lines");
    if (!midsession)
    {
        //
        // We don't permit switching to a different serial port in
        // midflight, although we do allow all other
        // reconfiguration.
        //
        s = ctrl_getset(b, "Connection/Serial", "serline", "Select a serial line");
        ctrl_editbox(s, "Serial line to connect to", 'l', 40, HELPCTX(serial_line), dlg_stdeditbox_handler, I(offsetof(Config,serline)), I(sizeof(((Config *)0)->serline)));
    }

    s = ctrl_getset(b, "Connection/Serial", "sercfg", "Configure the serial line");
    ctrl_editbox(s, "Speed (baud)", 's', 40, HELPCTX(serial_speed), dlg_stdeditbox_handler, I(offsetof(Config,serspeed)), I(-1));
    ctrl_editbox(s, "Data bits", 'b', 40, HELPCTX(serial_databits), dlg_stdeditbox_handler,I(offsetof(Config,serdatabits)),I(-1));
    //
    // Stop bits come in units of one half.
    //
    ctrl_editbox(s, "Stop bits", 't', 40, HELPCTX(serial_stopbits), dlg_stdeditbox_handler,I(offsetof(Config,serstopbits)),I(-2));
    ctrl_droplist(s, "Parity", 'p', 40,	  HELPCTX(serial_parity),   serial_parity_handler, I(parity_mask));
    ctrl_droplist(s, "Flow control", 'f', 40, HELPCTX(serial_flow), serial_flow_handler,   I(flow_mask));
}
*/
