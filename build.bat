"C:\Program Files\Epic Games\UE_4.26\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%cd%\PTrain.uproject" -clientconfig=Shipping -cook -allmaps -build -stage -pak -archive -archivedirectory="%cd%\.."
explorer.exe "%cd%\..\WindowsNoEditor"