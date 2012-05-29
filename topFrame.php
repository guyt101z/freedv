<?php

/*
* PHP code generated with wxFormBuilder (version Mar 17 2012)
* http://www.wxformbuilder.org/
*
* PLEASE DO "NOT" EDIT THIS FILE!
*/

include_once dlg_audio.h;

const ID_OPEN = 1000;
const ID_SAVE = 1001;
const ID_CLOSE = 1002;
const ID_EXIT = 1003;
const ID_COPY = 1004;
const ID_CUT = 1005;
const ID_PASTE = 1006;
const ID_OPTIONS = 1007;
const ID_ABOUT = 1008;

/*
 * Class TopFrame
 */

class TopFrame extends wxFrame {
	
	function __construct( $parent=null ){
		parent::__construct ( $parent, wxID_ANY, _("FDMDV2"), wxDefaultPosition, new wxSize( 732,507 ), wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		
		$this->SetSizeHints( wxDefaultSize, wxDefaultSize );
		$this->SetForegroundColour( wxSystemSettings->GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
		$this->SetBackgroundColour( wxSystemSettings->GetColour( wxSYS_COLOUR_3DLIGHT ) );
		
		$this->m_menubarMain = new wxMenuBar( wxMB_DOCKABLE );
		$this->file = new wxMenu();
		$this->m_menuItemOpen = new wxMenuItem( $this->file, ID_OPEN, _("&Open"), _("Open File"), wxITEM_NORMAL );
		$this->file->Append( $this->m_menuItemOpen );
		
		$this->m_menuItemSave = new wxMenuItem( $this->file, ID_SAVE, _("&Save"), _("Save current file"), wxITEM_NORMAL );
		$this->file->Append( $this->m_menuItemSave );
		
		$this->m_menuItemClose = new wxMenuItem( $this->file, ID_CLOSE, _("&Close"), _("Close current file"), wxITEM_NORMAL );
		$this->file->Append( $this->m_menuItemClose );
		
		$this->file->AppendSeparator();
		
		$this->m_menuItemExit = new wxMenuItem( $this->file, ID_EXIT, _("E&xit"), _("Exit Program"), wxITEM_NORMAL );
		$this->file->Append( $this->m_menuItemExit );
		
		$this->m_menubarMain->Append( $this->file, _("&File") ); 
		
		$this->edit = new wxMenu();
		$this->m_menuItemCopy = new wxMenuItem( $this->edit, ID_COPY, _("&Copy"), _("Copy selection"), wxITEM_NORMAL );
		$this->edit->Append( $this->m_menuItemCopy );
		$this->m_menuItemCopy->Enable( false );
		
		$this->m_menuItemCut = new wxMenuItem( $this->edit, ID_CUT, _("Cut"), _("Cut Selection"), wxITEM_NORMAL );
		$this->edit->Append( $this->m_menuItemCut );
		$this->m_menuItemCut->Enable( false );
		
		$this->m_menuItemPaste = new wxMenuItem( $this->edit, ID_PASTE, _("&Paste"), _("Paste selection"), wxITEM_NORMAL );
		$this->edit->Append( $this->m_menuItemPaste );
		$this->m_menuItemPaste->Enable( false );
		
		$this->m_menubarMain->Append( $this->edit, _("&Edit") ); 
		
		$this->tools = new wxMenu();
		$this->m_menuItemAudio = new wxMenuItem( $this->tools, wxID_ANY, _("&Audio"), wxEmptyString, wxITEM_NORMAL );
		$this->tools->Append( $this->m_menuItemAudio );
		
		$this->m_menuItemComCfg = new wxMenuItem( $this->tools, wxID_ANY, _("Com Port Config"), wxEmptyString, wxITEM_NORMAL );
		$this->tools->Append( $this->m_menuItemComCfg );
		
		$this->m_menuItemOptions = new wxMenuItem( $this->tools, ID_OPTIONS, _("&Options"), _("Set preferences"), wxITEM_NORMAL );
		$this->tools->Append( $this->m_menuItemOptions );
		
		$this->m_menubarMain->Append( $this->tools, _("&Tools") ); 
		
		$this->help = new wxMenu();
		$this->m_menuItemHelpUpdates = new wxMenuItem( $this->help, wxID_ANY, _("Check for Updates"), wxEmptyString, wxITEM_NORMAL );
		$this->help->Append( $this->m_menuItemHelpUpdates );
		$this->m_menuItemHelpUpdates->Enable( false );
		
		$this->m_menuItemAbout = new wxMenuItem( $this->help, ID_ABOUT, _("&About"), _("About this program"), wxITEM_NORMAL );
		$this->help->Append( $this->m_menuItemAbout );
		
		$this->m_menubarMain->Append( $this->help, _("&Help") ); 
		
		$this->SetMenuBar( $this->m_menubarMain );
		
		$bSizer1 = new wxBoxSizer( wxHORIZONTAL );
		
		$leftSizer = new wxBoxSizer( wxVERTICAL );
		
		$snrSizer = new wxStaticBoxSizer( new wxStaticBox( $this, wxID_ANY, _("SNR") ), wxVERTICAL );
		
		$this->m_gaugeSNR = new wxGauge( $this, wxID_ANY, 100, wxDefaultPosition, new wxSize( 15,135 ), wxGA_SMOOTH|wxGA_VERTICAL );
		$snrSizer->Add( $this->m_gaugeSNR, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
		
		$bSizer29 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_textSNR = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, new wxPoint( -1,-1 ), new wxSize( 35,25 ), wxTE_READONLY );
		$this->m_textSNR->SetMinSize( new wxSize( 35,25 ) );
		
		$bSizer29->Add( $this->m_textSNR, 0, wxALIGN_CENTER|wxALL, 1 );
		
		
		$snrSizer->Add( $bSizer29, 0, wxEXPAND, 1 );
		
		
		$leftSizer->Add( $snrSizer, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 1 );
		
		$levelSizer = new wxStaticBoxSizer( new wxStaticBox( $this, wxID_ANY, _("Level") ), wxVERTICAL );
		
		$this->m_gaugeLevel = new wxGauge( $this, wxID_ANY, 100, wxDefaultPosition, new wxSize( 15,135 ), wxGA_SMOOTH|wxGA_VERTICAL );
		$levelSizer->Add( $this->m_gaugeLevel, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
		
		
		$leftSizer->Add( $levelSizer, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 1 );
		
		
		$bSizer1->Add( $leftSizer, 0, wxALL|wxEXPAND, 5 );
		
		$centerSizer = new wxBoxSizer( wxVERTICAL );
		
		$upperSizer = new wxBoxSizer( wxVERTICAL );
		
		$this->m_auiNbookCtrl = new wxAuiNotebook( $this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_BOTTOM|wxAUI_NB_DEFAULT_STYLE );
		
		$upperSizer->Add( $this->m_auiNbookCtrl, 1, wxALIGN_TOP|wxEXPAND, 1 );
		
		
		$centerSizer->Add( $upperSizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_TOP|wxEXPAND, 0 );
		
		$lowerSizer = new wxBoxSizer( wxHORIZONTAL );
		
		$bSizer15 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_txtCtrl = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer15->Add( $this->m_txtCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
		
		
		$lowerSizer->Add( $bSizer15, 1, wxEXPAND, 5 );
		
		$bSizer141 = new wxBoxSizer( wxHORIZONTAL );
		
		$this->m_togRxID = new wxToggleButton( $this, wxID_ANY, _("RxID"), wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer141->Add( $this->m_togRxID, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL|wxFIXED_MINSIZE, 5 );
		
		$this->m_togTxID = new wxToggleButton( $this, wxID_ANY, _("TxID"), wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer141->Add( $this->m_togTxID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
		
		
		$lowerSizer->Add( $bSizer141, 0, wxALIGN_RIGHT, 5 );
		
		
		$centerSizer->Add( $lowerSizer, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 2 );
		
		
		$bSizer1->Add( $centerSizer, 4, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 1 );
		
		$rightSizer = new wxBoxSizer( wxVERTICAL );
		
		$sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( $this, wxID_ANY, _("SQ") ), wxVERTICAL );
		
		$this->m_sliderSQ = new wxSlider( $this, wxID_ANY, 50, 0, 100, wxDefaultPosition, new wxSize( -1,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_VERTICAL );
		$sbSizer3->Add( $this->m_sliderSQ, 2, wxALIGN_CENTER|wxALL, 1 );
		
		$bSizer131 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_ckboxSQ = new wxCheckBox( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
		$bSizer131->Add( $this->m_ckboxSQ, 0, wxALIGN_CENTER|wxALL, 5 );
		
		
		$sbSizer3->Add( $bSizer131, 0, wxALIGN_CENTER, 5 );
		
		
		$rightSizer->Add( $sbSizer3, 3, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 3 );
		
		$sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( $this, wxID_ANY, _("Control") ), wxVERTICAL );
		
		$bSizer151 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_togBtnSplit = new wxToggleButton( $this, wxID_ANY, _("Split"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_togBtnSplit->SetToolTipString( _("Toggle Split mode.") );
		
		$bSizer151->Add( $this->m_togBtnSplit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
		
		
		$sbSizer5->Add( $bSizer151, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 1 );
		
		$bSizer13 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_togBtnAnalog = new wxToggleButton( $this, wxID_ANY, _("Analog"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_togBtnAnalog->SetToolTipString( _("Toggle analog/digital operation.") );
		
		$bSizer13->Add( $this->m_togBtnAnalog, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
		
		
		$sbSizer5->Add( $bSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
		
		$bSizer14 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_togBtnALC = new wxToggleButton( $this, wxID_ANY, _("ALC"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_togBtnALC->SetToolTipString( _("Toggle ALC Mode.") );
		
		$bSizer14->Add( $this->m_togBtnALC, 0, wxALL, 1 );
		
		
		$sbSizer5->Add( $bSizer14, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALL, 1 );
		
		$bSizer11 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_btnTogTX = new wxToggleButton( $this, wxID_ANY, _("TX"), wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer11->Add( $this->m_btnTogTX, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
		
		
		$sbSizer5->Add( $bSizer11, 2, wxEXPAND, 1 );
		
		
		$rightSizer->Add( $sbSizer5, 2, wxALIGN_CENTER|wxALL|wxEXPAND, 3 );
		
		
		$bSizer1->Add( $rightSizer, 0, wxALL|wxEXPAND, 3 );
		
		
		$this->SetSizer( $bSizer1 );
		$this->Layout();
		$this->m_statusBar1 = $this->CreateStatusBar( 3, wxST_SIZEGRIP, wxID_ANY );
		
		// Connect Events
		$this->Connect( wxEVT_CLOSE_WINDOW, array($this, "topFrame_OnClose") );
		$this->Connect( wxEVT_PAINT, array($this, "topFrame_OnPaint") );
		$this->Connect( wxEVT_SIZE, array($this, "topFrame_OnSize") );
		$this->Connect( wxEVT_UPDATE_UI, array($this, "topFrame_OnUpdateUI") );
		$this->Connect( $this->m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnOpen") );
		$this->Connect( $this->m_menuItemOpen->GetId(), wxEVT_UPDATE_UI, array($this, "OnOpenUpdateUI") );
		$this->Connect( $this->m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnSave") );
		$this->Connect( $this->m_menuItemSave->GetId(), wxEVT_UPDATE_UI, array($this, "OnSaveUpdateUI") );
		$this->Connect( $this->m_menuItemClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnClose") );
		$this->Connect( $this->m_menuItemClose->GetId(), wxEVT_UPDATE_UI, array($this, "OnCloseUpdateUI") );
		$this->Connect( $this->m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnExit") );
		$this->Connect( $this->m_menuItemCopy->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnCopy") );
		$this->Connect( $this->m_menuItemCopy->GetId(), wxEVT_UPDATE_UI, array($this, "OnCopyUpdateUI") );
		$this->Connect( $this->m_menuItemCut->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnCut") );
		$this->Connect( $this->m_menuItemCut->GetId(), wxEVT_UPDATE_UI, array($this, "OnCutUpdateUI") );
		$this->Connect( $this->m_menuItemPaste->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnPaste") );
		$this->Connect( $this->m_menuItemPaste->GetId(), wxEVT_UPDATE_UI, array($this, "OnPasteUpdateUI") );
		$this->Connect( $this->m_menuItemAudio->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnToolsAudio") );
		$this->Connect( $this->m_menuItemAudio->GetId(), wxEVT_UPDATE_UI, array($this, "OnToolsAudioUI") );
		$this->Connect( $this->m_menuItemComCfg->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnToolsComCfg") );
		$this->Connect( $this->m_menuItemComCfg->GetId(), wxEVT_UPDATE_UI, array($this, "OnToolsComCfgUI") );
		$this->Connect( $this->m_menuItemOptions->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnToolsOptions") );
		$this->Connect( $this->m_menuItemOptions->GetId(), wxEVT_UPDATE_UI, array($this, "OnToolsOptionsUI") );
		$this->Connect( $this->m_menuItemHelpUpdates->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnHelpCheckUpdates") );
		$this->Connect( $this->m_menuItemHelpUpdates->GetId(), wxEVT_UPDATE_UI, array($this, "OnHelpCheckUpdatesUI") );
		$this->Connect( $this->m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, array($this, "OnHelpAbout") );
		$this->m_togRxID->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, array($this, "OnTogBtnRxID") ;)
		$this->m_togTxID->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, array($this, "OnTogBtnTxID") ;)
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_TOP, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_BOTTOM, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_LINEUP, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_LINEDOWN, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_PAGEUP, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_PAGEDOWN, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_THUMBTRACK, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_THUMBRELEASE, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_CHANGED, array($this, "OnCmdSliderScroll") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_BOTTOM, array($this, "OnSliderScrollBottom") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_CHANGED, array($this, "OnCmdSliderScrollChanged") );
		$this->m_sliderSQ->Connect( wxEVT_SCROLL_TOP, array($this, "OnSliderScrollTop") );
		$this->m_ckboxSQ->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, array($this, "OnCheckSQClick") );
		$this->m_togBtnSplit->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, array($this, "OnTogBtnSplitClick") ;)
		$this->m_togBtnAnalog->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, array($this, "OnTogBtnAnalogClick") ;)
		$this->m_togBtnALC->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, array($this, "OnTogBtnALCClick") ;)
		$this->m_btnTogTX->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, array($this, "OnTogBtnTXClick") ;)
	}
	
	
	function __destruct( ){
	}
	
	
	// Virtual event handlers, overide them in your derived class
	function topFrame_OnClose( $event ){
		$event->Skip();
	}
	
	function topFrame_OnPaint( $event ){
		$event->Skip();
	}
	
	function topFrame_OnSize( $event ){
		$event->Skip();
	}
	
	function topFrame_OnUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnOpen( $event ){
		$event->Skip();
	}
	
	function OnOpenUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnSave( $event ){
		$event->Skip();
	}
	
	function OnSaveUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnClose( $event ){
		$event->Skip();
	}
	
	function OnCloseUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnExit( $event ){
		$event->Skip();
	}
	
	function OnCopy( $event ){
		$event->Skip();
	}
	
	function OnCopyUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnCut( $event ){
		$event->Skip();
	}
	
	function OnCutUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnPaste( $event ){
		$event->Skip();
	}
	
	function OnPasteUpdateUI( $event ){
		$event->Skip();
	}
	
	function OnToolsAudio( $event ){
		$event->Skip();
	}
	
	function OnToolsAudioUI( $event ){
		$event->Skip();
	}
	
	function OnToolsComCfg( $event ){
		$event->Skip();
	}
	
	function OnToolsComCfgUI( $event ){
		$event->Skip();
	}
	
	function OnToolsOptions( $event ){
		$event->Skip();
	}
	
	function OnToolsOptionsUI( $event ){
		$event->Skip();
	}
	
	function OnHelpCheckUpdates( $event ){
		$event->Skip();
	}
	
	function OnHelpCheckUpdatesUI( $event ){
		$event->Skip();
	}
	
	function OnHelpAbout( $event ){
		$event->Skip();
	}
	
	function OnTogBtnRxID( $event ){
		$event->Skip();
	}
	
	function OnTogBtnTxID( $event ){
		$event->Skip();
	}
	
	function OnCmdSliderScroll( $event ){
		$event->Skip();
	}
	
	function OnSliderScrollBottom( $event ){
		$event->Skip();
	}
	
	function OnCmdSliderScrollChanged( $event ){
		$event->Skip();
	}
	
	function OnSliderScrollTop( $event ){
		$event->Skip();
	}
	
	function OnCheckSQClick( $event ){
		$event->Skip();
	}
	
	function OnTogBtnSplitClick( $event ){
		$event->Skip();
	}
	
	function OnTogBtnAnalogClick( $event ){
		$event->Skip();
	}
	
	function OnTogBtnALCClick( $event ){
		$event->Skip();
	}
	
	function OnTogBtnTXClick( $event ){
		$event->Skip();
	}
	
}

/*
 * Class DlgAbout
 */

class DlgAbout extends wxDialog {
	
	function __construct( $parent=null ){
		parent::__construct( $parent, wxID_ANY, _("About FDMDV2"), wxDefaultPosition, new wxSize( 352,343 ), wxDEFAULT_DIALOG_STYLE );
		
		$this->SetSizeHints( wxDefaultSize, wxDefaultSize );
		
		$bSizer20 = new wxBoxSizer( wxVERTICAL );
		
		$bSizer21 = new wxBoxSizer( wxVERTICAL );
		
		$bSizer23 = new wxBoxSizer( wxVERTICAL );
		
		$bSizer25 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_bitmapAbout = new wxStaticBitmap( $this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer25->Add( $this->m_bitmapAbout, 1, wxALL|wxEXPAND, 5 );
		
		
		$bSizer23->Add( $bSizer25, 1, wxEXPAND, 5 );
		
		$bSizer26 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_staticText1 = new wxStaticText( $this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText1->Wrap( -1 );
		$bSizer26->Add( $this->m_staticText1, 1, wxALL, 5 );
		
		
		$bSizer23->Add( $bSizer26, 0, 0, 5 );
		
		
		$bSizer21->Add( $bSizer23, 1, wxEXPAND, 5 );
		
		$bSizer24 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_textCtrl2 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer24->Add( $this->m_textCtrl2, 1, wxALL|wxEXPAND, 5 );
		
		
		$bSizer21->Add( $bSizer24, 1, wxEXPAND, 5 );
		
		
		$bSizer20->Add( $bSizer21, 1, wxEXPAND, 5 );
		
		$bSizer22 = new wxBoxSizer( wxVERTICAL );
		
		$this->m_button9 = new wxButton( $this, wxID_ANY, _("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer22->Add( $this->m_button9, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxALL, 5 );
		
		
		$bSizer20->Add( $bSizer22, 0, wxALIGN_RIGHT, 5 );
		
		
		$this->SetSizer( $bSizer20 );
		$this->Layout();
		
		$this->Centre( wxBOTH );
		
		// Connect Events
		$this->Connect( wxEVT_INIT_DIALOG, array($this, "OnInitDialog") );
	}
	
	
	function __destruct( ){
	}
	
	
	// Virtual event handlers, overide them in your derived class
	function OnInitDialog( $event ){
		$event->Skip();
	}
	
}

/*
 * Class DlgAudio
 */

class DlgAudio extends public dlg_audio {
	
	function __construct( $parent=null ){
		parent::__construct( $parent, wxID_ANY, _("Audio Options"), wxDefaultPosition, new wxSize( 465,302 ), wxDEFAULT_DIALOG_STYLE );
		
		$this->SetSizeHints( wxDefaultSize, wxDefaultSize );
		
		$bSizer27 = new wxBoxSizer( wxVERTICAL );
		
		$bSizer271 = new wxBoxSizer( wxHORIZONTAL );
		
		$bSizer31 = new wxBoxSizer( wxVERTICAL );
		
		$m_lbOutputStreamChoices = array();
		$this->m_lbOutputStream = new wxListBox( $this, wxID_ANY, wxDefaultPosition, wxDefaultSize, $m_lbOutputStreamChoices, wxLB_SINGLE );
		$bSizer31->Add( $this->m_lbOutputStream, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
		
		$this->m_textCtrl114 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer31->Add( $this->m_textCtrl114, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
		
		
		$bSizer271->Add( $bSizer31, 1, wxEXPAND, 5 );
		
		$bSizer281 = new wxBoxSizer( wxVERTICAL );
		
		$m_listBox3Choices = array();
		$this->m_listBox3 = new wxListBox( $this, wxID_ANY, wxDefaultPosition, wxDefaultSize, $m_listBox3Choices, 0 );
		$bSizer281->Add( $this->m_listBox3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
		
		$this->m_textCtrl15 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer281->Add( $this->m_textCtrl15, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
		
		
		$bSizer271->Add( $bSizer281, 1, wxEXPAND, 5 );
		
		
		$bSizer27->Add( $bSizer271, 1, wxEXPAND, 5 );
		
		$bSizer26 = new wxBoxSizer( wxHORIZONTAL );
		
		$bSizer29 = new wxBoxSizer( wxVERTICAL );
		
		$m_listBox4Choices = array();
		$this->m_listBox4 = new wxListBox( $this, wxID_ANY, wxDefaultPosition, wxDefaultSize, $m_listBox4Choices, 0 );
		$bSizer29->Add( $this->m_listBox4, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
		
		$this->m_textCtrl16 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer29->Add( $this->m_textCtrl16, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
		
		
		$bSizer26->Add( $bSizer29, 1, wxEXPAND, 5 );
		
		$bSizer30 = new wxBoxSizer( wxVERTICAL );
		
		$m_lbInputStreamChoices = array();
		$this->m_lbInputStream = new wxListBox( $this, wxID_ANY, wxDefaultPosition, wxDefaultSize, $m_lbInputStreamChoices, wxLB_SINGLE );
		$bSizer30->Add( $this->m_lbInputStream, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
		
		$this->m_textCtrl13 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$bSizer30->Add( $this->m_textCtrl13, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
		
		
		$bSizer26->Add( $bSizer30, 1, wxEXPAND, 1 );
		
		
		$bSizer27->Add( $bSizer26, 1, wxEXPAND, 1 );
		
		$bSizer28 = new wxBoxSizer( wxVERTICAL );
		
		$m_sdbSizer3 = new wxStdDialogButtonSizer();
		$this->m_sdbSizer3OK = new wxButton( $this, wxID_OK );
		$m_sdbSizer3->AddButton( $this->m_sdbSizer3OK );
		$this->m_sdbSizer3Apply = new wxButton( $this, wxID_APPLY );
		$m_sdbSizer3->AddButton( $this->m_sdbSizer3Apply );
		$this->m_sdbSizer3Cancel = new wxButton( $this, wxID_CANCEL );
		$m_sdbSizer3->AddButton( $this->m_sdbSizer3Cancel );
		$m_sdbSizer3->Realize();
		
		$bSizer28->Add( $m_sdbSizer3, 1, wxEXPAND, 5 );
		
		
		$bSizer27->Add( $bSizer28, 0, wxEXPAND, 5 );
		
		
		$this->SetSizer( $bSizer27 );
		$this->Layout();
		
		$this->Centre( wxBOTH );
		
		// Connect Events
		$this->m_sdbSizer3Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, array($this, "OnApply") );
		$this->m_sdbSizer3Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, array($this, "OnCancel") );
		$this->m_sdbSizer3OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, array($this, "OnOK") );
	}
	
	
	function __destruct( ){
	}
	
	
	// Virtual event handlers, overide them in your derived class
	function OnApply( $event ){
		$event->Skip();
	}
	
	function OnCancel( $event ){
		$event->Skip();
	}
	
	function OnOK( $event ){
		$event->Skip();
	}
	
}

/*
 * Class DlgOptions
 */

class DlgOptions extends wxDialog {
	
	function __construct( $parent=null ){
		parent::__construct( $parent, wxID_ANY, _("Program Options"), wxDefaultPosition, new wxSize( 332,364 ), wxDEFAULT_DIALOG_STYLE );
		
		$this->SetSizeHints( wxDefaultSize, wxDefaultSize );
		
		$bSizer30 = new wxBoxSizer( wxVERTICAL );
		
		$sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( $this, wxID_ANY, _("Config Options") ), wxHORIZONTAL );
		
		$gSizer2 = new wxGridSizer( 6, 2, 0, 0 );
		
		$this->m_staticText2 = new wxStaticText( $this, wxID_ANY, _("Option #1:"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText2->Wrap( -1 );
		$gSizer2->Add( $this->m_staticText2, 1, wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl3 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer2->Add( $this->m_textCtrl3, 1, wxALL, 5 );
		
		$this->m_staticText3 = new wxStaticText( $this, wxID_ANY, _("Option #2:"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText3->Wrap( -1 );
		$gSizer2->Add( $this->m_staticText3, 1, wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl4 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer2->Add( $this->m_textCtrl4, 1, wxALL, 5 );
		
		$this->m_staticText4 = new wxStaticText( $this, wxID_ANY, _("Option #3:"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText4->Wrap( -1 );
		$gSizer2->Add( $this->m_staticText4, 1, wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl5 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer2->Add( $this->m_textCtrl5, 1, wxALL, 5 );
		
		$this->m_staticText5 = new wxStaticText( $this, wxID_ANY, _("Option #4:"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText5->Wrap( -1 );
		$gSizer2->Add( $this->m_staticText5, 1, wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl6 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer2->Add( $this->m_textCtrl6, 1, wxALL, 5 );
		
		$this->m_staticText6 = new wxStaticText( $this, wxID_ANY, _("Option #5:"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText6->Wrap( -1 );
		$gSizer2->Add( $this->m_staticText6, 1, wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl7 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer2->Add( $this->m_textCtrl7, 1, wxALL, 5 );
		
		$this->m_staticText7 = new wxStaticText( $this, wxID_ANY, _("Option #6:"), wxDefaultPosition, wxDefaultSize, 0 );
		$this->m_staticText7->Wrap( -1 );
		$gSizer2->Add( $this->m_staticText7, 1, wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl8 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer2->Add( $this->m_textCtrl8, 1, wxALL, 5 );
		
		
		$sbSizer5->Add( $gSizer2, 1, wxEXPAND, 5 );
		
		
		$bSizer30->Add( $sbSizer5, 1, wxEXPAND, 5 );
		
		$m_sdbSizer4 = new wxStdDialogButtonSizer();
		$this->m_sdbSizer4OK = new wxButton( $this, wxID_OK );
		$m_sdbSizer4->AddButton( $this->m_sdbSizer4OK );
		$this->m_sdbSizer4Cancel = new wxButton( $this, wxID_CANCEL );
		$m_sdbSizer4->AddButton( $this->m_sdbSizer4Cancel );
		$m_sdbSizer4->Realize();
		
		$bSizer30->Add( $m_sdbSizer4, 0, wxALIGN_RIGHT, 5 );
		
		
		$this->SetSizer( $bSizer30 );
		$this->Layout();
		
		$this->Centre( wxBOTH );
		
		// Connect Events
		$this->Connect( wxEVT_CLOSE_WINDOW, array($this, "OnClose") );
		$this->Connect( wxEVT_INIT_DIALOG, array($this, "OnInitDialog") );
	}
	
	
	function __destruct( ){
	}
	
	
	// Virtual event handlers, overide them in your derived class
	function OnClose( $event ){
		$event->Skip();
	}
	
	function OnInitDialog( $event ){
		$event->Skip();
	}
	
}

/*
 * Class DlgComPorts
 */

class DlgComPorts extends wxDialog {
	
	function __construct( $parent=null ){
		parent::__construct( $parent, wxID_ANY, _("Select Com Port"), wxDefaultPosition, new wxSize( 399,226 ), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		
		$this->SetSizeHints( wxDefaultSize, wxDefaultSize );
		
		$bSizer30 = new wxBoxSizer( wxVERTICAL );
		
		$gSizer3 = new wxGridSizer( 2, 2, 0, 0 );
		
		$this->m_staticText8 = new wxStaticText( $this, wxID_ANY, _("Available Ports:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
		$this->m_staticText8->Wrap( -1 );
		$gSizer3->Add( $this->m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_listCtrlPortSelect = new wxListCtrl( $this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_SINGLE_SEL );
		$gSizer3->Add( $this->m_listCtrlPortSelect, 1, wxALL|wxEXPAND, 5 );
		
		$this->m_staticText9 = new wxStaticText( $this, wxID_ANY, _("Use Port:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
		$this->m_staticText9->Wrap( -1 );
		$gSizer3->Add( $this->m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
		
		$this->m_textCtrl10 = new wxTextCtrl( $this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		$gSizer3->Add( $this->m_textCtrl10, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
		
		
		$bSizer30->Add( $gSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
		
		$m_sdbSizer3 = new wxStdDialogButtonSizer();
		$this->m_sdbSizer3OK = new wxButton( $this, wxID_OK );
		$m_sdbSizer3->AddButton( $this->m_sdbSizer3OK );
		$this->m_sdbSizer3Cancel = new wxButton( $this, wxID_CANCEL );
		$m_sdbSizer3->AddButton( $this->m_sdbSizer3Cancel );
		$m_sdbSizer3->Realize();
		
		$bSizer30->Add( $m_sdbSizer3, 0, wxEXPAND, 5 );
		
		
		$this->SetSizer( $bSizer30 );
		$this->Layout();
		
		$this->Centre( wxBOTH );
		
		// Connect Events
		$this->m_listCtrlPortSelect->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, array($this, "OnListItemSelected") );
	}
	
	
	function __destruct( ){
	}
	
	
	// Virtual event handlers, overide them in your derived class
	function OnListItemSelected( $event ){
		$event->Skip();
	}
	
}

?>
