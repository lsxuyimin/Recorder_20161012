; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBgDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "recorder.h"
LastPage=0

ClassCount=27
Class1=CBgDlg
Class2=CCheckCardDlg
Class3=CDlgBarEx
Class4=CDlgBarExBottomAcq
Class5=CDlgBarExBottomPlay
Class6=CMainFrame
Class7=CMarkDlg
Class8=CMeasureAddDlg
Class9=CMeasureDlg
Class10=CMeasureEditDlg
Class11=CMeasureItem
Class12=CPopupRightDlg
Class13=CRecorderApp
Class14=CAboutDlg
Class15=CRecorderDoc
Class16=CRecorderView
Class17=CWatchDlg
Class18=CWaveData

ResourceCount=24
Resource1=IDD_DIALOG_StoragePath
Resource2=IDD_DIALOG_CheckCard
Resource3=IDD_DIALOG_MeasureAdd
Resource4=IDD_RECORDER_FORM
Resource5=IDD_DIALOG_BarEx
Resource6=IDD_ABOUTBOX
Resource7=IDD_DIALOG_Measure
Resource8=IDD_DIALOG_BarExBottomAcq
Resource9=IDD_DIALOG_BarExTopChnSel
Resource10=IDD_DIALOG_Zoom
Resource11=IDD_DIALOG_MeasureEdit
Resource12=IDD_DIALOG_ChSel
Resource13=IDR_MENU_WatchDlgPopup
Resource14=IDD_DIALOG_Save
Resource15=IDR_MENU_MeaDlgPopup
Resource16=IDD_DIALOG_SaveAs
Class19=CZoomDlg
Resource17=IDD_DIALOG_PopupRight
Class20=CStoragePathDlg
Resource18=IDD_DIALOG_BGDLG
Class21=CSaveDlg
Resource19=IDR_MAINFRAME
Class22=CSaveAsDlg
Resource20=IDD_DIALOG_WATCHDLG
Class23=CPopupPropertyDlg
Resource21=IDD_DIALOG_Mark
Class24=CChSelDlg
Resource22=IDD_DIALOG_BarExBottomPlay
Class25=CDlgBarExTopChnSel
Class26=CComboBoxRange
Resource23=IDD_DIALOG_PopupProperty
Class27=CFftConfDlg
Resource24=IDD_DIALOG_FftConf

[CLS:CBgDlg]
Type=0
BaseClass=CDialog
HeaderFile=BgDlg.h
ImplementationFile=BgDlg.cpp
LastObject=ID_WATCH_Setting

[CLS:CCheckCardDlg]
Type=0
BaseClass=CDialog
HeaderFile=CheckCardDlg.h
ImplementationFile=CheckCardDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CCheckCardDlg

[CLS:CDlgBarEx]
Type=0
BaseClass=CDialogBar
HeaderFile=DlgBarEx.h
ImplementationFile=DlgBarEx.cpp
Filter=D
VirtualFilter=dWC
LastObject=ID_WATCH_AddOsc

[CLS:CDlgBarExBottomAcq]
Type=0
BaseClass=CDialogBar
HeaderFile=DlgBarExBottomAcq.h
ImplementationFile=DlgBarExBottomAcq.cpp

[CLS:CDlgBarExBottomPlay]
Type=0
BaseClass=CDialogBar
HeaderFile=DlgBarExBottomPlay.h
ImplementationFile=DlgBarExBottomPlay.cpp
LastObject=IDC_RADIO_Pause
Filter=D
VirtualFilter=dWC

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=ID_WATCH_TimeDomain
Filter=T
VirtualFilter=fWC

[CLS:CMarkDlg]
Type=0
BaseClass=CDialog
HeaderFile=MarkDlg.h
ImplementationFile=MarkDlg.cpp

[CLS:CMeasureAddDlg]
Type=0
BaseClass=CDialog
HeaderFile=MeasureAddDlg.h
ImplementationFile=MeasureAddDlg.cpp

[CLS:CMeasureDlg]
Type=0
BaseClass=CDialog
HeaderFile=MeasureDlg.h
ImplementationFile=MeasureDlg.cpp

[CLS:CMeasureEditDlg]
Type=0
BaseClass=CDialog
HeaderFile=MeasureEditDlg.h
ImplementationFile=MeasureEditDlg.cpp

[CLS:CMeasureItem]
Type=0
BaseClass=CCmdTarget
HeaderFile=MeasureItem.h
ImplementationFile=MeasureItem.cpp
LastObject=CMeasureItem

