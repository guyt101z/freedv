///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "topFrame.h"

//=========================================================================
// Code that lays out the main application window
//=========================================================================
TopFrame::TopFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    //=====================================================
    // Menubar Setup
    m_menubarMain = new wxMenuBar(wxMB_DOCKABLE);
    file = new wxMenu();
    wxMenuItem* m_menuItemOpen;
    m_menuItemOpen = new wxMenuItem(file, ID_OPEN, wxString(_("&Open")) , _("Open File"), wxITEM_NORMAL);
    file->Append(m_menuItemOpen);

    wxMenuItem* m_menuItemSave;
    m_menuItemSave = new wxMenuItem(file, ID_SAVE, wxString(_("&Save")) , _("Save current file"), wxITEM_NORMAL);
    file->Append(m_menuItemSave);

    wxMenuItem* m_menuItemClose;
    m_menuItemClose = new wxMenuItem(file, ID_CLOSE, wxString(_("&Close")) , _("Close current file"), wxITEM_NORMAL);
    file->Append(m_menuItemClose);

    file->AppendSeparator();

    wxMenuItem* m_menuItemExit;
    m_menuItemExit = new wxMenuItem(file, ID_EXIT, wxString(_("E&xit")) , _("Exit Program"), wxITEM_NORMAL);
    file->Append(m_menuItemExit);

    m_menubarMain->Append(file, _("&File"));

    edit = new wxMenu();
    wxMenuItem* m_menuItemCopy;
    m_menuItemCopy = new wxMenuItem(edit, ID_COPY, wxString(_("&Copy")) , _("Copy selection"), wxITEM_NORMAL);
    edit->Append(m_menuItemCopy);
    m_menuItemCopy->Enable(false);

    wxMenuItem* m_menuItemCut;
    m_menuItemCut = new wxMenuItem(edit, ID_CUT, wxString(_("Cut")) , _("Cut Selection"), wxITEM_NORMAL);
    edit->Append(m_menuItemCut);
    m_menuItemCut->Enable(false);

    wxMenuItem* m_menuItemPaste;
    m_menuItemPaste = new wxMenuItem(edit, ID_PASTE, wxString(_("&Paste")) , _("Paste selection"), wxITEM_NORMAL);
    edit->Append(m_menuItemPaste);
    m_menuItemPaste->Enable(false);

    m_menubarMain->Append(edit, _("&Edit"));

    tools = new wxMenu();
    wxMenuItem* m_menuItemAudio;
    m_menuItemAudio = new wxMenuItem(tools, wxID_ANY, wxString(_("&Audio Config")) , wxEmptyString, wxITEM_NORMAL);
    tools->Append(m_menuItemAudio);

    wxMenuItem* m_menuItemRigCtrlCfg;
    m_menuItemRigCtrlCfg = new wxMenuItem(tools, wxID_ANY, wxString(_("Rig Control Config")) , wxEmptyString, wxITEM_NORMAL);
    tools->Append(m_menuItemRigCtrlCfg);

    wxMenuItem* m_menuItemOptions;
    m_menuItemOptions = new wxMenuItem(tools, ID_OPTIONS, wxString(_("Other Program &Options")) , _("Set preferences"), wxITEM_NORMAL);
    tools->Append(m_menuItemOptions);

    tools->AppendSeparator();

    wxMenuItem* m_menuItemCaptRxInStream;
    m_menuItemCaptRxInStream = new wxMenuItem(tools, wxID_ANY, wxString(_("Capture Rx Input Stream")), wxEmptyString, wxITEM_NORMAL);
#ifdef __WXMSW__
    m_menuItemCaptRxInStream->SetBitmaps(wxNullBitmap);
#elif defined(__WXGTK__)
    m_menuItemCaptRxInStream->SetBitmap(wxNullBitmap);
#endif
    tools->Append(m_menuItemCaptRxInStream);

    wxMenuItem* m_menuItemCaptTxInStream;
    m_menuItemCaptTxInStream = new wxMenuItem(tools, wxID_ANY, wxString(_("Capture Tx Input Stream")), wxEmptyString, wxITEM_NORMAL);
