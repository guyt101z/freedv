///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 17 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "topFrame.h"

///////////////////////////////////////////////////////////////////////////

TopFrame::TopFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
	
	m_menubarMain = new wxMenuBar( wxMB_DOCKABLE );
	file = new wxMenu();
	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( file, ID_OPEN, wxString( _("&Open") ) , _("Open File"), wxITEM_NORMAL );
	file->Append( m_menuItemOpen );
	
	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( file, ID_SAVE, wxString( _("&Save") ) , _("Save current file"), wxITEM_NORMAL );
	file->Append( m_menuItemSave );
	
	wxMenuItem* m_menuItemClose;
	m_menuItemClose = new wxMenuItem( file, ID_CLOSE, wxString( _("&Close") ) , _("Close current file"), wxITEM_NORMAL );
	file->Append( m_menuItemClose );
	
	file->AppendSeparator();
	
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( file, ID_EXIT, wxString( _("E&xit") ) , _("Exit Program"), wxITEM_NORMAL );
	file->Append( m_menuItemExit );
	
	m_menubarMain->Append( file, _("&File") ); 
	
	edit = new wxMenu();
	wxMenuItem* m_menuItemCopy;
	m_menuItemCopy = new wxMenuItem( edit, ID_COPY, wxString( _("&Copy") ) , _("Copy selection"), wxITEM_NORMAL );
	edit->Append( m_menuItemCopy );
	m_menuItemCopy->Enable( false );
	
	wxMenuItem* m_menuItemCut;
	m_menuItemCut = new wxMenuItem( edit, ID_CUT, wxString( _("Cut") ) , _("Cut Selection"), wxITEM_NORMAL );
	edit->Append( m_menuItemCut );
	m_menuItemCut->Enable( false );
	
	wxMenuItem* m_menuItemPaste;
	m_menuItemPaste = new wxMenuItem( edit, ID_PASTE, wxString( _("&Paste") ) , _("Paste selection"), wxITEM_NORMAL );
	edit->Append( m_menuItemPaste );
	m_menuItemPaste->Enable( false );
	
	m_menubarMain->Append( edit, _("&Edit") ); 
	
	tools = new wxMenu();
	wxMenuItem* m_menuItemAudio;
	m_menuItemAudio = new wxMenuItem( tools, wxID_ANY, wxString( _("&Audio") ) , wxEmptyString, wxITEM_NORMAL );
	tools->Append( m_menuItemAudio );
	
	wxMenuItem* m_menuItemComCfg;
	m_menuItemComCfg = new wxMenuItem( tools, wxID_ANY, wxString( _("Com Port Config") ) , wxEmptyString, wxITEM_NORMAL );
	tools->Append( m_menuItemComCfg );
	
	wxMenuItem* m_menuItemOptions;
	m_menuItemOptions = new wxMenuItem( tools, ID_OPTIONS, wxString( _("&Options") ) , _("Set preferences"), wxITEM_NORMAL );
	tools->Append( m_menuItemOptions );
	
	m_menubarMain->Append( tools, _("&Tools") ); 
	
	help = new wxMenu();
	wxMenuItem* m_menuItemHelpUpdates;
	m_menuItemHelpUpdates = new wxMenuItem( help, wxID_ANY, wxString( _("Check for Updates") ) , wxEmptyString, wxITEM_NORMAL );
	help->Append( m_menuItemHelpUpdates );
	m_menuItemHelpUpdates->Enable( false );
	
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( help, ID_ABOUT, wxString( _("&About") ) , _("About this program"), wxITEM_NORMAL );
	help->Append( m_menuItemAbout );
	
	m_menubarMain->Append( help, _("&Help") ); 
	
	this->SetMenuBar( m_menubarMain );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* leftSizer;
	leftSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* snrSizer;
	snrSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("SNR") ), wxVERTICAL );
	
	m_gaugeSNR = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( 15,135 ), wxGA_SMOOTH|wxGA_VERTICAL );
	m_gaugeSNR->SetToolTip( _("Display signal to noise ratio.") );
	
	snrSizer->Add( m_gaugeSNR, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	m_textSNR = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxSize( 35,25 ), wxTE_READONLY );
	m_textSNR->SetToolTip( _("Show S/N ratio numerically.") );
	m_textSNR->SetMinSize( wxSize( 35,25 ) );
	
	bSizer29->Add( m_textSNR, 0, wxALIGN_CENTER|wxALL, 1 );
	
	
	snrSizer->Add( bSizer29, 0, wxEXPAND, 1 );
	
	
	leftSizer->Add( snrSizer, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 1 );
	
	wxStaticBoxSizer* levelSizer;
	levelSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Level") ), wxVERTICAL );
	
	m_gaugeLevel = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( 15,135 ), wxGA_SMOOTH|wxGA_VERTICAL );
	m_gaugeLevel->SetToolTip( _("Display signal level.") );
	
	levelSizer->Add( m_gaugeLevel, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
	
	
	leftSizer->Add( levelSizer, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 1 );
	
	
	bSizer1->Add( leftSizer, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* centerSizer;
	centerSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* upperSizer;
	upperSizer = new wxBoxSizer( wxVERTICAL );
	
	m_auiNbookCtrl = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_BOTTOM|wxAUI_NB_DEFAULT_STYLE );
	m_auiNbookCtrl->SetFont( wxFont( 6, 70, 90, 90, false, wxEmptyString ) );
	
	
	upperSizer->Add( m_auiNbookCtrl, 1, wxALIGN_TOP|wxEXPAND, 1 );
	
	
	centerSizer->Add( upperSizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_TOP|wxEXPAND, 0 );
	
	wxBoxSizer* lowerSizer;
	lowerSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_txtCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_txtCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	
	lowerSizer->Add( bSizer15, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxHORIZONTAL );
	
	m_togRxID = new wxToggleButton( this, wxID_ANY, _("RxID"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer141->Add( m_togRxID, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL|wxFIXED_MINSIZE, 5 );
	
	m_togTxID = new wxToggleButton( this, wxID_ANY, _("TxID"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer141->Add( m_togTxID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	lowerSizer->Add( bSizer141, 0, wxALIGN_RIGHT, 5 );
	
	
	centerSizer->Add( lowerSizer, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 2 );
	
	
	bSizer1->Add( centerSizer, 4, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 1 );
	
	wxBoxSizer* rightSizer;
	rightSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("SQ") ), wxVERTICAL );
	
	m_sliderSQ = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( -1,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_VERTICAL );
	m_sliderSQ->SetToolTip( _("Set Squelch level.") );
	
	sbSizer3->Add( m_sliderSQ, 2, wxALIGN_CENTER|wxALL, 1 );
	
	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxVERTICAL );
	
	m_ckboxSQ = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	m_ckboxSQ->SetToolTip( _("Activate/Deactivate Squelch") );
	
	bSizer131->Add( m_ckboxSQ, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	sbSizer3->Add( bSizer131, 0, wxALIGN_CENTER, 5 );
	
	
	rightSizer->Add( sbSizer3, 2, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 3 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Control") ), wxVERTICAL );
	
	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxVERTICAL );
	
	m_togBtnOnOff = new wxToggleButton( this, wxID_ANY, _("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_togBtnOnOff->SetToolTip( _("Begin receiving data.") );
	
	bSizer1511->Add( m_togBtnOnOff, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	
	sbSizer5->Add( bSizer1511, 0, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxVERTICAL );
	
	m_togBtnSplit = new wxToggleButton( this, wxID_ANY, _("Split"), wxDefaultPosition, wxDefaultSize, 0 );
	m_togBtnSplit->SetToolTip( _("Toggle split frequency mode.") );
	
	bSizer151->Add( m_togBtnSplit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	
	sbSizer5->Add( bSizer151, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 1 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	m_togBtnAnalog = new wxToggleButton( this, wxID_ANY, _("Analog"), wxDefaultPosition, wxDefaultSize, 0 );
	m_togBtnAnalog->SetToolTip( _("Toggle analog/digital operation.") );
	
	bSizer13->Add( m_togBtnAnalog, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	
	sbSizer5->Add( bSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_togBtnALC = new wxToggleButton( this, wxID_ANY, _("ALC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_togBtnALC->SetToolTip( _("Toggle automatic level control mode.") );
	
	bSizer14->Add( m_togBtnALC, 0, wxALL, 1 );
	
	
	sbSizer5->Add( bSizer14, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALL, 1 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_btnTogTX = new wxToggleButton( this, wxID_ANY, _("TX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnTogTX->SetToolTip( _("Switch between Receive and Transmit") );
	
	bSizer11->Add( m_btnTogTX, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	
	sbSizer5->Add( bSizer11, 2, wxEXPAND, 1 );
	
	
	rightSizer->Add( sbSizer5, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 3 );
	
	
	bSizer1->Add( rightSizer, 0, wxALL|wxEXPAND, 3 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 3, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TopFrame::topFrame_OnClose ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( TopFrame::topFrame_OnPaint ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( TopFrame::topFrame_OnSize ) );
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::topFrame_OnUpdateUI ) );
	this->Connect( m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnOpen ) );
	this->Connect( m_menuItemOpen->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnOpenUpdateUI ) );
	this->Connect( m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnSave ) );
	this->Connect( m_menuItemSave->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnSaveUpdateUI ) );
	this->Connect( m_menuItemClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnClose ) );
	this->Connect( m_menuItemClose->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnCloseUpdateUI ) );
	this->Connect( m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnExit ) );
	this->Connect( m_menuItemCopy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnCopy ) );
	this->Connect( m_menuItemCopy->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnCopyUpdateUI ) );
	this->Connect( m_menuItemCut->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnCut ) );
	this->Connect( m_menuItemCut->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnCutUpdateUI ) );
	this->Connect( m_menuItemPaste->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnPaste ) );
	this->Connect( m_menuItemPaste->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnPasteUpdateUI ) );
	this->Connect( m_menuItemAudio->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnToolsAudio ) );
	this->Connect( m_menuItemAudio->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnToolsAudioUI ) );
	this->Connect( m_menuItemComCfg->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnToolsComCfg ) );
	this->Connect( m_menuItemComCfg->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnToolsComCfgUI ) );
	this->Connect( m_menuItemOptions->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnToolsOptions ) );
	this->Connect( m_menuItemOptions->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnToolsOptionsUI ) );
	this->Connect( m_menuItemHelpUpdates->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnHelpCheckUpdates ) );
	this->Connect( m_menuItemHelpUpdates->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnHelpCheckUpdatesUI ) );
	this->Connect( m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnHelpAbout ) );
	m_togRxID->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnRxID ), NULL, this );
	m_togTxID->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnTxID ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TopFrame::OnSliderScrollBottom ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TopFrame::OnCmdSliderScrollChanged ), NULL, this );
	m_sliderSQ->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TopFrame::OnSliderScrollTop ), NULL, this );
	m_ckboxSQ->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TopFrame::OnCheckSQClick ), NULL, this );
	m_togBtnOnOff->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnOnOff ), NULL, this );
	m_togBtnSplit->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnSplitClick ), NULL, this );
	m_togBtnAnalog->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnAnalogClick ), NULL, this );
	m_togBtnALC->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnALCClick ), NULL, this );
	m_btnTogTX->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnTXClick ), NULL, this );
}

