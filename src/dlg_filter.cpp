//==========================================================================
// Name:            dlg_filter.cpp
// Purpose:         Dialog for controlling Codec audio filtering
// Date:            Nov 25 2012
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
#include "dlg_filter.h"

#define SLIDER_MAX 100
#define SLIDER_LENGTH 155

#define MIN_MAG_DB -20.0
#define MAX_MAG_DB  20.0

extern struct CODEC2      *g_pCodec2;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class FilterDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
FilterDlg::FilterDlg(wxWindow* parent, bool running, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    m_running = running;

    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer30;
    bSizer30 = new wxBoxSizer(wxVERTICAL);

    // LPC Post Filter --------------------------------------------------------

    wxStaticBoxSizer* lpcpfs = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("LPC Post Filter")), wxHORIZONTAL);

    wxBoxSizer* left = new wxBoxSizer(wxVERTICAL);

    m_codec2LPCPostFilterEnable = new wxCheckBox(this, wxID_ANY, _("Enable"), wxDefaultPosition,wxDefaultSize, wxCHK_2STATE);
    left->Add(m_codec2LPCPostFilterEnable);

    m_codec2LPCPostFilterBassBoost = new wxCheckBox(this, wxID_ANY, _("0-1 kHz 3dB Boost"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    left->Add(m_codec2LPCPostFilterBassBoost);
    lpcpfs->Add(left, 0, wxALL, 5);

    wxBoxSizer* right = new wxBoxSizer(wxVERTICAL);

    newLPCPFControl(&m_codec2LPCPostFilterBeta, &m_staticTextBeta, right, "Beta");
    newLPCPFControl(&m_codec2LPCPostFilterGamma, &m_staticTextGamma, right, "Gamma");
    lpcpfs->Add(right, 0, wxALL, 5);

    bSizer30->Add(lpcpfs, 0, wxALL, 5);

    // EQ Filters -----------------------------------------------------------

    wxStaticBoxSizer* eqMicInSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Mic In Equaliser")), wxVERTICAL);

    m_MicInBass   = newEQ(eqMicInSizer, "Bass"  , disableQ);
    m_MicInMid    = newEQ(eqMicInSizer, "Mid"   , enableQ);
    m_MicInTreble = newEQ(eqMicInSizer, "Treble", disableQ);

    wxStaticBoxSizer* eqSpkOutSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Speaker Out Equaliser")), wxVERTICAL);

    m_SpkOutBass   = newEQ(eqSpkOutSizer, "Bass"  , disableQ);
    m_SpkOutMid    = newEQ(eqSpkOutSizer, "Mid"   , enableQ);
    m_SpkOutTreble = newEQ(eqSpkOutSizer, "Treble", disableQ);
    
    bSizer30->Add(eqMicInSizer, 0, wxALL|wxEXPAND, 5);
    bSizer30->Add(eqSpkOutSizer, 0, wxALL, 5);

    // Spectrum Plots -----------------------------------------------------------

    long nb_style = wxAUI_NB_BOTTOM | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS;
    m_auiNotebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(200,400), nb_style);
    m_auiNotebook->SetFont(wxFont(8, 70, 90, 90, false, wxEmptyString));

    bSizer30->Add(m_auiNotebook, 0, wxEXPAND|wxALL, 5);
    
    m_MicInFreqRespPlot = new PlotSpectrum((wxFrame*) m_auiNotebook, MIN_MAG_DB, MAX_MAG_DB);
    m_auiNotebook->AddPage(m_MicInFreqRespPlot, _("Microphone In Equaliser"));
    m_SpkOutFreqRespPlot = new PlotSpectrum((wxFrame*)m_auiNotebook, MIN_MAG_DB, MAX_MAG_DB);
    m_auiNotebook->AddPage(m_SpkOutFreqRespPlot, _("Speaker Out Equaliser"));

    // OK - Cancel - Default Buttons at the bottom --------------------------

    wxBoxSizer* bSizer31 = new wxBoxSizer(wxHORIZONTAL);
    m_sdbSizer5OK = new wxButton(this, wxID_OK);
    bSizer31->Add(m_sdbSizer5OK, 0, wxALL, 5);
    m_sdbSizer5Default = new wxButton(this, wxID_ANY, wxT("Default"));
    bSizer31->Add(m_sdbSizer5Default, 0, wxALL, 5);
    m_sdbSizer5Cancel = new wxButton(this, wxID_CANCEL);
    bSizer31->Add(m_sdbSizer5Cancel, 0, wxALL, 5);

    bSizer30->Add(bSizer31, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    this->SetSizer(bSizer30);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events -------------------------------------------------------

    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(FilterDlg::OnInitDialog));

    m_codec2LPCPostFilterEnable->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxScrollEventHandler(FilterDlg::OnEnable), NULL, this);
    m_codec2LPCPostFilterBassBoost->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxScrollEventHandler(FilterDlg::OnBassBoost), NULL, this);
    m_codec2LPCPostFilterBeta->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnBetaScroll), NULL, this);
    m_codec2LPCPostFilterGamma->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnGammaScroll), NULL, this);

    m_sdbSizer5Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnCancel), NULL, this);
    m_sdbSizer5Default->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnDefault), NULL, this);
    m_sdbSizer5OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnOK), NULL, this);
}