#ifdef __WXMSW__
    m_menuItemCaptTxInStream->SetBitmaps(wxNullBitmap);
#elif defined(__WXGTK__)
    m_menuItemCaptTxInStream->SetBitmap(wxNullBitmap);
#endif
    tools->Append(m_menuItemCaptTxInStream);

    wxMenuItem* m_menuItemCaptRxOutStream;
    m_menuItemCaptRxOutStream = new wxMenuItem(tools, wxID_ANY, wxString(_("Capture Rx Output Stream")), wxEmptyString, wxITEM_NORMAL);
#ifdef __WXMSW__
    m_menuItemCaptRxOutStream->SetBitmaps(wxNullBitmap);
#elif defined(__WXGTK__)
    m_menuItemCaptRxOutStream->SetBitmap(wxNullBitmap);
#endif
    tools->Append(m_menuItemCaptRxOutStream);

    wxMenuItem* m_menuItemCaptTxOutStream;
    m_menuItemCaptTxOutStream = new wxMenuItem(tools, wxID_ANY, wxString(_("Capture Tx Output Stream")), wxEmptyString, wxITEM_NORMAL);
#ifdef __WXMSW__
    m_menuItemCaptTxOutStream->SetBitmaps(wxNullBitmap);
#elif defined(__WXGTK__)
    m_menuItemCaptTxOutStream->SetBitmap(wxNullBitmap);