[CLS:CPopupRightDlg]
Type=0
BaseClass=CDialog
HeaderFile=PopupRightDlg.h
ImplementationFile=PopupRightDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CPopupRightDlg

[CLS:CRecorderApp]
Type=0
BaseClass=CWinApp
HeaderFile=Recorder.h
ImplementationFile=Recorder.cpp
LastObject=CRecorderApp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Recorder.cpp
ImplementationFile=Recorder.cpp
LastObject=CAboutDlg

[CLS:CRecorderDoc]
Type=0
BaseClass=CDocument
HeaderFile=RecorderDoc.h
ImplementationFile=RecorderDoc.cpp

[CLS:CRecorderView]
Type=0
BaseClass=CFormView
HeaderFile=RecorderView.h
ImplementationFile=RecorderView.cpp

[CLS:CWatchDlg]
Type=0
BaseClass=CDialog
HeaderFile=WatchDlg.h
ImplementationFile=WatchDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=ID_WATCH_AddFreDomain

[CLS:CWaveData]
Type=0
BaseClass=CCmdTarget
HeaderFile=WaveData.h
ImplementationFile=WaveData.cpp

[DLG:IDD_DIALOG_BGDLG]
Type=1
Class=CBgDlg
ControlCount=1
Control1=IDC_TAB_BgDlg,SysTabControl32,1342177280

[DLG:IDD_DIALOG_CheckCard]
Type=1
Class=CCheckCardDlg
ControlCount=11
Control1=IDC_RADIO_Demo,button,1342308361
Control2=IDC_RADIO_PhyCard,button,1342177289
Control3=IDC_RADIO_OpenExistData,button,1342177289
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1073807360
Control6=IDC_LIST_PhyCard,listbox,1352728835
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_DemoChn,combobox,1344340226
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_ExistDataPath,edit,1350631552
Control11=IDC_BUTTON_Browse,button,1342242816

[DLG:IDD_DIALOG_BarEx]
Type=1
Class=CDlgBarEx
ControlCount=21
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_COMBO_RATE,combobox,1344339971
Control4=IDC_COMBO_LENGTH,combobox,1344339971
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_TRIGGER_CHANNEL,combobox,1344339971
Control9=IDC_STATIC,static,1208090624
Control10=IDC_SLIDER_TRIGGER_LEVEL,msctls_trackbar32,1208025112
Control11=IDC_STATIC,static,1208090624
Control12=IDC_COMBO_RANGE,combobox,1210122243
Control13=IDC_STATIC,static,1342308352
Control14=IDC_COMBO_TRIGGER_MODE,combobox,1344339971
Control15=IDC_STATIC_TRIGGER_LEVEL,static,1208090624
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_SLIDER_TRIGGER_UpperLimit,msctls_trackbar32,1342242840
Control19=IDC_SLIDER_TRIGGER_LowerLimit,msctls_trackbar32,1342242840
Control20=IDC_STATIC_TRIGGER_UpperLimit,static,1342308352
Control21=IDC_STATIC_TRIGGER_LowerLimit,static,1342308352

[DLG:IDD_DIALOG_BarExBottomAcq]
Type=1
Class=CDlgBarExBottomAcq
ControlCount=10
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_COMBO_RATE,combobox,1344339971
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SLIDER_TRIGGER_LEVEL,msctls_trackbar32,1342242840
Control7=IDC_BUTTON1,button,1342242816
Control8=IDC_BUTTON2,button,1342242816
Control9=IDC_BUTTON3,button,1342242816
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_BarExBottomPlay]
Type=1
Class=CDlgBarExBottomPlay
ControlCount=7
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC_AcqStatus,static,1342308352
Control4=IDC_RADIO_Play,button,1342312585
Control5=IDC_RADIO_Pause,button,1342181513
Control6=IDC_BUTTON_SpeedUp,button,1342242944
Control7=IDC_BUTTON_SpeedDown,button,1342242944

[DLG:IDD_DIALOG_Mark]
Type=1
Class=CMarkDlg
ControlCount=3
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,static,1073872896

[DLG:IDD_DIALOG_MeasureAdd]
Type=1
Class=CMeasureAddDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342181383
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMBO_MeaChannel,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO_MeaType,combobox,1344339971
Control8=IDC_STATIC,static,1342308352
Control9=IDC_COMBO_MeaPart,combobox,1344339971