TopFrame::~TopFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TopFrame::topFrame_OnClose ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( TopFrame::topFrame_OnPaint ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( TopFrame::topFrame_OnSize ) );
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::topFrame_OnUpdateUI ) );
	this->Disconnect( ID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnOpen ) );
	this->Disconnect( ID_OPEN, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnOpenUpdateUI ) );
	this->Disconnect( ID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnSave ) );
	this->Disconnect( ID_SAVE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnSaveUpdateUI ) );
	this->Disconnect( ID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnClose ) );
	this->Disconnect( ID_CLOSE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnCloseUpdateUI ) );
	this->Disconnect( ID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnExit ) );
	this->Disconnect( ID_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnCopy ) );
	this->Disconnect( ID_COPY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnCopyUpdateUI ) );
	this->Disconnect( ID_CUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnCut ) );
	this->Disconnect( ID_CUT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnCutUpdateUI ) );
	this->Disconnect( ID_PASTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnPaste ) );
	this->Disconnect( ID_PASTE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnPasteUpdateUI ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnToolsAudio ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnToolsAudioUI ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnToolsComCfg ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnToolsComCfgUI ) );
	this->Disconnect( ID_OPTIONS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnToolsOptions ) );
	this->Disconnect( ID_OPTIONS, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnToolsOptionsUI ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnHelpCheckUpdates ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( TopFrame::OnHelpCheckUpdatesUI ) );
	this->Disconnect( ID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TopFrame::OnHelpAbout ) );
	m_togRxID->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnRxID ), NULL, this );
	m_togTxID->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnTxID ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TopFrame::OnCmdSliderScroll ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TopFrame::OnSliderScrollBottom ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TopFrame::OnCmdSliderScrollChanged ), NULL, this );
	m_sliderSQ->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TopFrame::OnSliderScrollTop ), NULL, this );
	m_ckboxSQ->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TopFrame::OnCheckSQClick ), NULL, this );
	m_togBtnOnOff->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnOnOff ), NULL, this );
	m_togBtnSplit->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnSplitClick ), NULL, this );
	m_togBtnAnalog->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnAnalogClick ), NULL, this );
	m_togBtnALC->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnALCClick ), NULL, this );
	m_btnTogTX->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( TopFrame::OnTogBtnTXClick ), NULL, this );
	
}

