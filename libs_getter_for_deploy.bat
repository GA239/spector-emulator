@echo Off
set windeployqtdir=%1
set ISSOdir=%2

cd ..
mkdir deploy
set deployir=%CD%\deploy

mkdir deploy\src
set deploy_src_dir=%CD%\deploy\src

xcopy build-spector-emulator\src\spector-emulator-app\release\spector-emulator.exe %deploy_src_dir%

xcopy build-spector-emulator\src\spector-emulator-app\test_gas-spector_1.gscfg %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-app\test_gas-spector_2.gscfg %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-app\test_gas-spector_3.gscfg %deploy_src_dir%

xcopy build-spector-emulator\src\spector-emulator-app\gsData_test_1.txt %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-app\gsData_test_2.txt %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-app\gsData_test_3.txt %deploy_src_dir%

xcopy build-spector-emulator\src\spector-emulator-controll-widget\release\ControllSEWidget.dll %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-data-generator\release\DataGeneratorLib.dll %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-peak-detector\release\PeakDetectorLib.dll %deploy_src_dir%
xcopy build-spector-emulator\src\spector-emulator-plot-widget\release\PlotSEWidget.dll %deploy_src_dir%

xcopy %windeployqtdir%\libstdc++-6.dll %deploy_src_dir%
xcopy %windeployqtdir%\libgcc_s_dw2-1.dll %deploy_src_dir%
xcopy %windeployqtdir%\libwinpthread-1.dll %deploy_src_dir%

xcopy spector-emulator\libs\libqt-searchwidget\release\searchwidget.dll %deploy_src_dir%

xcopy spector-emulator\deploy\install.ico %deployir%

cd %windeployqtdir%
windeployqt.exe --release %deploy_src_dir%\ControllSEWidget.dll
windeployqt.exe --release %deploy_src_dir%\PlotSEWidget.dll
windeployqt.exe --release %deploy_src_dir%\PeakDetectorLib.dll
windeployqt.exe --release %deploy_src_dir%\DataGeneratorLib.dll
windeployqt.exe --release %deploy_src_dir%\spector-emulator.exe

cd %deployir%

ECHO #define MyAppName "Spector Emulator" >> script.iss
ECHO #define MyAppVersion "1.0" >> script.iss
ECHO #define MyAppPublisher "Gavrilov A.O." >> script.iss
ECHO #define MyAppExeName "spector-emulator.exe" >> script.iss
ECHO [Setup] >> script.iss
ECHO ; NOTE: The value of AppId uniquely identifies this application. >> script.iss
ECHO ; Do not use the same AppId value in installers for other applications. >> script.iss
ECHO AppId={{024325A0-B6D4-40DA-8947-30BBD89CE829} >> script.iss
ECHO AppName={#MyAppName} >> script.iss
ECHO AppVersion={#MyAppVersion} >> script.iss
ECHO ;AppVerName={#MyAppName} {#MyAppVersion} >> script.iss
ECHO AppPublisher={#MyAppPublisher} >> script.iss
ECHO DefaultDirName={pf}\{#MyAppName} >> script.iss
ECHO DisableProgramGroupPage=yes >> script.iss
ECHO OutputBaseFilename=spector-emulator-setup >> script.iss
ECHO SetupIconFile=%deployir%\install.ico >> script.iss
ECHO Compression=lzma >> script.iss
ECHO SolidCompression=yes >> script.iss
ECHO ; "ArchitecturesAllowed=x64" specifies that Setup cannot run on >> script.iss
ECHO ; anything but x64. >> script.iss
ECHO ;ArchitecturesAllowed=x64 >> script.iss
ECHO ; "ArchitecturesInstallIn64BitMode=x64" requests that the install be >> script.iss
ECHO ; done in "64-bit mode" on x64, meaning it should use the native >> script.iss
ECHO ; 64-bit Program Files directory and the 64-bit view of the registry. >> script.iss
ECHO ArchitecturesInstallIn64BitMode=x64 >> script.iss
ECHO [Languages] >> script.iss
ECHO Name: "english"; MessagesFile: "compiler:Default.isl" >> script.iss
ECHO Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl" >> script.iss
ECHO [Tasks] >> script.iss
ECHO Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked >> script.iss
ECHO Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1 >> script.iss
ECHO [Files] >> script.iss
ECHO Source: "%deploy_src_dir%\spector-emulator.exe"; DestDir: "{app}"; Flags: ignoreversion >> script.iss
ECHO Source: "%deploy_src_dir%\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs >> script.iss
ECHO ; NOTE: Don't use "Flags: ignoreversion" on any shared system files >> script.iss
ECHO [Icons] >> script.iss
ECHO Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}" >> script.iss
ECHO Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon >> script.iss
ECHO Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon >> script.iss
ECHO [Run] >> script.iss
ECHO Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent >> script.iss

%ISSOdir%\ISCC.exe  %deployir%\script.iss

cd %~dp0