[DLG:IDD_DIALOG_Measure]
Type=1
Class=CMeasureDlg
ControlCount=3
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC_TitleBar,static,1073741829

[DLG:IDD_DIALOG_MeasureEdit]
Type=1
Class=CMeasureEditDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342181383
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMBO_MeaChannel,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO_MeaType,combobox,1344339971
Control8=IDC_STATIC,static,1342308352
Control9=IDC_COMBO_MeaPart,combobox,1344339971

[DLG:IDD_DIALOG_PopupRight]
Type=1
Class=CPopupRightDlg
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_COMBO_WavefForm,combobox,1344339971
Control7=IDC_COMBO_Frequency,combobox,1344339971
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_AmpMax,edit,1350631552
Control12=IDC_EDIT_AmpMin,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_COMBO_ChnIndex,combobox,1344339971

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDOK,button,1342373889
Control3=IDC_STATIC,static,1342308480
Control4=IDC_STATIC,static,1342308480

[DLG:IDD_RECORDER_FORM]
Type=1
Class=CRecorderView
ControlCount=1
Control1=IDC_STATIC_InitialMark,static,1073872896

[DLG:IDD_DIALOG_WATCHDLG]
Type=1
Class=CWatchDlg
ControlCount=4
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC_Title,static,1073741824
Control4=IDC_STATIC_CursorRead,static,1073741824

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_APP_CONNECT
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_Costum
Command4=ID_FILE_SAVE_AS_Costum
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_CopyText
Command11=ID_EDIT_CopySelectedImage
Command12=ID_MENUITEM32774
Command13=ID_EDIT_UNDO
Command14=ID_CTRL_StartAcq
Command15=ID_CTRL_StopAcq
Command16=ID_WATCH_AddOsc
Command17=ID_WATCH_AddFreDomain
Command18=ID_WATCH_RenameWatchDlg
Command19=ID_WATCH_CloseWatchDlg
Command20=ID_LAYOUT_Mode1
Command21=ID_LAYOUT_Mode2
Command22=ID_LAYOUT_Mode3
Command23=ID_LAYOUT_Mode4
Command24=ID_WATCH_NextPage
Command25=ID_WATCH_PrePage
Command26=ID_WATCH_None
Command27=ID_WATCH_ZoomIn
Command28=ID_WATCH_ZoomOut
Command29=ID_WATCH_ZoomReStore
Command30=ID_WATCH_Setting
Command31=ID_MEA_ADDMEASURE
Command32=ID_MEA_EDITMEASURE
Command33=ID_MEA_DELMEASURE
Command34=ID_VIEW_TOOLBAR
Command35=ID_VIEW_STATUS_BAR
Command36=ID_APP_Help
Command37=ID_APP_ABOUT
CommandCount=37

[MNU:IDR_MENU_WatchDlgPopup]
Type=1
Class=?
Command1=ID_WATCH_AddOsc
Command2=ID_WATCH_AddFreDomain
Command3=ID_WATCH_RenameWatchDlg
Command4=ID_WATCH_CloseWatchDlg
Command5=ID_WATCH_ChSel
Command6=ID_LAYOUT_Mode1
Command7=ID_LAYOUT_Mode2
Command8=ID_LAYOUT_Mode3
Command9=ID_LAYOUT_Mode4
Command10=ID_WATCH_Property
Command11=ID_MEA_ADDMEASURE
CommandCount=11

[MNU:IDR_MENU_MeaDlgPopup]
Type=1
Class=?
Command1=ID_MEA_ADDMEASURE
Command2=ID_MEA_EDITMEASURE
Command3=ID_MEA_DELMEASURE
CommandCount=3

[DLG:IDD_DIALOG_Zoom]
Type=1
Class=CZoomDlg
ControlCount=3
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,static,1073872896

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE_Costum
Command3=ID_APP_ABOUT
Command4=ID_WATCH_TimeDomain
Command5=ID_WATCH_FreqDomain
Command6=ID_WATCH_FreqDomainConf
Command7=ID_CTRL_StartAcq
Command8=ID_CTRL_StopAcq
Command9=ID_LAYOUT_Mode1
Command10=ID_LAYOUT_Mode2
Command11=ID_LAYOUT_Mode3
Command12=ID_LAYOUT_Mode4
Command13=ID_WATCH_PrePage
Command14=ID_WATCH_NextPage
Command15=ID_WATCH_Setting
Command16=ID_WATCH_ZoomIn
Command17=ID_WATCH_ZoomOut
Command18=ID_WATCH_None
Command19=ID_WATCH_ZoomReStore
CommandCount=19

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[CLS:CZoomDlg]
Type=0
HeaderFile=ZoomDlg.h
ImplementationFile=ZoomDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CZoomDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_StoragePath]
Type=1
Class=CStoragePathDlg
ControlCount=6
Control1=IDC_RADIO_OverrideDoc,button,1342308361
Control2=IDC_RADIO_NewDoc,button,1342177289
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1073807360
Control5=IDC_EDIT_NewPath,edit,1350631552
Control6=IDC_STATIC_WorkPlace,static,1342308352

