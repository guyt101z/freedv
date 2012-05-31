# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Mar 17 2012)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc
import wx.aui

import gettext
_ = gettext.gettext

ID_OPEN = 1000
ID_SAVE = 1001
ID_CLOSE = 1002
ID_EXIT = 1003
ID_COPY = 1004
ID_CUT = 1005
ID_PASTE = 1006
ID_OPTIONS = 1007
ID_ABOUT = 1008

###########################################################################
## Class TopFrame
###########################################################################

class TopFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = _(u"FDMDV2"), pos = wx.DefaultPosition, size = wx.Size( 732,507 ), style = wx.DEFAULT_FRAME_STYLE|wx.RESIZE_BORDER|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetForegroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_WINDOWTEXT ) )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_3DLIGHT ) )
		
		self.m_menubarMain = wx.MenuBar( wx.MB_DOCKABLE )
		self.file = wx.Menu()
		self.m_menuItemOpen = wx.MenuItem( self.file, ID_OPEN, _(u"&Open"), _(u"Open File"), wx.ITEM_NORMAL )
		self.file.AppendItem( self.m_menuItemOpen )
		
		self.m_menuItemSave = wx.MenuItem( self.file, ID_SAVE, _(u"&Save"), _(u"Save current file"), wx.ITEM_NORMAL )
		self.file.AppendItem( self.m_menuItemSave )
		
		self.m_menuItemClose = wx.MenuItem( self.file, ID_CLOSE, _(u"&Close"), _(u"Close current file"), wx.ITEM_NORMAL )
		self.file.AppendItem( self.m_menuItemClose )
		
		self.file.AppendSeparator()
		
		self.m_menuItemExit = wx.MenuItem( self.file, ID_EXIT, _(u"E&xit"), _(u"Exit Program"), wx.ITEM_NORMAL )
		self.file.AppendItem( self.m_menuItemExit )
		
		self.m_menubarMain.Append( self.file, _(u"&File") ) 
		
		self.edit = wx.Menu()
		self.m_menuItemCopy = wx.MenuItem( self.edit, ID_COPY, _(u"&Copy"), _(u"Copy selection"), wx.ITEM_NORMAL )
		self.edit.AppendItem( self.m_menuItemCopy )
		self.m_menuItemCopy.Enable( False )
		
		self.m_menuItemCut = wx.MenuItem( self.edit, ID_CUT, _(u"Cut"), _(u"Cut Selection"), wx.ITEM_NORMAL )
		self.edit.AppendItem( self.m_menuItemCut )
		self.m_menuItemCut.Enable( False )
		
		self.m_menuItemPaste = wx.MenuItem( self.edit, ID_PASTE, _(u"&Paste"), _(u"Paste selection"), wx.ITEM_NORMAL )
		self.edit.AppendItem( self.m_menuItemPaste )
		self.m_menuItemPaste.Enable( False )
		
		self.m_menubarMain.Append( self.edit, _(u"&Edit") ) 
		
		self.tools = wx.Menu()
		self.m_menuItemAudio = wx.MenuItem( self.tools, wx.ID_ANY, _(u"&Audio"), wx.EmptyString, wx.ITEM_NORMAL )
		self.tools.AppendItem( self.m_menuItemAudio )
		
		self.m_menuItemComCfg = wx.MenuItem( self.tools, wx.ID_ANY, _(u"Com Port Config"), wx.EmptyString, wx.ITEM_NORMAL )
		self.tools.AppendItem( self.m_menuItemComCfg )
		
		self.m_menuItemOptions = wx.MenuItem( self.tools, ID_OPTIONS, _(u"&Options"), _(u"Set preferences"), wx.ITEM_NORMAL )
		self.tools.AppendItem( self.m_menuItemOptions )
		
		self.m_menubarMain.Append( self.tools, _(u"&Tools") ) 
		
		self.help = wx.Menu()
		self.m_menuItemHelpUpdates = wx.MenuItem( self.help, wx.ID_ANY, _(u"Check for Updates"), wx.EmptyString, wx.ITEM_NORMAL )
		self.help.AppendItem( self.m_menuItemHelpUpdates )
		self.m_menuItemHelpUpdates.Enable( False )
		
		self.m_menuItemAbout = wx.MenuItem( self.help, ID_ABOUT, _(u"&About"), _(u"About this program"), wx.ITEM_NORMAL )
		self.help.AppendItem( self.m_menuItemAbout )
		
		self.m_menubarMain.Append( self.help, _(u"&Help") ) 
		
		self.SetMenuBar( self.m_menubarMain )
		
		bSizer1 = wx.BoxSizer( wx.HORIZONTAL )
		
		leftSizer = wx.BoxSizer( wx.VERTICAL )
		
		snrSizer = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, _(u"SNR") ), wx.VERTICAL )
		
		self.m_gaugeSNR = wx.Gauge( self, wx.ID_ANY, 100, wx.DefaultPosition, wx.Size( 15,135 ), wx.GA_SMOOTH|wx.GA_VERTICAL )
		snrSizer.Add( self.m_gaugeSNR, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 10 )
		
		bSizer29 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_textSNR = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.Point( -1,-1 ), wx.Size( 35,25 ), wx.TE_READONLY )
		self.m_textSNR.SetMinSize( wx.Size( 35,25 ) )
		
		bSizer29.Add( self.m_textSNR, 0, wx.ALIGN_CENTER|wx.ALL, 1 )
		
		
		snrSizer.Add( bSizer29, 0, wx.EXPAND, 1 )
		
		
		leftSizer.Add( snrSizer, 2, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 1 )
		
		levelSizer = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, _(u"Level") ), wx.VERTICAL )
		
		self.m_gaugeLevel = wx.Gauge( self, wx.ID_ANY, 100, wx.DefaultPosition, wx.Size( 15,135 ), wx.GA_SMOOTH|wx.GA_VERTICAL )
		levelSizer.Add( self.m_gaugeLevel, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 10 )
		
		
		leftSizer.Add( levelSizer, 2, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 1 )
		
		
		bSizer1.Add( leftSizer, 0, wx.ALL|wx.EXPAND, 5 )
		
		centerSizer = wx.BoxSizer( wx.VERTICAL )
		
		upperSizer = wx.BoxSizer( wx.VERTICAL )
		
		self.m_auiNbookCtrl = wx.aui.AuiNotebook( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.aui.AUI_NB_BOTTOM|wx.aui.AUI_NB_DEFAULT_STYLE )
		
		upperSizer.Add( self.m_auiNbookCtrl, 1, wx.ALIGN_TOP|wx.EXPAND, 1 )
		
		
		centerSizer.Add( upperSizer, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_TOP|wx.EXPAND, 0 )
		
		lowerSizer = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer15 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_txtCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer15.Add( self.m_txtCtrl, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 5 )
		
		
		lowerSizer.Add( bSizer15, 1, wx.EXPAND, 5 )
		
		bSizer141 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_togRxID = wx.ToggleButton( self, wx.ID_ANY, _(u"RxID"), wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer141.Add( self.m_togRxID, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_LEFT|wx.ALL|wx.FIXED_MINSIZE, 5 )
		
		self.m_togTxID = wx.ToggleButton( self, wx.ID_ANY, _(u"TxID"), wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer141.Add( self.m_togTxID, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		
		lowerSizer.Add( bSizer141, 0, wx.ALIGN_RIGHT, 5 )
		
		
		centerSizer.Add( lowerSizer, 0, wx.ALIGN_BOTTOM|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.EXPAND, 2 )
		
		
		bSizer1.Add( centerSizer, 4, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 1 )
		
		rightSizer = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer3 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, _(u"SQ") ), wx.VERTICAL )
		
		self.m_sliderSQ = wx.Slider( self, wx.ID_ANY, 50, 0, 100, wx.DefaultPosition, wx.Size( -1,100 ), wx.SL_AUTOTICKS|wx.SL_INVERSE|wx.SL_VERTICAL )
		sbSizer3.Add( self.m_sliderSQ, 2, wx.ALIGN_CENTER|wx.ALL, 1 )
		
		bSizer131 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_ckboxSQ = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.CHK_2STATE )
		bSizer131.Add( self.m_ckboxSQ, 0, wx.ALIGN_CENTER|wx.ALL, 5 )
		
		
		sbSizer3.Add( bSizer131, 0, wx.ALIGN_CENTER, 5 )
		
		
		rightSizer.Add( sbSizer3, 3, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 3 )
		
		sbSizer5 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, _(u"Control") ), wx.VERTICAL )
		
		bSizer151 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_togBtnSplit = wx.ToggleButton( self, wx.ID_ANY, _(u"Split"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_togBtnSplit.SetToolTipString( _(u"Toggle Split mode.") )
		
		bSizer151.Add( self.m_togBtnSplit, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 1 )
		
		
		sbSizer5.Add( bSizer151, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 1 )
		
		bSizer13 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_togBtnAnalog = wx.ToggleButton( self, wx.ID_ANY, _(u"Analog"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_togBtnAnalog.SetToolTipString( _(u"Toggle analog/digital operation.") )
		
		bSizer13.Add( self.m_togBtnAnalog, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 1 )
		
		
		sbSizer5.Add( bSizer13, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 1 )
		
		bSizer14 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_togBtnALC = wx.ToggleButton( self, wx.ID_ANY, _(u"ALC"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_togBtnALC.SetToolTipString( _(u"Toggle ALC Mode.") )
		
		bSizer14.Add( self.m_togBtnALC, 0, wx.ALL, 1 )
		
		
		sbSizer5.Add( bSizer14, 0, wx.ALIGN_CENTER|wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 1 )
		
		bSizer11 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_btnTogTX = wx.ToggleButton( self, wx.ID_ANY, _(u"TX"), wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer11.Add( self.m_btnTogTX, 1, wx.ALIGN_CENTER|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 1 )
		
		
		sbSizer5.Add( bSizer11, 2, wx.EXPAND, 1 )
		
		
		rightSizer.Add( sbSizer5, 2, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 3 )
		
		
		bSizer1.Add( rightSizer, 0, wx.ALL|wx.EXPAND, 3 )
		
		
		self.SetSizer( bSizer1 )
		self.Layout()
		self.m_statusBar1 = self.CreateStatusBar( 3, wx.ST_SIZEGRIP, wx.ID_ANY )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.topFrame_OnClose )
		self.Bind( wx.EVT_PAINT, self.topFrame_OnPaint )
		self.Bind( wx.EVT_SIZE, self.topFrame_OnSize )
		self.Bind( wx.EVT_UPDATE_UI, self.topFrame_OnUpdateUI )
		self.Bind( wx.EVT_MENU, self.OnOpen, id = self.m_menuItemOpen.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnOpenUpdateUI, id = self.m_menuItemOpen.GetId() )
		self.Bind( wx.EVT_MENU, self.OnSave, id = self.m_menuItemSave.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnSaveUpdateUI, id = self.m_menuItemSave.GetId() )
		self.Bind( wx.EVT_MENU, self.OnClose, id = self.m_menuItemClose.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnCloseUpdateUI, id = self.m_menuItemClose.GetId() )
		self.Bind( wx.EVT_MENU, self.OnExit, id = self.m_menuItemExit.GetId() )
		self.Bind( wx.EVT_MENU, self.OnCopy, id = self.m_menuItemCopy.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnCopyUpdateUI, id = self.m_menuItemCopy.GetId() )
		self.Bind( wx.EVT_MENU, self.OnCut, id = self.m_menuItemCut.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnCutUpdateUI, id = self.m_menuItemCut.GetId() )
		self.Bind( wx.EVT_MENU, self.OnPaste, id = self.m_menuItemPaste.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnPasteUpdateUI, id = self.m_menuItemPaste.GetId() )
		self.Bind( wx.EVT_MENU, self.OnToolsAudio, id = self.m_menuItemAudio.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnToolsAudioUI, id = self.m_menuItemAudio.GetId() )
		self.Bind( wx.EVT_MENU, self.OnToolsComCfg, id = self.m_menuItemComCfg.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnToolsComCfgUI, id = self.m_menuItemComCfg.GetId() )
		self.Bind( wx.EVT_MENU, self.OnToolsOptions, id = self.m_menuItemOptions.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnToolsOptionsUI, id = self.m_menuItemOptions.GetId() )
		self.Bind( wx.EVT_MENU, self.OnHelpCheckUpdates, id = self.m_menuItemHelpUpdates.GetId() )
		self.Bind( wx.EVT_UPDATE_UI, self.OnHelpCheckUpdatesUI, id = self.m_menuItemHelpUpdates.GetId() )
		self.Bind( wx.EVT_MENU, self.OnHelpAbout, id = self.m_menuItemAbout.GetId() )
		self.m_togRxID.Bind( wx.EVT_TOGGLEBUTTON, self.OnTogBtnRxID )
		self.m_togTxID.Bind( wx.EVT_TOGGLEBUTTON, self.OnTogBtnTxID )
		self.m_sliderSQ.Bind( wx.EVT_COMMAND_SCROLL, self.OnCmdSliderScroll )
		self.m_sliderSQ.Bind( wx.EVT_SCROLL_BOTTOM, self.OnSliderScrollBottom )
		self.m_sliderSQ.Bind( wx.EVT_SCROLL_CHANGED, self.OnCmdSliderScrollChanged )
		self.m_sliderSQ.Bind( wx.EVT_SCROLL_TOP, self.OnSliderScrollTop )
		self.m_ckboxSQ.Bind( wx.EVT_CHECKBOX, self.OnCheckSQClick )
		self.m_togBtnSplit.Bind( wx.EVT_TOGGLEBUTTON, self.OnTogBtnSplitClick )
		self.m_togBtnAnalog.Bind( wx.EVT_TOGGLEBUTTON, self.OnTogBtnAnalogClick )
		self.m_togBtnALC.Bind( wx.EVT_TOGGLEBUTTON, self.OnTogBtnALCClick )
		self.m_btnTogTX.Bind( wx.EVT_TOGGLEBUTTON, self.OnTogBtnTXClick )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def topFrame_OnClose( self, event ):
		event.Skip()
	
	def topFrame_OnPaint( self, event ):
		event.Skip()
	
	def topFrame_OnSize( self, event ):
		event.Skip()
	
	def topFrame_OnUpdateUI( self, event ):
		event.Skip()
	
	def OnOpen( self, event ):
		event.Skip()
	
	def OnOpenUpdateUI( self, event ):
		event.Skip()
	
	def OnSave( self, event ):
		event.Skip()
	
	def OnSaveUpdateUI( self, event ):
		event.Skip()
	
	def OnClose( self, event ):
		event.Skip()
	
	def OnCloseUpdateUI( self, event ):
		event.Skip()
	
	def OnExit( self, event ):
		event.Skip()
	
	def OnCopy( self, event ):
		event.Skip()
	
	def OnCopyUpdateUI( self, event ):
		event.Skip()
	
	def OnCut( self, event ):
		event.Skip()
	
	def OnCutUpdateUI( self, event ):
		event.Skip()
	
	def OnPaste( self, event ):
		event.Skip()
	
	def OnPasteUpdateUI( self, event ):
		event.Skip()
	
	def OnToolsAudio( self, event ):
		event.Skip()
	
	def OnToolsAudioUI( self, event ):
		event.Skip()
	
	def OnToolsComCfg( self, event ):
		event.Skip()
	
	def OnToolsComCfgUI( self, event ):
		event.Skip()
	
	def OnToolsOptions( self, event ):
		event.Skip()
	
	def OnToolsOptionsUI( self, event ):
		event.Skip()
	
	def OnHelpCheckUpdates( self, event ):
		event.Skip()
	
	def OnHelpCheckUpdatesUI( self, event ):
		event.Skip()
	
	def OnHelpAbout( self, event ):
		event.Skip()
	
	def OnTogBtnRxID( self, event ):
		event.Skip()
	
	def OnTogBtnTxID( self, event ):
		event.Skip()
	
	def OnCmdSliderScroll( self, event ):
		event.Skip()
	
	def OnSliderScrollBottom( self, event ):
		event.Skip()
	
	def OnCmdSliderScrollChanged( self, event ):
		event.Skip()
	
	def OnSliderScrollTop( self, event ):
		event.Skip()
	
	def OnCheckSQClick( self, event ):
		event.Skip()
	
	def OnTogBtnSplitClick( self, event ):
		event.Skip()
	
	def OnTogBtnAnalogClick( self, event ):
		event.Skip()
	
	def OnTogBtnALCClick( self, event ):
		event.Skip()
	
	def OnTogBtnTXClick( self, event ):
		event.Skip()
	

###########################################################################
## Class DlgAbout
###########################################################################

class DlgAbout ( wx.Dialog ):
	
	def __init__( self, parent ):
		wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = _(u"About FDMDV2"), pos = wx.DefaultPosition, size = wx.Size( 352,343 ), style = wx.DEFAULT_DIALOG_STYLE )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		
		bSizer20 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer21 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer23 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer25 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_bitmapAbout = wx.StaticBitmap( self, wx.ID_ANY, wx.NullBitmap, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer25.Add( self.m_bitmapAbout, 1, wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer23.Add( bSizer25, 1, wx.EXPAND, 5 )
		
		bSizer26 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_staticText1 = wx.StaticText( self, wx.ID_ANY, _(u"MyLabel"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText1.Wrap( -1 )
		bSizer26.Add( self.m_staticText1, 1, wx.ALL, 5 )
		
		
		bSizer23.Add( bSizer26, 0, 0, 5 )
		
		
		bSizer21.Add( bSizer23, 1, wx.EXPAND, 5 )
		
		bSizer24 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_textCtrl2 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer24.Add( self.m_textCtrl2, 1, wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer21.Add( bSizer24, 1, wx.EXPAND, 5 )
		
		
		bSizer20.Add( bSizer21, 1, wx.EXPAND, 5 )
		
		bSizer22 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_button9 = wx.Button( self, wx.ID_ANY, _(u"MyButton"), wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer22.Add( self.m_button9, 0, wx.ALIGN_BOTTOM|wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		
		bSizer20.Add( bSizer22, 0, wx.ALIGN_RIGHT, 5 )
		
		
		self.SetSizer( bSizer20 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.Bind( wx.EVT_INIT_DIALOG, self.OnInitDialog )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnInitDialog( self, event ):
		event.Skip()
	

###########################################################################
## Class DlgAudio
###########################################################################

class DlgAudio ( wx.Dialog ):
	
	def __init__( self, parent ):
		wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = _(u"Audio Options"), pos = wx.DefaultPosition, size = wx.Size( 553,381 ), style = wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		
		bSizer32 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_nbAudioOptions = wx.Notebook( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_panelDevices = wx.Panel( self.m_nbAudioOptions, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer34 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer10 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelDevices, wx.ID_ANY, _(u"PortAudio") ), wx.VERTICAL )
		
		bSizer41 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer37 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_radioBtnPortAudio = wx.RadioButton( self.m_panelDevices, wx.ID_ANY, _(u"PortAudio"), wx.DefaultPosition, wx.DefaultSize, wx.RB_GROUP )
		self.m_radioBtnPortAudio.SetValue( True ) 
		bSizer37.Add( self.m_radioBtnPortAudio, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		
		bSizer41.Add( bSizer37, 0, wx.EXPAND, 5 )
		
		gSizer7 = wx.GridSizer( 4, 1, 0, 0 )
		
		self.m_staticText14 = wx.StaticText( self.m_panelDevices, wx.ID_ANY, _(u"Audio in (mic) -> codec:"), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_RIGHT )
		self.m_staticText14.Wrap( -1 )
		gSizer7.Add( self.m_staticText14, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_staticText15 = wx.StaticText( self.m_panelDevices, wx.ID_ANY, _(u"codec -> Radio Tx: "), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_RIGHT )
		self.m_staticText15.Wrap( -1 )
		gSizer7.Add( self.m_staticText15, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_staticText16 = wx.StaticText( self.m_panelDevices, wx.ID_ANY, _(u"Radio Rx -> codec:"), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_RIGHT )
		self.m_staticText16.Wrap( -1 )
		gSizer7.Add( self.m_staticText16, 0, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_staticText17 = wx.StaticText( self.m_panelDevices, wx.ID_ANY, _(u"codec -> Audio out (spkr):"), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_RIGHT )
		self.m_staticText17.Wrap( -1 )
		gSizer7.Add( self.m_staticText17, 0, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		
		bSizer41.Add( gSizer7, 1, wx.EXPAND, 5 )
		
		gSizer5 = wx.GridSizer( 4, 1, 0, 0 )
		
		m_comboBox1Choices = []
		self.m_comboBox1 = wx.ComboBox( self.m_panelDevices, wx.ID_ANY, _(u"Combo!"), wx.DefaultPosition, wx.DefaultSize, m_comboBox1Choices, 0 )
		gSizer5.Add( self.m_comboBox1, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 5 )
		
		m_comboBox2Choices = []
		self.m_comboBox2 = wx.ComboBox( self.m_panelDevices, wx.ID_ANY, _(u"Combo!"), wx.DefaultPosition, wx.DefaultSize, m_comboBox2Choices, 0 )
		gSizer5.Add( self.m_comboBox2, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 5 )
		
		m_comboBox11Choices = []
		self.m_comboBox11 = wx.ComboBox( self.m_panelDevices, wx.ID_ANY, _(u"Combo!"), wx.DefaultPosition, wx.DefaultSize, m_comboBox11Choices, 0 )
		gSizer5.Add( self.m_comboBox11, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 5 )
		
		m_comboBox12Choices = []
		self.m_comboBox12 = wx.ComboBox( self.m_panelDevices, wx.ID_ANY, _(u"Combo!"), wx.DefaultPosition, wx.DefaultSize, m_comboBox12Choices, 0 )
		gSizer5.Add( self.m_comboBox12, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer41.Add( gSizer5, 1, wx.EXPAND, 5 )
		
		
		sbSizer10.Add( bSizer41, 1, wx.EXPAND, 5 )
		
		
		bSizer34.Add( sbSizer10, 1, wx.EXPAND, 5 )
		
		sbSizer11 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelDevices, wx.ID_ANY, _(u"File I/O") ), wx.HORIZONTAL )
		
		bSizer38 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_radioBtnFileOnly = wx.RadioButton( self.m_panelDevices, wx.ID_ANY, _(u"File Only"), wx.DefaultPosition, wx.DefaultSize, wx.RB_GROUP )
		bSizer38.Add( self.m_radioBtnFileOnly, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		
		sbSizer11.Add( bSizer38, 0, wx.EXPAND, 5 )
		
		bSizer45 = wx.BoxSizer( wx.VERTICAL )
		
		
		sbSizer11.Add( bSizer45, 1, wx.EXPAND, 5 )
		
		bSizer46 = wx.BoxSizer( wx.VERTICAL )
		
		
		sbSizer11.Add( bSizer46, 1, wx.EXPAND, 5 )
		
		
		bSizer34.Add( sbSizer11, 1, wx.EXPAND, 5 )
		
		
		self.m_panelDevices.SetSizer( bSizer34 )
		self.m_panelDevices.Layout()
		bSizer34.Fit( self.m_panelDevices )
		self.m_nbAudioOptions.AddPage( self.m_panelDevices, _(u"Devices"), True )
		self.m_panelSettings = wx.Panel( self.m_nbAudioOptions, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer27 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer271 = wx.BoxSizer( wx.HORIZONTAL )
		
		sbSizer6 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelSettings, wx.ID_ANY, _(u"Rx Input") ), wx.VERTICAL )
		
		bSizer31 = wx.BoxSizer( wx.VERTICAL )
		
		m_lbRxInputChoices = []
		self.m_lbRxInput = wx.ListBox( self.m_panelSettings, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_lbRxInputChoices, wx.LB_HSCROLL|wx.LB_SINGLE )
		bSizer31.Add( self.m_lbRxInput, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 2 )
		
		self.m_textRxInput = wx.TextCtrl( self.m_panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer31.Add( self.m_textRxInput, 0, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 2 )
		
		
		sbSizer6.Add( bSizer31, 1, wx.EXPAND, 6 )
		
		
		bSizer271.Add( sbSizer6, 1, wx.EXPAND, 5 )
		
		sbSizer7 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelSettings, wx.ID_ANY, _(u"Tx Output") ), wx.VERTICAL )
		
		bSizer281 = wx.BoxSizer( wx.VERTICAL )
		
		m_lbTxOutputChoices = []
		self.m_lbTxOutput = wx.ListBox( self.m_panelSettings, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_lbTxOutputChoices, wx.LB_HSCROLL|wx.LB_SINGLE )
		bSizer281.Add( self.m_lbTxOutput, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 2 )
		
		self.m_textCtrl15 = wx.TextCtrl( self.m_panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer281.Add( self.m_textCtrl15, 0, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 2 )
		
		
		sbSizer7.Add( bSizer281, 1, wx.EXPAND, 5 )
		
		
		bSizer271.Add( sbSizer7, 1, wx.EXPAND, 6 )
		
		
		bSizer27.Add( bSizer271, 1, wx.EXPAND, 5 )
		
		bSizer26 = wx.BoxSizer( wx.HORIZONTAL )
		
		sbSizer8 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelSettings, wx.ID_ANY, _(u"Voice Input") ), wx.VERTICAL )
		
		bSizer29 = wx.BoxSizer( wx.VERTICAL )
		
		m_lbVoiceInputChoices = []
		self.m_lbVoiceInput = wx.ListBox( self.m_panelSettings, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_lbVoiceInputChoices, wx.LB_HSCROLL|wx.LB_SINGLE )
		bSizer29.Add( self.m_lbVoiceInput, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 2 )
		
		self.m_textVoiceInput = wx.TextCtrl( self.m_panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer29.Add( self.m_textVoiceInput, 0, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 2 )
		
		
		sbSizer8.Add( bSizer29, 1, wx.EXPAND, 5 )
		
		
		bSizer26.Add( sbSizer8, 1, wx.EXPAND, 6 )
		
		sbSizer9 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelSettings, wx.ID_ANY, _(u"Voice Output") ), wx.VERTICAL )
		
		bSizer30 = wx.BoxSizer( wx.VERTICAL )
		
		m_lbVoiceOutputChoices = []
		self.m_lbVoiceOutput = wx.ListBox( self.m_panelSettings, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_lbVoiceOutputChoices, wx.LB_HSCROLL|wx.LB_SINGLE )
		bSizer30.Add( self.m_lbVoiceOutput, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 2 )
		
		self.m_textVoiceOutput = wx.TextCtrl( self.m_panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer30.Add( self.m_textVoiceOutput, 0, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 2 )
		
		
		sbSizer9.Add( bSizer30, 1, wx.EXPAND, 1 )
		
		
		bSizer26.Add( sbSizer9, 1, wx.EXPAND, 6 )
		
		
		bSizer27.Add( bSizer26, 1, wx.EXPAND, 1 )
		
		
		self.m_panelSettings.SetSizer( bSizer27 )
		self.m_panelSettings.Layout()
		bSizer27.Fit( self.m_panelSettings )
		self.m_nbAudioOptions.AddPage( self.m_panelSettings, _(u"Settings"), False )
		self.m_panelRightChannel = wx.Panel( self.m_nbAudioOptions, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer36 = wx.BoxSizer( wx.VERTICAL )
		
		
		self.m_panelRightChannel.SetSizer( bSizer36 )
		self.m_panelRightChannel.Layout()
		bSizer36.Fit( self.m_panelRightChannel )
		self.m_nbAudioOptions.AddPage( self.m_panelRightChannel, _(u"Right Channel"), False )
		
		bSizer32.Add( self.m_nbAudioOptions, 1, wx.EXPAND |wx.ALL, 1 )
		
		bSizer28 = wx.BoxSizer( wx.VERTICAL )
		
		m_sdbSizer3 = wx.StdDialogButtonSizer()
		self.m_sdbSizer3OK = wx.Button( self, wx.ID_OK )
		m_sdbSizer3.AddButton( self.m_sdbSizer3OK )
		self.m_sdbSizer3Apply = wx.Button( self, wx.ID_APPLY )
		m_sdbSizer3.AddButton( self.m_sdbSizer3Apply )
		self.m_sdbSizer3Cancel = wx.Button( self, wx.ID_CANCEL )
		m_sdbSizer3.AddButton( self.m_sdbSizer3Cancel )
		m_sdbSizer3.Realize();
		
		bSizer28.Add( m_sdbSizer3, 1, wx.EXPAND, 5 )
		
		
		bSizer32.Add( bSizer28, 0, wx.EXPAND, 5 )
		
		
		bSizer32.AddSpacer( ( 0, 15), 0, wx.EXPAND, 5 )
		
		
		self.SetSizer( bSizer32 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.Bind( wx.EVT_INIT_DIALOG, self.OnInitDialog )
		self.m_lbRxInput.Bind( wx.EVT_LISTBOX, self.OnRxInputSelect )
		self.m_lbTxOutput.Bind( wx.EVT_LISTBOX, self.OnTxOutputSelect )
		self.m_lbVoiceInput.Bind( wx.EVT_LISTBOX, self.OnVoiceInputSelect )
		self.m_lbVoiceOutput.Bind( wx.EVT_LISTBOX, self.OnVoiceOutputSelect )
		self.m_sdbSizer3Apply.Bind( wx.EVT_BUTTON, self.OnApply )
		self.m_sdbSizer3Cancel.Bind( wx.EVT_BUTTON, self.OnCancel )
		self.m_sdbSizer3OK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnInitDialog( self, event ):
		event.Skip()
	
	def OnRxInputSelect( self, event ):
		event.Skip()
	
	def OnTxOutputSelect( self, event ):
		event.Skip()
	
	def OnVoiceInputSelect( self, event ):
		event.Skip()
	
	def OnVoiceOutputSelect( self, event ):
		event.Skip()
	
	def OnApply( self, event ):
		event.Skip()
	
	def OnCancel( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

###########################################################################
## Class DlgOptions
###########################################################################

class DlgOptions ( wx.Dialog ):
	
	def __init__( self, parent ):
		wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = _(u"Program Options"), pos = wx.DefaultPosition, size = wx.Size( 332,364 ), style = wx.DEFAULT_DIALOG_STYLE )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		
		bSizer30 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer5 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, _(u"Config Options") ), wx.HORIZONTAL )
		
		gSizer2 = wx.GridSizer( 6, 2, 0, 0 )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, _(u"Option #1:"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		gSizer2.Add( self.m_staticText2, 1, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl3 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_textCtrl3, 1, wx.ALL, 5 )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, _(u"Option #2:"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		gSizer2.Add( self.m_staticText3, 1, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl4 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_textCtrl4, 1, wx.ALL, 5 )
		
		self.m_staticText4 = wx.StaticText( self, wx.ID_ANY, _(u"Option #3:"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		gSizer2.Add( self.m_staticText4, 1, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl5 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_textCtrl5, 1, wx.ALL, 5 )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, _(u"Option #4:"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		gSizer2.Add( self.m_staticText5, 1, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl6 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_textCtrl6, 1, wx.ALL, 5 )
		
		self.m_staticText6 = wx.StaticText( self, wx.ID_ANY, _(u"Option #5:"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		gSizer2.Add( self.m_staticText6, 1, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl7 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_textCtrl7, 1, wx.ALL, 5 )
		
		self.m_staticText7 = wx.StaticText( self, wx.ID_ANY, _(u"Option #6:"), wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText7.Wrap( -1 )
		gSizer2.Add( self.m_staticText7, 1, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl8 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_textCtrl8, 1, wx.ALL, 5 )
		
		
		sbSizer5.Add( gSizer2, 1, wx.EXPAND, 5 )
		
		
		bSizer30.Add( sbSizer5, 1, wx.EXPAND, 5 )
		
		m_sdbSizer4 = wx.StdDialogButtonSizer()
		self.m_sdbSizer4OK = wx.Button( self, wx.ID_OK )
		m_sdbSizer4.AddButton( self.m_sdbSizer4OK )
		self.m_sdbSizer4Cancel = wx.Button( self, wx.ID_CANCEL )
		m_sdbSizer4.AddButton( self.m_sdbSizer4Cancel )
		m_sdbSizer4.Realize();
		
		bSizer30.Add( m_sdbSizer4, 0, wx.ALIGN_RIGHT, 5 )
		
		
		self.SetSizer( bSizer30 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.Bind( wx.EVT_INIT_DIALOG, self.OnInitDialog )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnInitDialog( self, event ):
		event.Skip()
	

###########################################################################
## Class DlgComPorts
###########################################################################

class DlgComPorts ( wx.Dialog ):
	
	def __init__( self, parent ):
		wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = _(u"Select Com Port"), pos = wx.DefaultPosition, size = wx.Size( 399,226 ), style = wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		
		bSizer30 = wx.BoxSizer( wx.VERTICAL )
		
		gSizer3 = wx.GridSizer( 2, 2, 0, 0 )
		
		self.m_staticText8 = wx.StaticText( self, wx.ID_ANY, _(u"Available Ports:"), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_RIGHT )
		self.m_staticText8.Wrap( -1 )
		gSizer3.Add( self.m_staticText8, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_listCtrlPortSelect = wx.ListCtrl( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LC_ICON|wx.LC_SINGLE_SEL )
		gSizer3.Add( self.m_listCtrlPortSelect, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText9 = wx.StaticText( self, wx.ID_ANY, _(u"Use Port:"), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_RIGHT )
		self.m_staticText9.Wrap( -1 )
		gSizer3.Add( self.m_staticText9, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		self.m_textCtrl10 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer3.Add( self.m_textCtrl10, 1, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		
		bSizer30.Add( gSizer3, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 5 )
		
		m_sdbSizer3 = wx.StdDialogButtonSizer()
		self.m_sdbSizer3OK = wx.Button( self, wx.ID_OK )
		m_sdbSizer3.AddButton( self.m_sdbSizer3OK )
		self.m_sdbSizer3Cancel = wx.Button( self, wx.ID_CANCEL )
		m_sdbSizer3.AddButton( self.m_sdbSizer3Cancel )
		m_sdbSizer3.Realize();
		
		bSizer30.Add( m_sdbSizer3, 0, wx.EXPAND, 5 )
		
		
		self.SetSizer( bSizer30 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_listCtrlPortSelect.Bind( wx.EVT_LIST_ITEM_SELECTED, self.OnListItemSelected )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnListItemSelected( self, event ):
		event.Skip()
	