DlgAbout::DlgAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmapAbout = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_bitmapAbout, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer23->Add( bSizer25, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer26->Add( m_staticText1, 1, wxALL, 5 );
	
	
	bSizer23->Add( bSizer26, 0, 0, 5 );
	
	
	bSizer21->Add( bSizer23, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( m_textCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer21->Add( bSizer24, 1, wxEXPAND, 5 );
	
	
	bSizer20->Add( bSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_button9 = new wxButton( this, wxID_ANY, _("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( m_button9, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizer20->Add( bSizer22, 0, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( bSizer20 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgAbout::OnClose ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgAbout::OnInitDialog ) );
}

DlgAbout::~DlgAbout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgAbout::OnClose ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgAbout::OnInitDialog ) );
	
}

DlgAudio::DlgAudio( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );
	
	m_nbAudioOptions = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelDevices = new wxPanel( m_nbAudioOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panelDevices, wxID_ANY, _("PortAudio") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 3, 1, 0, 0 );
	
	gSizer6->SetMinSize( wxSize( 115,-1 ) ); 
	m_radioBtnPortAudio = new wxRadioButton( m_panelDevices, wxID_ANY, _("PortAudio"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_radioBtnPortAudio->SetValue( true ); 
	gSizer6->Add( m_radioBtnPortAudio, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxGridSizer* gSizer51;
	gSizer51 = new wxGridSizer( 2, 2, 0, 0 );
	
	gSizer51->SetMinSize( wxSize( 110,-1 ) ); 
	m_staticText151 = new wxStaticText( m_panelDevices, wxID_ANY, _("# Ch Out:"), wxDefaultPosition, wxSize( 80,-1 ), wxALIGN_RIGHT );
	m_staticText151->Wrap( -1 );
	m_staticText151->SetFont( wxFont( 7, 70, 90, 90, false, wxEmptyString ) );
	m_staticText151->SetMinSize( wxSize( 80,-1 ) );
	m_staticText151->SetMaxSize( wxSize( 80,-1 ) );
	
	gSizer51->Add( m_staticText151, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_textNumChOut = new wxTextCtrl( m_panelDevices, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), wxTE_READONLY );
	m_textNumChOut->SetFont( wxFont( 7, 70, 90, 90, false, wxEmptyString ) );
	m_textNumChOut->SetMinSize( wxSize( 30,-1 ) );
	m_textNumChOut->SetMaxSize( wxSize( 30,-1 ) );
	
	gSizer51->Add( m_textNumChOut, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	m_staticText141 = new wxStaticText( m_panelDevices, wxID_ANY, _("# Ch In:"), wxDefaultPosition, wxSize( 80,-1 ), wxALIGN_RIGHT );
	m_staticText141->Wrap( -1 );
	m_staticText141->SetFont( wxFont( 7, 70, 90, 90, false, wxEmptyString ) );
	m_staticText141->SetMinSize( wxSize( 80,-1 ) );
	m_staticText141->SetMaxSize( wxSize( 90,-1 ) );
	
	gSizer51->Add( m_staticText141, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_textNumChIn = new wxTextCtrl( m_panelDevices, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), wxTE_READONLY );
	m_textNumChIn->SetFont( wxFont( 7, 70, 90, 90, false, wxEmptyString ) );
	m_textNumChIn->SetMinSize( wxSize( 30,-1 ) );
	m_textNumChIn->SetMaxSize( wxSize( 30,-1 ) );
	
	gSizer51->Add( m_textNumChIn, 0, wxALL, 4 );
	
	
	gSizer6->Add( gSizer51, 0, wxEXPAND, 5 );
	
	
	bSizer41->Add( gSizer6, 0, wxALL, 5 );
	
	wxGridSizer* gSizer7;
	gSizer7 = new wxGridSizer( 4, 2, 0, 0 );
	
	m_staticText14 = new wxStaticText( m_panelDevices, wxID_ANY, _("Audio in (mic) -> codec:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText14->Wrap( -1 );
	gSizer7->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_comboAudioCodec = new wxComboBox( m_panelDevices, wxID_ANY, _("<default>"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	gSizer7->Add( m_comboAudioCodec, 1, wxALL, 5 );
	
	m_staticText15 = new wxStaticText( m_panelDevices, wxID_ANY, _("codec -> Radio Tx:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText15->Wrap( -1 );
	gSizer7->Add( m_staticText15, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_comboCodecTx = new wxComboBox( m_panelDevices, wxID_ANY, _("<default>"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	gSizer7->Add( m_comboCodecTx, 1, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( m_panelDevices, wxID_ANY, _("Radio Rx -> codec:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText16->Wrap( -1 );
	gSizer7->Add( m_staticText16, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_comboRadioRx = new wxComboBox( m_panelDevices, wxID_ANY, _("<default>"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	gSizer7->Add( m_comboRadioRx, 1, wxALL, 5 );
	
	m_staticText17 = new wxStaticText( m_panelDevices, wxID_ANY, _("codec -> Audio out (spkr):"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText17->Wrap( -1 );
	gSizer7->Add( m_staticText17, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_comboCodecSpkr = new wxComboBox( m_panelDevices, wxID_ANY, _("<default>"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	gSizer7->Add( m_comboCodecSpkr, 0, wxALL, 5 );
	
	
	bSizer41->Add( gSizer7, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer10->Add( bSizer41, 1, wxEXPAND, 5 );
	
	
	bSizer34->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( m_panelDevices, wxID_ANY, _("File I/O") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );
	
	m_radioBtnFileOnly = new wxRadioButton( m_panelDevices, wxID_ANY, _("File Only"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( m_radioBtnFileOnly, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer11->Add( bSizer38, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxVERTICAL );
	
	
	sbSizer11->Add( bSizer45, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer( wxVERTICAL );
	
	
	sbSizer11->Add( bSizer46, 1, wxEXPAND, 5 );
	
	
	bSizer34->Add( sbSizer11, 1, wxEXPAND, 5 );
	
	
	m_panelDevices->SetSizer( bSizer34 );
	m_panelDevices->Layout();
	bSizer34->Fit( m_panelDevices );
	m_nbAudioOptions->AddPage( m_panelDevices, _("Devices"), true );
	m_panelSettings = new wxPanel( m_nbAudioOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer271;
	bSizer271 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panelSettings, wxID_ANY, _("Rx Input") ), wxVERTICAL );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_lbRxInput = new wxListBox( m_panelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_SINGLE ); 
	bSizer31->Add( m_lbRxInput, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 2 );
	
	m_textRxInput = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_textRxInput, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 2 );
	
	
	sbSizer6->Add( bSizer31, 1, wxEXPAND, 6 );
	
	
	bSizer271->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panelSettings, wxID_ANY, _("Tx Output") ), wxVERTICAL );
	
	wxBoxSizer* bSizer281;
	bSizer281 = new wxBoxSizer( wxVERTICAL );
	
	m_lbTxOutput = new wxListBox( m_panelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_SINGLE ); 
	bSizer281->Add( m_lbTxOutput, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 2 );
	
	m_textCtrl15 = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer281->Add( m_textCtrl15, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 2 );
	
	
	sbSizer7->Add( bSizer281, 1, wxEXPAND, 5 );
	
	
	bSizer271->Add( sbSizer7, 1, wxEXPAND, 6 );
	
	
	bSizer27->Add( bSizer271, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panelSettings, wxID_ANY, _("Voice Input") ), wxVERTICAL );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	m_lbVoiceInput = new wxListBox( m_panelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_SINGLE ); 
	bSizer29->Add( m_lbVoiceInput, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 2 );
	
	m_textVoiceInput = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_textVoiceInput, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 2 );
	
	
	sbSizer8->Add( bSizer29, 1, wxEXPAND, 5 );
	
	
	bSizer26->Add( sbSizer8, 1, wxEXPAND, 6 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panelSettings, wxID_ANY, _("Voice Output") ), wxVERTICAL );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	m_lbVoiceOutput = new wxListBox( m_panelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_SINGLE ); 
	bSizer30->Add( m_lbVoiceOutput, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 2 );
	
	m_textVoiceOutput = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( m_textVoiceOutput, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 2 );
	
	
	sbSizer9->Add( bSizer30, 1, wxEXPAND, 1 );
	
	
	bSizer26->Add( sbSizer9, 1, wxEXPAND, 6 );
	
	
	bSizer27->Add( bSizer26, 1, wxEXPAND, 1 );
	
	
	m_panelSettings->SetSizer( bSizer27 );
	m_panelSettings->Layout();
	bSizer27->Fit( m_panelSettings );
	m_nbAudioOptions->AddPage( m_panelSettings, _("Settings"), false );
	m_panelRightChannel = new wxPanel( m_nbAudioOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );
	
	
	m_panelRightChannel->SetSizer( bSizer36 );
	m_panelRightChannel->Layout();
	bSizer36->Fit( m_panelRightChannel );
	m_nbAudioOptions->AddPage( m_panelRightChannel, _("Right Channel"), false );
	
	bSizer32->Add( m_nbAudioOptions, 1, wxEXPAND | wxALL, 1 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	m_sdbSizer3 = new wxStdDialogButtonSizer();
	m_sdbSizer3OK = new wxButton( this, wxID_OK );
	m_sdbSizer3->AddButton( m_sdbSizer3OK );
	m_sdbSizer3Apply = new wxButton( this, wxID_APPLY );
	m_sdbSizer3->AddButton( m_sdbSizer3Apply );
	m_sdbSizer3Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer3->AddButton( m_sdbSizer3Cancel );
	m_sdbSizer3->Realize();
	
	bSizer28->Add( m_sdbSizer3, 1, wxEXPAND, 5 );
	
	
	bSizer32->Add( bSizer28, 0, wxEXPAND, 5 );
	
	
	bSizer32->Add( 0, 15, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer32 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgAudio::OnClose ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgAudio::OnInitDialog ) );
	m_lbRxInput->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnRxInputSelect ), NULL, this );
	m_lbTxOutput->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnTxOutputSelect ), NULL, this );
	m_lbVoiceInput->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnVoiceInputSelect ), NULL, this );
	m_lbVoiceOutput->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnVoiceOutputSelect ), NULL, this );
	m_sdbSizer3Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgAudio::OnApply ), NULL, this );
	m_sdbSizer3Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgAudio::OnCancel ), NULL, this );
	m_sdbSizer3OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgAudio::OnOK ), NULL, this );
}

DlgAudio::~DlgAudio()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgAudio::OnClose ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgAudio::OnInitDialog ) );
	m_lbRxInput->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnRxInputSelect ), NULL, this );
	m_lbTxOutput->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnTxOutputSelect ), NULL, this );
	m_lbVoiceInput->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnVoiceInputSelect ), NULL, this );
	m_lbVoiceOutput->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgAudio::OnVoiceOutputSelect ), NULL, this );
	m_sdbSizer3Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgAudio::OnApply ), NULL, this );
	m_sdbSizer3Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgAudio::OnCancel ), NULL, this );
	m_sdbSizer3OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgAudio::OnOK ), NULL, this );
	
}