#endif
    tools->Append(m_menuItemCaptTxOutStream);

    tools->AppendSeparator();

    wxMenuItem* m_menuItemPlayAudioFile;
    m_menuItemPlayAudioFile = new wxMenuItem(tools, wxID_ANY, wxString(_("Play File")) , wxEmptyString, wxITEM_NORMAL);
    tools->Append(m_menuItemPlayAudioFile);

    m_menubarMain->Append(tools, _("&Tools"));

    help = new wxMenu();
    wxMenuItem* m_menuItemHelpUpdates;
    m_menuItemHelpUpdates = new wxMenuItem(help, wxID_ANY, wxString(_("Check for Updates")) , wxEmptyString, wxITEM_NORMAL);
    help->Append(m_menuItemHelpUpdates);
    m_menuItemHelpUpdates->Enable(false);

    wxMenuItem* m_menuItemAbout;
    m_menuItemAbout = new wxMenuItem(help, ID_ABOUT, wxString(_("&About")) , _("About this program"), wxITEM_NORMAL);
    help->Append(m_menuItemAbout);

    m_menubarMain->Append(help, _("&Help"));

    this->SetMenuBar(m_menubarMain);

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer(wxHORIZONTAL);

    //=====================================================
    // Left side
    //=====================================================
    wxBoxSizer* leftSizer;
    leftSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* snrSizer;
    snrSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("SNR")), wxVERTICAL);

    //------------------------------
    // S/N ratio Guage (vert. bargraph)
    //------------------------------
    m_gaugeSNR = new wxGauge(this, wxID_ANY, 20, wxDefaultPosition, wxSize(15,135), wxGA_SMOOTH|wxGA_VERTICAL);
    m_gaugeSNR->SetToolTip(_("Displays signal to noise ratio in dB."));
    snrSizer->Add(m_gaugeSNR, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);
    wxBoxSizer* bSizer29;
    bSizer29 = new wxBoxSizer(wxVERTICAL);

    //------------------------------
    // Box for S/N ratio (Numeric)
    //------------------------------
    m_textSNR = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    //m_textSNR = new wxStaticText(this, wxID_ANY, wxEmptyString, wxPoint(-1,-1), wxSize(35,25), wxTE_READONLY);
    m_textSNR->SetToolTip(_("Show SNR ratio numerically."));
    m_textSNR->SetMinSize(wxSize(35,25));
    bSizer29->Add(m_textSNR, 0, wxALIGN_CENTER|wxALL, 1);
    snrSizer->Add(bSizer29, 0, wxEXPAND, 1);
    leftSizer->Add(snrSizer, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 1);
    wxStaticBoxSizer* levelSizer;
    levelSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Level")), wxVERTICAL);

    //------------------------------
    // Signal Level(vert. bargraph)
    //------------------------------
    m_gaugeLevel = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxSize(15,135), wxGA_SMOOTH|wxGA_VERTICAL);
    m_gaugeLevel->SetToolTip(_("Display signal level."));
    levelSizer->Add(m_gaugeLevel, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);
    leftSizer->Add(levelSizer, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 1);
    bSizer1->Add(leftSizer, 0, wxALL|wxEXPAND, 5);

    //=====================================================
    // Center Section
    //=====================================================
    wxBoxSizer* centerSizer;
    centerSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* upperSizer;
    upperSizer = new wxBoxSizer(wxVERTICAL);

    //=====================================================
    // Tabbed Notebppk control containing display graphs
    //=====================================================
    //m_auiNbookCtrl = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_BOTTOM|wxAUI_NB_DEFAULT_STYLE);
    //long style = wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_MIDDLE_CLICK_CLOSE;
    long nb_style = wxAUI_NB_BOTTOM | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS;
    m_auiNbookCtrl = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nb_style);
    // This line sets the fontsize for the tabs on the notebook control
    m_auiNbookCtrl->SetFont(wxFont(8, 70, 90, 90, false, wxEmptyString));

    upperSizer->Add(m_auiNbookCtrl, 1, wxALIGN_TOP|wxEXPAND, 1);
    centerSizer->Add(upperSizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_TOP|wxEXPAND, 0);
    wxBoxSizer* lowerSizer;
    lowerSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* bSizer15;
    bSizer15 = new wxBoxSizer(wxVERTICAL);
    m_txtCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizer15->Add(m_txtCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5);
    lowerSizer->Add(bSizer15, 1, wxEXPAND, 5);
    wxBoxSizer* bSizer141;
    bSizer141 = new wxBoxSizer(wxHORIZONTAL);

    //=====================================================
    // These are the buttons that autosend the userid (?)
    //=====================================================

    // RxID
    //---------
    m_togRxID = new wxToggleButton(this, wxID_ANY, _("RxID"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer141->Add(m_togRxID, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL|wxFIXED_MINSIZE, 5);

    // TxID
    //---------
    m_togTxID = new wxToggleButton(this, wxID_ANY, _("TxID"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer141->Add(m_togTxID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lowerSizer->Add(bSizer141, 0, wxALIGN_RIGHT, 5);
    centerSizer->Add(lowerSizer, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 2);
    bSizer1->Add(centerSizer, 4, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 1);

    //=====================================================
    // Right side
    //=====================================================
    wxBoxSizer* rightSizer;
    rightSizer = new wxBoxSizer(wxVERTICAL);

    //=====================================================
    // Squelch Slider Control
    //=====================================================
    wxStaticBoxSizer* sbSizer3;
    sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Squelch")), wxVERTICAL);

    m_sliderSQ = new wxSlider(this, wxID_ANY, 0, 0, 40, wxDefaultPosition, wxSize(-1,80), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_VERTICAL);
    m_sliderSQ->SetToolTip(_("Set Squelch level in dB."));

    sbSizer3->Add(m_sliderSQ, 1, wxALIGN_CENTER_HORIZONTAL, 0);

    //------------------------------
    // Squelch Level static text box
    //------------------------------

    m_textSQ = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);

    sbSizer3->Add(m_textSQ, 0, wxALIGN_CENTER_HORIZONTAL, 0);

    //------------------------------
    // Squelch Toggle Checkbox
    //------------------------------

    m_ckboxSQ = new wxCheckBox(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    m_ckboxSQ->SetToolTip(_("Activate/Deactivate Squelch"));

    sbSizer3->Add(m_ckboxSQ, 0, wxALIGN_CENTER_HORIZONTAL, 0);


    rightSizer->Add(sbSizer3, 2, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 0);

    //------------------------------
    // Synch Indicator box
    //------------------------------
    wxStaticBoxSizer* sbSizer3_33;
    sbSizer3_33 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Sync")), wxVERTICAL);

    m_rbSync = new wxRadioButton( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbSync->SetForegroundColour( wxColour( 255, 0, 0 ) );
    sbSizer3_33->Add(m_rbSync, 0, wxALIGN_CENTER|wxALL, 1);

    rightSizer->Add(sbSizer3_33,0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 3);

    //=====================================================
    // Control Toggles box
    //=====================================================
    wxStaticBoxSizer* sbSizer5;
    sbSizer5 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Control")), wxVERTICAL);
    wxBoxSizer* bSizer1511;
    bSizer1511 = new wxBoxSizer(wxVERTICAL);

    //-------------------------------
    // Stop/Stop signal processing (rx and tx)
    //-------------------------------
    m_togBtnOnOff = new wxToggleButton(this, wxID_ANY, _("Start"), wxDefaultPosition, wxDefaultSize, 0);
    m_togBtnOnOff->SetToolTip(_("Begin/End receiving data."));
    bSizer1511->Add(m_togBtnOnOff, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1);
    sbSizer5->Add(bSizer1511, 0, wxEXPAND, 1);

    //------------------------------
    // Toggle Loopback button for RX
    //------------------------------
    wxBoxSizer* bSizer15113;
    bSizer15113 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* bSizer15111;
    bSizer15111 = new wxBoxSizer(wxVERTICAL);
    wxSize wxSz = wxSize(44, 30);
    m_togBtnLoopRx = new wxToggleButton(this, wxID_ANY, _("Loop\nRX"), wxDefaultPosition, wxSz, 0);
    m_togBtnLoopRx->SetFont(wxFont(6, 70, 90, 90, false, wxEmptyString));
    m_togBtnLoopRx->SetToolTip(_("Loopback Receive audio data."));

    bSizer15111->Add(m_togBtnLoopRx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

    //sbSizer5->Add(bSizer15111, 0, wxEXPAND, 1);
    bSizer15113->Add(bSizer15111, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

    //------------------------------
    // Toggle Loopback button for Tx
    //------------------------------
    wxBoxSizer* bSizer15112;
    bSizer15112 = new wxBoxSizer(wxVERTICAL);
    m_togBtnLoopTx = new wxToggleButton(this, wxID_ANY, _("Loop\nTX"), wxDefaultPosition, wxSz, 0);
    m_togBtnLoopTx->SetFont(wxFont(6, 70, 90, 90, false, wxEmptyString));
    m_togBtnLoopTx->SetToolTip(_("Loopback Transmit audio data."));

    bSizer15112->Add(m_togBtnLoopTx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);
    bSizer15113->Add(bSizer15112, 0,  wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

    sbSizer5->Add(bSizer15113, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1);

    //------------------------------
    // Split Frequency Mode Toggle
    //------------------------------
    wxBoxSizer* bSizer151;
    bSizer151 = new wxBoxSizer(wxVERTICAL);

    m_togBtnSplit = new wxToggleButton(this, wxID_ANY, _("Split"), wxDefaultPosition, wxDefaultSize, 0);
    m_togBtnSplit->SetToolTip(_("Toggle split frequency mode."));

    bSizer151->Add(m_togBtnSplit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1);
    sbSizer5->Add(bSizer151, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 1);
    wxBoxSizer* bSizer13;
    bSizer13 = new wxBoxSizer(wxVERTICAL);

    //------------------------------
    // Analog Passthrough Toggle
    //------------------------------
    m_togBtnAnalog = new wxToggleButton(this, wxID_ANY, _("Analog"), wxDefaultPosition, wxDefaultSize, 0);
    m_togBtnAnalog->SetToolTip(_("Toggle analog/digital operation."));
    bSizer13->Add(m_togBtnAnalog, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1);
    sbSizer5->Add(bSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1);

    // not implemented on fdmdv2
#ifdef ALC
    //------------------------------
    // Toggle for ALC
    //------------------------------
    wxBoxSizer* bSizer14;
    bSizer14 = new wxBoxSizer(wxVERTICAL);
    m_togBtnALC = new wxToggleButton(this, wxID_ANY, _("ALC"), wxDefaultPosition, wxDefaultSize, 0);
    m_togBtnALC->SetToolTip(_("Toggle automatic level control mode."));

    bSizer14->Add(m_togBtnALC, 0, wxALL, 1);
    sbSizer5->Add(bSizer14, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
#endif

    //------------------------------
    // Toggle Transmit/Receive relays
    //------------------------------
    wxBoxSizer* bSizer11;
    bSizer11 = new wxBoxSizer(wxVERTICAL);
    m_btnTogTX = new wxToggleButton(this, wxID_ANY, _("TX"), wxDefaultPosition, wxDefaultSize, 0);
    m_btnTogTX->SetToolTip(_("Switch between Receive and Transmit"));
    bSizer11->Add(m_btnTogTX, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1);
    sbSizer5->Add(bSizer11, 2, wxEXPAND, 1);
    rightSizer->Add(sbSizer5, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 3);
    bSizer1->Add(rightSizer, 0, wxALL|wxEXPAND, 3);
    this->SetSizer(bSizer1);
    this->Layout();
    m_statusBar1 = this->CreateStatusBar(3, wxST_SIZEGRIP, wxID_ANY);

    //=====================================================
    // End of layout
    //=====================================================

    //-------------------
    // Connect Events
    //-------------------
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(TopFrame::topFrame_OnClose));
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(TopFrame::topFrame_OnPaint));
    this->Connect(wxEVT_SIZE, wxSizeEventHandler(TopFrame::topFrame_OnSize));
    this->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::topFrame_OnUpdateUI));
    this->Connect(m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnOpen));
    this->Connect(m_menuItemOpen->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnOpenUpdateUI));
    this->Connect(m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnSave));
    this->Connect(m_menuItemSave->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnSaveUpdateUI));
    this->Connect(m_menuItemClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnClose));
    this->Connect(m_menuItemClose->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnCloseUpdateUI));
    this->Connect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnExit));
    this->Connect(m_menuItemCopy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCopy));
    this->Connect(m_menuItemCopy->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnCopyUpdateUI));
    this->Connect(m_menuItemCut->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCut));
    this->Connect(m_menuItemCut->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnCutUpdateUI));
    this->Connect(m_menuItemPaste->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnPaste));
    this->Connect(m_menuItemPaste->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnPasteUpdateUI));
    this->Connect(m_menuItemAudio->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnToolsAudio));
    this->Connect(m_menuItemAudio->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnToolsAudioUI));
    this->Connect(m_menuItemRigCtrlCfg->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnToolsComCfg));
    this->Connect(m_menuItemRigCtrlCfg->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnToolsComCfgUI));
    this->Connect(m_menuItemOptions->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnToolsOptions));
    this->Connect(m_menuItemOptions->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnToolsOptionsUI));
