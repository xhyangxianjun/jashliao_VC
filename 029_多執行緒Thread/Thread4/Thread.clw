; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CThreadDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Thread.h"

ClassCount=4
Class1=CThreadApp
Class2=CThreadDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_THREAD_DIALOG

[CLS:CThreadApp]
Type=0
HeaderFile=Thread.h
ImplementationFile=Thread.cpp
Filter=N

[CLS:CThreadDlg]
Type=0
HeaderFile=ThreadDlg.h
ImplementationFile=ThreadDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ThreadDlg.h
ImplementationFile=ThreadDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_THREAD_DIALOG]
Type=1
Class=CThreadDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816