[CLS:CStoragePathDlg]
Type=0
HeaderFile=StoragePathDlg.h
ImplementationFile=StoragePathDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CStoragePathDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_Save]
Type=1
Class=CSaveDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_WorkPlace,static,1342308352
Control4=IDC_EDIT_SavePath,edit,1350631552
Control5=IDC_STATIC,static,1342308352

[CLS:CSaveDlg]
Type=0
HeaderFile=SaveDlg.h
ImplementationFile=SaveDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_FILE_SAVE
VirtualFilter=dWC

[DLG:IDD_DIALOG_SaveAs]
Type=1
Class=CSaveAsDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_WorkPlace,static,1342308352
Control4=IDC_EDIT_SavePath,edit,1350631552
Control5=IDC_STATIC,static,1342308352

[CLS:CSaveAsDlg]
Type=0
HeaderFile=SaveAsDlg.h
ImplementationFile=SaveAsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSaveAsDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_PopupProperty]
Type=1
Class=CPopupPropertyDlg
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC_Source,static,1342308352
Control10=IDC_STATIC_SamplingRate,static,1342308352
Control11=IDC_STATIC_SamplingLen,static,1342308352
Control12=IDC_STATIC_SamplingTime,static,1342308352
Control13=IDC_STATIC_Range,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC_ChNum,static,1342308352

[CLS:CPopupPropertyDlg]
Type=0
HeaderFile=PopupPropertyDlg.h
ImplementationFile=PopupPropertyDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPopupPropertyDlg
VirtualFilter=dWC

[CLS:CChSelDlg]
Type=0
HeaderFile=ChSelDlg.h
ImplementationFile=ChSelDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST_CH
VirtualFilter=dWC

[DLG:IDD_DIALOG_ChSel]
Type=1
Class=CChSelDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_CH,SysListView32,1350631425
Control4=IDC_STATIC_Tip,static,1342308352

[DLG:IDD_DIALOG_BarExTopChnSel]
Type=1
Class=CDlgBarExTopChnSel
ControlCount=8
Control1=IDC_STATIC_ChnSel1,static,1342308352
Control2=IDC_COMBO_ChnRange1,combobox,1344339971
Control3=IDC_STATIC_ChnSel2,static,1342308352
Control4=IDC_COMBO_ChnRange2,combobox,1344339971
Control5=IDC_STATIC_ChnSel3,static,1342308352
Control6=IDC_COMBO_ChnRange3,combobox,1344339971
Control7=IDC_STATIC_ChnSel4,static,1342308352
Control8=IDC_COMBO_ChnRange4,combobox,1344339971

[CLS:CDlgBarExTopChnSel]
Type=0
HeaderFile=DlgBarExTopChnSel.h
ImplementationFile=DlgBarExTopChnSel.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDC_COMBO_ChnRange1
VirtualFilter=dWC

[CLS:CComboBoxRange]
Type=0
HeaderFile=ComboBoxRange.h
ImplementationFile=ComboBoxRange.cpp
BaseClass=CComboBox
Filter=W
VirtualFilter=cWC
LastObject=CComboBoxRange

[DLG:IDD_DIALOG_FftConf]
Type=1
Class=CFftConfDlg
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO_FftBins,combobox,1344339971
Control8=IDC_COMBO_WindowMode,combobox,1344339971
Control9=IDC_COMBO_LogUnit,combobox,1344339971
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_Impedance,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_RADIO_Logarithm,button,1342308361
Control14=IDC_RADIO_Linear,button,1342177289
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC_Tips,static,1342308352

[CLS:CFftConfDlg]
Type=0
HeaderFile=FftConfDlg.h
ImplementationFile=FftConfDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO_LogUnit
VirtualFilter=dWC