//    this->Connect(m_menuItemCaptRxStream->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCaptureRxStream));
//    this->Connect(m_menuItemCaptTxStream->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCaptureTxStream));
    this->Connect(m_menuItemPlayAudioFile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnPlayAudioFile));
    this->Connect(m_menuItemHelpUpdates->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnHelpCheckUpdates));
    this->Connect(m_menuItemHelpUpdates->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnHelpCheckUpdatesUI));
    this->Connect(m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnHelpAbout));
    m_togRxID->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnRxID), NULL, this);
    m_togTxID->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnTxID), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(TopFrame::OnSliderScrollBottom), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(TopFrame::OnCmdSliderScrollChanged), NULL, this);
    m_sliderSQ->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(TopFrame::OnSliderScrollTop), NULL, this);
    m_ckboxSQ->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(TopFrame::OnCheckSQClick), NULL, this);

    m_togBtnOnOff->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnOnOff), NULL, this);
    m_togBtnLoopRx->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnLoopRx), NULL, this);
    m_togBtnLoopTx->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnLoopTx), NULL, this);
    m_togBtnSplit->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnSplitClick), NULL, this);
    m_togBtnAnalog->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnAnalogClick), NULL, this);
#ifdef ALC
    m_togBtnALC->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnALCClick), NULL, this);
