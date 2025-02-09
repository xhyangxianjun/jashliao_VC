Attribute VB_Name = "Module1"
Option Explicit

Type NETRESOURCE
        dwScope As Long
        dwType As Long
        dwDisplayType As Long
        dwUsage As Long
        lpLocalName As String
        lpRemoteName As String
        lpComment As String
        lpProvider As String
End Type
Declare Function WNetAddConnection2 Lib "mpr.dll" Alias "WNetAddConnection2A" (lpNetResource As NETRESOURCE, ByVal lpPassword As String, ByVal lpUserName As String, ByVal dwFlags As Long) As Long
Declare Function WNetCancelConnection2 Lib "mpr.dll" Alias "WNetCancelConnection2A" (ByVal lpName As String, ByVal dwFlags As Long, ByVal fForce As Long) As Long
Declare Function WNetGetLastError Lib "mpr.dll" Alias "WNetGetLastErrorA" (lpError As Long, ByVal lpErrorBuf As String, ByVal nErrorBufSize As Long, ByVal lpNameBuf As String, ByVal nNameBufSize As Long) As Long

Public Const RESOURCE_PUBLICNET = &H2
Public Const RESOURCETYPE_ANY = &H0
Public Const RESOURCEDISPLAYTYPE_GENERIC = &H0
Public Const RESOURCEUSAGE_CONNECTABLE = &H1
Public Const CONNECT_UPDATE_PROFILE = &H1
Public Function AddConnection(ByVal RemoteLocation As String, ByVal LocalDriver As String, _
                              ByVal Passwd As String, ByVal UserName As String) As Boolean
Dim ne As NETRESOURCE, i As Long
Dim errstr As String, errpriv As String, erno As Long

ne.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC
ne.dwScope = RESOURCE_PUBLICNET
ne.dwType = RESOURCETYPE_ANY
ne.dwUsage = RESOURCEUSAGE_CONNECTABLE
ne.lpComment = vbNullString
ne.lpLocalName = LocalDriver
ne.lpProvider = vbNullString
ne.lpRemoteName = RemoteLocation

i = WNetAddConnection2(ne, Passwd, UserName, 0)
If i = 0 Then
   AddConnection = True
Else
  AddConnection = False
  errstr = String(256, 0)
  errpriv = String(256, 0)
  i = WNetGetLastError(erno, errstr, 256, errpriv, 256)
  errstr = Left(errstr, InStr(1, errstr, Chr(0)) - 1)
  MsgBox errstr, vbCritical
End If
End Function

Public Function CancelConnection(ByVal LocalDriver As String, ByVal ForceClose As Boolean) As Boolean
Dim i As Long
Dim errstr As String, errpriv As String, erno As Long

i = WNetCancelConnection2(LocalDriver, 0, IIf(ForceClose, 1, 0))
If i = 0 Then
   CancelConnection = True
Else
  CancelConnection = False
  errstr = String(256, 0)
  errpriv = String(256, 0)
  i = WNetGetLastError(erno, errstr, 256, errpriv, 256)
  errstr = Left(errstr, InStr(1, errstr, Chr(0)) - 1)
  MsgBox errstr, vbCritical
End If

End Function


