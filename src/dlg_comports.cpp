//==========================================================================
// Name:            dlg_comports.cpp
// Purpose:         Creates simple wxWidgets dialog GUI to select
//                  real/virtual Comm ports.
// Date:            May 11 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include "dlg_comports.h"
#include "fdmdv2_hdw_ports.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
ComPortsDlg::ComPortsDlg(wxWindow* parent) : DlgComPorts(parent)
{
//    m_serialports = HdwPortList();
//    m_serialports.setType(PORT_TYPE_SERIAL);
}

//-------------------------------------------------------------------------
// OnListItemSelected()
//-------------------------------------------------------------------------
void ComPortsDlg::OnListItemSelected(wxListEvent& event)
{
    // TODO: Implement OnListItemSelected
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void ComPortsDlg::ExchangeData(int inout)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if(inout == EXCHANGE_DATA_IN)
    {
        m_textRigCtrlPort->SetValue(wxGetApp().m_strRigCtrlPort);
        m_textRigCtrlBaud->SetValue(wxGetApp().m_strRigCtrlBaud);
        m_textRigCtrlDatabits->SetValue(wxGetApp().m_strRigCtrlDatabits);
        m_textRigCtrlStopbits->SetValue(wxGetApp().m_strRigCtrlStopbits);
        m_textRigCtrlParity->SetValue(wxGetApp().m_strRigCtrlParity);
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
        wxGetApp().m_strRigCtrlPort             = m_textRigCtrlPort->GetValue();
        wxGetApp().m_strRigCtrlBaud             = m_textRigCtrlBaud->GetValue();
        wxGetApp().m_strRigCtrlDatabits         = m_textRigCtrlDatabits->GetValue();
        wxGetApp().m_strRigCtrlStopbits         = m_textRigCtrlStopbits->GetValue();
        wxGetApp().m_strRigCtrlParity           = m_textRigCtrlParity->GetValue();

        pConfig->Write(wxT("/Rig/Port"),        wxGetApp().m_strRigCtrlPort);
        pConfig->Write(wxT("/Rig/Baud"),        wxGetApp().m_strRigCtrlBaud);
        pConfig->Write(wxT("/Rig/DataBits"),    wxGetApp().m_strRigCtrlDatabits);
        pConfig->Write(wxT("/Rig/StopBits"),    wxGetApp().m_strRigCtrlStopbits);
        pConfig->Write(wxT("/Rig/Parity"),      wxGetApp().m_strRigCtrlParity);
        //m_textRigCtrlFlowControl

        pConfig->Flush();
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
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

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void ComPortsDlg::OnInitDialog(wxInitDialogEvent& event)
{
    ExchangeData(EXCHANGE_DATA_IN);
//    populateAudioInfo();
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