#endif
    m_btnTogTX->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnTXClick), NULL, this);
}

TopFrame::~TopFrame()
{
    //-------------------
    // Disconnect Events
    //-------------------
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(TopFrame::topFrame_OnClose));
    this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(TopFrame::topFrame_OnPaint));
    this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(TopFrame::topFrame_OnSize));
    this->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::topFrame_OnUpdateUI));
    this->Disconnect(ID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnOpen));
    this->Disconnect(ID_OPEN, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnOpenUpdateUI));
    this->Disconnect(ID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnSave));
    this->Disconnect(ID_SAVE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnSaveUpdateUI));
    this->Disconnect(ID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnClose));
    this->Disconnect(ID_CLOSE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnCloseUpdateUI));
    this->Disconnect(ID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnExit));
    this->Disconnect(ID_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCopy));
    this->Disconnect(ID_COPY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnCopyUpdateUI));
    this->Disconnect(ID_CUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCut));
    this->Disconnect(ID_CUT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnCutUpdateUI));
    this->Disconnect(ID_PASTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnPaste));
    this->Disconnect(ID_PASTE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnPasteUpdateUI));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnToolsAudio));
    this->Disconnect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnToolsAudioUI));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnToolsComCfg));
    this->Disconnect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnToolsComCfgUI));
    this->Disconnect(ID_OPTIONS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnToolsOptions));
    this->Disconnect(ID_OPTIONS, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnToolsOptionsUI));
