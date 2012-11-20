///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
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
#include <wx/radiobut.h>
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
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
        wxStaticText* m_textSNR;
        wxGauge* m_gaugeLevel;
        wxTextCtrl* m_txtCtrl;
        wxSlider* m_sliderSQ;
        wxCheckBox* m_ckboxSQ;
        wxStaticText* m_textSQ;
        wxStatusBar* m_statusBar1;
        wxRadioButton *m_rbSync;

        // Virtual event handlers, overide them in your derived class
        virtual void topFrame_OnClose( wxCloseEvent& event ) { event.Skip(); }
        virtual void topFrame_OnPaint( wxPaintEvent& event ) { event.Skip(); }
        virtual void topFrame_OnSize( wxSizeEvent& event ) { event.Skip(); }
        virtual void topFrame_OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }

#ifdef UNIMPLEMENTED
        virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnOpenUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnSaveUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnCloseUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
#endif
        virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
#ifdef UNIMPLEMENTED
        virtual void OnCopy( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnCopyUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnCut( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnCutUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnPaste( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnPasteUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
#endif
        virtual void OnToolsAudio( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnToolsAudioUI( wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnToolsComCfg( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnToolsComCfgUI( wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnToolsOptions( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnToolsOptionsUI( wxUpdateUIEvent& event ) { event.Skip(); }
#ifdef UNIMPLEMENTED
        //virtual void OnCaptureRxStream( wxCommandEvent& event ) { event.Skip(); }
        //virtual void OnCaptureTxStream( wxCommandEvent& event ) { event.Skip(); }
#endif
        virtual void OnPlayFileToMicIn( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnRecFileFromRadio( wxCommandEvent& event ) { event.Skip(); }

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

        virtual void OnTogBtnLoopRx( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTogBtnLoopTx( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTogBtnOnOff( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTogBtnSplitClick( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTogBtnAnalogClick( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTogBtnALCClick( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTogBtnTXClick( wxCommandEvent& event ) { event.Skip(); }

        virtual void OnTogBtnSplitClickUI(wxUpdateUIEvent& event) { event.Skip(); }
        virtual void OnTogBtnAnalogClickUI(wxUpdateUIEvent& event) { event.Skip(); }
        virtual void OnTogBtnALCClickUI(wxUpdateUIEvent& event) { event.Skip(); }
        virtual void OnTogBtnRxIDUI(wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnTogBtnTxIDUI(wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnTogBtnTXClickUI(wxUpdateUIEvent& event ) { event.Skip(); }
        virtual void OnTogBtnOnOffUI(wxUpdateUIEvent& event ) { event.Skip(); }

    public:
        wxToggleButton* m_togRxID;
        wxToggleButton* m_togTxID;
        wxToggleButton* m_togBtnOnOff;
        wxToggleButton* m_togBtnSplit;
        wxToggleButton* m_togBtnAnalog;
        wxToggleButton* m_togBtnALC;
        wxToggleButton* m_btnTogTX;
        wxToggleButton* m_togBtnLoopRx;
        wxToggleButton* m_togBtnLoopTx;

        wxAuiNotebook* m_auiNbookCtrl;

        TopFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("FDMDV2"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 761,500 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );

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
        virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
        virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }


    public:

        DlgAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About FDMDV2"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 352,343 ), long style = wxDEFAULT_DIALOG_STYLE );
        ~DlgAbout();

};

/*
///////////////////////////////////////////////////////////////////////////////
/// Class DlgAudio
///////////////////////////////////////////////////////////////////////////////
class DlgAudio : public wxDialog
{
    private:

    protected:
        wxNotebook* m_nbAudioOptions;
        wxPanel* m_panelDevices;
        wxRadioButton* m_radioBtnPortAudio;
        wxStaticText* m_staticText151;
        wxTextCtrl* m_textNumChOut;
        wxStaticText* m_staticText141;
        wxTextCtrl* m_textNumChIn;
        wxStaticText* m_staticText14;
        wxComboBox* m_comboAudioCodec;
        wxStaticText* m_staticText15;
        wxComboBox* m_comboCodecTx;
        wxStaticText* m_staticText16;
        wxComboBox* m_comboRadioRx;
        wxStaticText* m_staticText17;
        wxComboBox* m_comboCodecSpkr;
        wxRadioButton* m_radioBtnFileOnly;
        wxPanel* m_panelSettings;
        wxListBox* m_lbRxInput;
        wxTextCtrl* m_textRxInput;
        wxListBox* m_lbTxOutput;
        wxTextCtrl* m_textTxOutput;
        wxListBox* m_lbVoiceInput;
        wxTextCtrl* m_textVoiceInput;
        wxListBox* m_lbVoiceOutput;
        wxTextCtrl* m_textVoiceOutput;
        wxStdDialogButtonSizer* m_sdbSizer3;
        wxButton* m_sdbSizer3OK;
        wxButton* m_sdbSizer3Apply;
        wxButton* m_sdbSizer3Cancel;

        // Virtual event handlers, overide them in your derived class
        virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
        virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
        virtual void OnRxInputSelect( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnTxOutputSelect( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnVoiceInputSelect( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnVoiceOutputSelect( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }


    public:

        DlgAudio( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Audio Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 555,419 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~DlgAudio();

};
*/

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

/*
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
        wxTextCtrl* m_textRigCtrlPort;
        wxStaticText* m_staticText91;
        wxTextCtrl* m_textRigCtrlBaud;
        wxStaticText* m_staticText911;
        wxTextCtrl* m_textRigCtrlDatabits;
        wxStaticText* m_staticText912;
        wxTextCtrl* m_textRigCtrlStopbits;
        wxStaticText* m_staticText913;
        wxTextCtrl* m_textRigCtrlParity;
        wxStdDialogButtonSizer* m_sdbSizer5;
        wxButton* m_sdbSizer5OK;
        wxButton* m_sdbSizer5Apply;
        wxButton* m_sdbSizer5Cancel;

        // Virtual event handlers, overide them in your derived class
        virtual void OnListItemSelected( wxListEvent& event ) { event.Skip(); }
        virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
        virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
        virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }


    public:

        DlgComPorts( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select Com Port"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 399,354 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~DlgComPorts();

};
*/

#endif //__TOPFRAME_H__
