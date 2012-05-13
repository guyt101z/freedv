///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 17 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TOPFRAME_H__
#define __TOPFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/aui/auibook.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

///////////////////////////////////////////////////////////////////////////

#define ID_OPEN 1000
#define ID_SAVE 1001
#define ID_CLOSE 1002
#define ID_EXIT 1003
#define ID_COPY 1004
#define ID_CUT 1005
#define ID_PASTE 1006
#define ID_OPTIONS 1007
#define ID_ABOUT 1008

///////////////////////////////////////////////////////////////////////////////
/// Class TopFrame
///////////////////////////////////////////////////////////////////////////////
class TopFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubarMain;
		wxMenu* file;
		wxMenu* edit;
		wxMenu* tools;
		wxMenu* help;
		wxGauge* m_gaugeSNR;
		wxTextCtrl* m_textSNR;
		wxGauge* m_gaugeLevel;
		wxTextCtrl* m_txtCtrl;
		wxToggleButton* m_togRxID;
		wxToggleButton* m_togTxID;
		wxSlider* m_sliderSQ;
		wxCheckBox* m_ckboxSQ;
		wxToggleButton* m_togBtnSplit;
		wxToggleButton* m_togBtnAnalog;
		wxToggleButton* m_togBtnALC;
		wxToggleButton* m_btnTogTX;
		wxStatusBar* m_statusBar1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void topFrame_OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void topFrame_OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void topFrame_OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void topFrame_OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpenUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCloseUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCopyUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnCut( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCutUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnPaste( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPasteUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnToolsAudio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolsAudioUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnToolsComCfg( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolsComCfgUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnToolsOptions( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolsOptionsUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnHelpCheckUpdates( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpCheckUpdatesUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnHelpAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTogBtnRxID( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTogBtnTxID( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCmdSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnSliderScrollBottom( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnCmdSliderScrollChanged( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnSliderScrollTop( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnCheckSQClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTogBtnSplitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTogBtnAnalogClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTogBtnALCClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTogBtnTXClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxAuiNotebook* m_auiNbookCtrl;
		
		TopFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("FDMDV2"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 732,507 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		
		~TopFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DlgAbout
///////////////////////////////////////////////////////////////////////////////
class DlgAbout : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmapAbout;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl2;
		wxButton* m_button9;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		
	
	public:
		
		DlgAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About FDMDV2"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 352,343 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~DlgAbout();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DlgAudio
///////////////////////////////////////////////////////////////////////////////
class DlgAudio : public wxDialog 
{
	private:
	
	protected:
		wxListCtrl* m_listCtrl1;
		wxListCtrl* m_listCtrl2;
		wxListCtrl* m_listCtrl3;
		wxListCtrl* m_listCtrl4;
		wxStdDialogButtonSizer* m_sdbSizer3;
		wxButton* m_sdbSizer3OK;
		wxButton* m_sdbSizer3Apply;
		wxButton* m_sdbSizer3Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DlgAudio( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Audio Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 465,302 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~DlgAudio();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DlgOptions
///////////////////////////////////////////////////////////////////////////////
class DlgOptions : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl3;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrl5;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrl6;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textCtrl7;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_textCtrl8;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4OK;
		wxButton* m_sdbSizer4Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		
	
	public:
		
		DlgOptions( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Program Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 332,364 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~DlgOptions();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DlgComPorts
///////////////////////////////////////////////////////////////////////////////
class DlgComPorts : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText8;
		wxListCtrl* m_listCtrlPortSelect;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_textCtrl10;
		wxStdDialogButtonSizer* m_sdbSizer3;
		wxButton* m_sdbSizer3OK;
		wxButton* m_sdbSizer3Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnListItemSelected( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		
		DlgComPorts( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select Com Port"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 399,226 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DlgComPorts();
	
};

#endif //__TOPFRAME_H__