//    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCaptureRxStream));
//    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnCaptureTxStream));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnPlayAudioFile));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnHelpCheckUpdates));
    this->Disconnect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnHelpCheckUpdatesUI));
    this->Disconnect(ID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopFrame::OnHelpAbout));
    m_togRxID->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnRxID), NULL, this);
    m_togTxID->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnTxID), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(TopFrame::OnCmdSliderScroll), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(TopFrame::OnSliderScrollBottom), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(TopFrame::OnCmdSliderScrollChanged), NULL, this);
    m_sliderSQ->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(TopFrame::OnSliderScrollTop), NULL, this);
    m_ckboxSQ->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(TopFrame::OnCheckSQClick), NULL, this);

    m_togBtnOnOff->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnOnOff), NULL, this);
    m_togBtnLoopRx->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnLoopRx), NULL, this);
    m_togBtnLoopTx->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnLoopTx), NULL, this);
    m_togBtnSplit->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnSplitClick), NULL, this);
    m_togBtnAnalog->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnAnalogClick), NULL, this);
#ifdef ALC
    m_togBtnALC->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnALCClick), NULL, this);
#endif
    m_btnTogTX->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(TopFrame::OnTogBtnTXClick), NULL, this);

}

DlgAbout::DlgAbout(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer20;
    bSizer20 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer21;
    bSizer21 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer23;
    bSizer23 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer25;
    bSizer25 = new wxBoxSizer(wxVERTICAL);

    m_bitmapAbout = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    bSizer25->Add(m_bitmapAbout, 1, wxALL|wxEXPAND, 5);


    bSizer23->Add(bSizer25, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer26;
    bSizer26 = new wxBoxSizer(wxVERTICAL);

    m_staticText1 = new wxStaticText(this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText1->Wrap(-1);
    bSizer26->Add(m_staticText1, 1, wxALL, 5);


    bSizer23->Add(bSizer26, 0, 0, 5);


    bSizer21->Add(bSizer23, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer24;
    bSizer24 = new wxBoxSizer(wxVERTICAL);

    m_textCtrl2 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizer24->Add(m_textCtrl2, 1, wxALL|wxEXPAND, 5);


    bSizer21->Add(bSizer24, 1, wxEXPAND, 5);


    bSizer20->Add(bSizer21, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer22;
    bSizer22 = new wxBoxSizer(wxVERTICAL);

    m_button9 = new wxButton(this, wxID_ANY, _("MyButton"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer22->Add(m_button9, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxALL, 5);


    bSizer20->Add(bSizer22, 0, wxALIGN_RIGHT, 5);


    this->SetSizer(bSizer20);
    this->Layout();

    this->Centre(wxBOTH);
    this->Centre(wxBOTH);

    // Connect Events
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgAbout::OnClose));
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgAbout::OnInitDialog));
}

DlgAbout::~DlgAbout()
{
    // Disconnect Events
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgAbout::OnClose));
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgAbout::OnInitDialog));

}

