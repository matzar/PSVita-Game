set FBX2SCN_EXE=..\..\gef_abertay\tools\fbx2scn\build\vs2015\x64\Release\fbx2scn.exe
set OUTPUT_DIR=..\media
 
rem Repeat the following lines if you have more than one asset
 
rem change asset_name to the name of you FBX file without the .FBX extension
rem set ASSET_NAME=apple
 
rem set FBX2SCN_FLAGS=ignore-skinning -scaling-factor 10.0 -strip-texture-path -texture-extension .png
rem %FBX2SCN_EXE% -o %OUTPUT_DIR%\%ASSET_NAME%.scn %FBX2SCN_FLAGS% %ASSET_NAME%.fbx
 

rem remove the lines below if you only have one asset
rem change asset2_name to the name of you FBX file without the .FBX extension
rem set ASSET_NAME=alienFlagship2
 
rem set FBX2SCN_FLAGS=-ignore-skinning -scaling-factor 1.0 -strip-texture-path -texture-extension .png
rem %FBX2SCN_EXE% -o %OUTPUT_DIR%\%ASSET_NAME%.scn %FBX2SCN_FLAGS% %ASSET_NAME%.fbx

rem remove the lines below if you only have one asset
rem change asset2_name to the name of you FBX file without the .FBX extension
rem set ASSET_NAME=rock
 
rem set FBX2SCN_FLAGS=-ignore-skinning -scaling-factor 0.1 -strip-texture-path -texture-extension .png
rem %FBX2SCN_EXE% -o %OUTPUT_DIR%\%ASSET_NAME%.scn %FBX2SCN_FLAGS% %ASSET_NAME%.fbx

rem set ASSET_NAME=triceratop
rem set FBX2SCN_FLAGS=-axis-preset mayaZup -scaling-factor 0.002 -strip-texture-path -texture-extension .png
rem %FBX2SCN_EXE% -o %OUTPUT_DIR%\%ASSET_NAME%.scn %FBX2SCN_FLAGS% %ASSET_NAME%.fbx

pause