DlgOptions::DlgOptions( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Config Options") ), wxHORIZONTAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 6, 2, 0, 0 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Option #1:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gSizer2->Add( m_staticText2, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textCtrl3, 1, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Option #2:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	gSizer2->Add( m_staticText3, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textCtrl4, 1, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Option #3:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	gSizer2->Add( m_staticText4, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl5 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textCtrl5, 1, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Option #4:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gSizer2->Add( m_staticText5, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl6 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textCtrl6, 1, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Option #5:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	gSizer2->Add( m_staticText6, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl7 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textCtrl7, 1, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Option #6:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer2->Add( m_staticText7, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl8 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textCtrl8, 1, wxALL, 5 );
	
	
	sbSizer5->Add( gSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer30->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4OK = new wxButton( this, wxID_OK );
	m_sdbSizer4->AddButton( m_sdbSizer4OK );
	m_sdbSizer4Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer4->AddButton( m_sdbSizer4Cancel );
	m_sdbSizer4->Realize();
	
	bSizer30->Add( m_sdbSizer4, 0, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( bSizer30 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgOptions::OnClose ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgOptions::OnInitDialog ) );
}

DlgOptions::~DlgOptions()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgOptions::OnClose ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgOptions::OnInitDialog ) );
	
}

DlgComPorts::DlgComPorts( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("Available Ports:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText8->Wrap( -1 );
	gSizer3->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_listCtrlPortSelect = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_SINGLE_SEL );
	gSizer3->Add( m_listCtrlPortSelect, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Use Port:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText9->Wrap( -1 );
	gSizer3->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_textCtrl10 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_textCtrl10, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer30->Add( gSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
	
	m_sdbSizer3 = new wxStdDialogButtonSizer();
	m_sdbSizer3OK = new wxButton( this, wxID_OK );
	m_sdbSizer3->AddButton( m_sdbSizer3OK );
	m_sdbSizer3Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer3->AddButton( m_sdbSizer3Cancel );
	m_sdbSizer3->Realize();
	
	bSizer30->Add( m_sdbSizer3, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer30 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_listCtrlPortSelect->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( DlgComPorts::OnListItemSelected ), NULL, this );
}

DlgComPorts::~DlgComPorts()
{
	// Disconnect Events
	m_listCtrlPortSelect->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( DlgComPorts::OnListItemSelected ), NULL, this );
	
}