//-------------------------------------------------------------------------
// ~FilterDlg()
//-------------------------------------------------------------------------
FilterDlg::~FilterDlg()
{
    // Disconnect Events
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(FilterDlg::OnInitDialog));
    m_codec2LPCPostFilterBeta->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnBetaScroll), NULL, this);
    m_codec2LPCPostFilterGamma->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnGammaScroll), NULL, this);
    m_sdbSizer5Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnCancel), NULL, this);
    m_sdbSizer5Default->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnDefault), NULL, this);
    m_sdbSizer5OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnOK), NULL, this);
}

void FilterDlg::newLPCPFControl(wxSlider **slider, wxStaticText **stValue, wxSizer *s, wxString controlName)
{
    wxBoxSizer *bs = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* st = new wxStaticText(this, wxID_ANY, controlName, wxDefaultPosition, wxSize(70,-1), wxALIGN_RIGHT);
    bs->Add(st, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    *slider = new wxSlider(this, wxID_ANY, 0, 0, SLIDER_MAX, wxDefaultPosition, wxSize(SLIDER_LENGTH,wxDefaultCoord));
    bs->Add(*slider, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    *stValue = new wxStaticText(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    bs->Add(*stValue, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 2);

    s->Add(bs, 1);
}

wxSlider *FilterDlg::newEQControl(wxStaticBoxSizer *bs, wxString controlName)
{
    wxStaticText* st = new wxStaticText(this, wxID_ANY, controlName, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    bs->Add(st, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    wxSlider* slider = new wxSlider(this, wxID_ANY, 0, 0, SLIDER_MAX, wxDefaultPosition, wxSize(SLIDER_LENGTH,wxDefaultCoord));
    bs->Add(slider, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    return slider;
}

EQ FilterDlg::newEQ(wxStaticBoxSizer *bs, wxString eqName, bool enableQ)
{
    EQ eq;

    wxStaticBoxSizer *bsEQ = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, eqName), wxHORIZONTAL);

    eq.sliderFreq = newEQControl(bsEQ, "Freq"); eq.sliderFreqId = eq.sliderFreq->GetId();
    eq.sliderGain = newEQControl(bsEQ, "Gain");
    if (enableQ)
        eq.sliderQ = newEQControl(bsEQ, "Q");
    else
        eq.sliderQ = NULL;

    bs->Add(bsEQ);

    return eq;
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void FilterDlg::ExchangeData(int inout)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if(inout == EXCHANGE_DATA_IN)
    {
        //printf("EXCHANGE_DATA_IN\n");
        m_codec2LPCPostFilterEnable->SetValue(wxGetApp().m_codec2LPCPostFilterEnable);
        m_codec2LPCPostFilterBassBoost->SetValue(wxGetApp().m_codec2LPCPostFilterBassBoost);
        m_beta = wxGetApp().m_codec2LPCPostFilterBeta; setBeta();
        m_gamma = wxGetApp().m_codec2LPCPostFilterGamma; setGamma();
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
        //printf("EXCHANGE_DATA_OUT\n");
        wxGetApp().m_codec2LPCPostFilterEnable     = m_codec2LPCPostFilterEnable->GetValue();
        wxGetApp().m_codec2LPCPostFilterBassBoost  = m_codec2LPCPostFilterBassBoost->GetValue();
        wxGetApp().m_codec2LPCPostFilterBeta       = m_beta;
        wxGetApp().m_codec2LPCPostFilterGamma      = m_gamma;

        pConfig->Write(wxT("/Filter/codec2LPCPostFilterEnable"),     wxGetApp().m_codec2LPCPostFilterEnable);
        pConfig->Write(wxT("/Filter/codec2LPCPostFilterBassBoost"),  wxGetApp().m_codec2LPCPostFilterBassBoost);
        pConfig->Write(wxT("/Filter/codec2LPCPostFilterBeta"),       m_beta*100.0);
        pConfig->Write(wxT("/Filter/codec2LPCPostFilterGamma"),      m_gamma*100.0);

        pConfig->Flush();
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void FilterDlg::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnDefault()
//-------------------------------------------------------------------------
void FilterDlg::OnDefault(wxCommandEvent& event)
{
    m_beta = CODEC2_LPC_PF_BETA; setBeta();
    m_gamma = CODEC2_LPC_PF_GAMMA; setGamma();
    m_codec2LPCPostFilterEnable->SetValue(true);
    m_codec2LPCPostFilterBassBoost->SetValue(true);
}

//-------------------------------------------------------------------------
// OnOK()
//-------------------------------------------------------------------------
void FilterDlg::OnOK(wxCommandEvent& event)
{
    //printf("FilterDlg::OnOK\n");
    ExchangeData(EXCHANGE_DATA_OUT);
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void FilterDlg::OnClose(wxCloseEvent& event)
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void FilterDlg::OnInitDialog(wxInitDialogEvent& event)
{
    //printf("FilterDlg::OnInitDialog\n");
    ExchangeData(EXCHANGE_DATA_IN);
    //printf("m_beta: %f\n", m_beta);
}

void FilterDlg::setBeta(void) {
    wxString buf;
    buf.Printf(wxT("%3.2f"), m_beta);
    m_staticTextBeta->SetLabel(buf);
    int slider = (int)(m_beta*SLIDER_MAX + 0.5);
    m_codec2LPCPostFilterBeta->SetValue(slider);
}

void FilterDlg::setCodec2(void) {
    if (m_running) {
        assert(g_pCodec2 != NULL);
        codec2_set_lpc_post_filter(g_pCodec2, 
                               m_codec2LPCPostFilterEnable->GetValue(), 
                               m_codec2LPCPostFilterBassBoost->GetValue(), 
                               m_beta, m_gamma);
    }
}

void FilterDlg::setGamma(void) {
    wxString buf;
    buf.Printf(wxT("%3.2f"), m_gamma);
    m_staticTextGamma->SetLabel(buf);
    int slider = (int)(m_gamma*SLIDER_MAX + 0.5);
    m_codec2LPCPostFilterGamma->SetValue(slider);
}

void FilterDlg::OnEnable(wxScrollEvent& event) {
    setCodec2();    
}

void FilterDlg::OnBassBoost(wxScrollEvent& event) {
    setCodec2();    
}

void FilterDlg::OnBetaScroll(wxScrollEvent& event) {
    m_beta = (float)m_codec2LPCPostFilterBeta->GetValue()/SLIDER_MAX;
    setBeta();
    setCodec2();
}

void FilterDlg::OnGammaScroll(wxScrollEvent& event) {
    m_gamma = (float)m_codec2LPCPostFilterGamma->GetValue()/SLIDER_MAX;
    setGamma();
    setCodec2();
}

