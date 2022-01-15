﻿; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "ScreenAutoRotate"
#define MyAppVersion "2.0.0"
#define MyAppPublisher "J0B10"
#define MyAppURL "https://github.com/J0B10/ScreenAutoRotate"
#define MyAppExeName "launch.bat"
#define MyAppAssocName MyAppName + " File"
#define MyAppAssocExt ".myp"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt
#define MyAppProjectLoc "D:\VSProjects\ScreenAutoRotate"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B79800A8-78BE-435B-94C7-11651AEE29DD}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\J0B10\ScreenAutoRotate
ChangesAssociations=yes
DisableProgramGroupPage=yes
LicenseFile={#MyAppProjectLoc}\LICENSE
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
;PrivilegesRequiredOverridesAllowed=dialog
OutputDir={#MyAppProjectLoc}\win_installer\out
OutputBaseFilename=mysetup
SetupIconFile={#MyAppProjectLoc}\win_installer\icon.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Tasks]
Name: StartMenuEntry; Description: "Start {#MyAppName} when windows starts"
; TODO: Implement adding to PATH
;Name: envPath; Description: "Add to PATH variable" 

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#MyAppProjectLoc}\win_installer\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppProjectLoc}\win_installer\icon.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppProjectLoc}\tray\target\ScreenAutoRotate.jar"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppProjectLoc}\cli\out\build\x86-Release\ScreenAutoRotate.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#MyAppProjectLoc}\README.md"; DestDir: "{app}"; Flags: isreadme
Source: "{#MyAppProjectLoc}\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: ".myp"; ValueData: ""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\icon.ico";
Name: "{userstartup}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\icon.ico"; Tasks:StartMenuEntry;

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: shellexec postinstall skipifsilent
