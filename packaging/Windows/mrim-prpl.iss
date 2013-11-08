[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{28D30FEA-45C4-4EE6-BC6C-ECDB7156AD98}
AppName=mrim-prpl
AppVersion=0.2.0 (git: 11ecd12dfa2b53a342789e7361a9a045c0493228)
AppPublisherURL=http://code.google.com/p/mrim-prp
AppSupportURL=http://code.google.com/p/mrim-prp
AppUpdatesURL=http://code.google.com/p/mrim-prpl/downloads/list
DefaultDirName="{code:GetPidginPath}"
DefaultGroupName=mrim-prpl
DisableProgramGroupPage=yes
LicenseFile=..\..\LICENSE
OutputDir=Output
OutputBaseFilename=mrim-prpl-0.2.0
Compression=lzma
SolidCompression=yes


[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Files]
Source: "..\..\src\mrim-prpl.dll"; DestDir: "{app}\plugins"; Flags:
Source: "..\..\pixmaps\mrim16.png"; DestDir: "{app}\pixmaps\pidgin\protocols\16"; DestName: mrim.png; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\..\pixmaps\mrim22.png"; DestDir: "{app}\pixmaps\pidgin\protocols\22"; DestName: mrim.png; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\..\pixmaps\mrim48.png"; DestDir: "{app}\pixmaps\pidgin\protocols\48"; DestName: mrim.png; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files


[Code]
function GetPidginPath(Param: String): String;
var
  Path: String;
begin
  if RegQueryStringValue(HKLM, 'SOFTWARE\Pidgin','', Path) then 
    Result:=Path
  else
      if RegQueryStringValue(HKEY_CURRENT_USER, 'Software\pidgin','', Path) then
           Result:=Path
      else
          begin
              MsgBox('Can not find Pidgin installation path. Select path manualy', mbInformation, MB_OK);
              Result:='{pf}\Pidgin';
          end;
end;