DlgOptions::DlgOptions(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer30;
    bSizer30 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* sbSizer5;
    sbSizer5 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Config Options")), wxHORIZONTAL);

    wxGridSizer* gSizer2;
    gSizer2 = new wxGridSizer(6, 2, 0, 0);

    m_staticText2 = new wxStaticText(this, wxID_ANY, _("Option #1:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    gSizer2->Add(m_staticText2, 1, wxALIGN_RIGHT|wxALL, 5);

    m_textCtrl3 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer2->Add(m_textCtrl3, 1, wxALL, 5);

    m_staticText3 = new wxStaticText(this, wxID_ANY, _("Option #2:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText3->Wrap(-1);
    gSizer2->Add(m_staticText3, 1, wxALIGN_RIGHT|wxALL, 5);

    m_textCtrl4 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer2->Add(m_textCtrl4, 1, wxALL, 5);

    m_staticText4 = new wxStaticText(this, wxID_ANY, _("Option #3:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText4->Wrap(-1);
    gSizer2->Add(m_staticText4, 1, wxALIGN_RIGHT|wxALL, 5);

    m_textCtrl5 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer2->Add(m_textCtrl5, 1, wxALL, 5);

    m_staticText5 = new wxStaticText(this, wxID_ANY, _("Option #4:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText5->Wrap(-1);
    gSizer2->Add(m_staticText5, 1, wxALIGN_RIGHT|wxALL, 5);

    m_textCtrl6 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer2->Add(m_textCtrl6, 1, wxALL, 5);

    m_staticText6 = new wxStaticText(this, wxID_ANY, _("Option #5:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText6->Wrap(-1);
    gSizer2->Add(m_staticText6, 1, wxALIGN_RIGHT|wxALL, 5);

    m_textCtrl7 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer2->Add(m_textCtrl7, 1, wxALL, 5);

    m_staticText7 = new wxStaticText(this, wxID_ANY, _("Option #6:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText7->Wrap(-1);
    gSizer2->Add(m_staticText7, 1, wxALIGN_RIGHT|wxALL, 5);

    m_textCtrl8 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    gSizer2->Add(m_textCtrl8, 1, wxALL, 5);


    sbSizer5->Add(gSizer2, 1, wxEXPAND, 5);


    bSizer30->Add(sbSizer5, 1, wxEXPAND, 5);

    m_sdbSizer4 = new wxStdDialogButtonSizer();
    m_sdbSizer4OK = new wxButton(this, wxID_OK);
    m_sdbSizer4->AddButton(m_sdbSizer4OK);
    m_sdbSizer4Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer4->AddButton(m_sdbSizer4Cancel);
    m_sdbSizer4->Realize();

    bSizer30->Add(m_sdbSizer4, 0, wxALIGN_RIGHT, 5);


    this->SetSizer(bSizer30);
    this->Layout();

    this->Centre(wxBOTH);
    this->Centre(wxBOTH);

    // Connect Events
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgOptions::OnClose));
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgOptions::OnInitDialog));
}

DlgOptions::~DlgOptions()
{
    // Disconnect Events
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgOptions::OnClose));
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgOptions::OnInitDialog));

}

