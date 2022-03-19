# PTrain

This is the main source repository for the MVFT pilot training simulator. 

**Hover Controls**
- Space / Gamepad Left Stick UP: Power
- Up-Down Arrows / Gamepad Right Stick Up-Down: Forward/Back
- Left-Right Arrows / Gamepad Right Stick LR: Left/Right
- A-D / Gamepad Left Stick LR: Yaw

**Glide Controls**
- TBD

**Other Controls**
- Enter: Toggle Glide and Hover
- P: Reset plane
- L: Toggle HMD

## Setup
To configure a CAEN machine to use the simulator in-editor, follow these instructions:
1. Use AppsAnywhere to install Visual Studio and Unreal Engine 4.26 (NOT 4.23!)
	- To install `git bash`, install AirSim through AppsAnywhere. I haven't found a way to install git directly.
2. Download this repository as a zip, or to get it as a repository, `git clone --depth=1` (to avoid downloading the entire history) __into `C:\TEMP`__. This is very important, because the temp folder is not networked. If you try to open the project from your home folder, it will probably not work.
3. Run `openall.bat` in the repository root folder. This will launch SteamVR and Unreal. If there is an issue, you can open them manually by following these instructions:
    1. Launch SteamVR by going to `C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64` and opening `vrstartup.exe`. SteamVR may take you through the room configuration. Follow the on-screen prompts for room-size VR. 
    2. Open the PTrainer repository folder in Explorer and right-click `PTrain.uproject`, then select Open With, select Choose an App from this PC, then navigate to `C:\Program Files\Epic Games\UE_4.26\Engine\Binaries\Win64\UE4_Editor.exe`
6. On first-time launch, Unreal should ask to compile content, press Yes, then that should complete and the editor window should appear.
    - Wait for shaders to finish compiling for visuals to be correct
    - It will take around 20 minutes for Unreal to first-time launch on a CAEN computer, so if it looks stuck, it's not, just give it some time (subsequent launches will be much faster)
7. To run in VR, select the dropdown menu next to Play, and select VR Preview. It will launch a separate window. To exit, press the escape key or alt-tab to the Unreal window.
    
To configure on a personal computer:
1. Install prerequisites for your platform:
    - All: SteamVR (for VR support, note SteamVR does not support macOS)
    - macOS: Xcode
    - Windows: Visual Studio with Desktop Development for C++ module
2. Install Unreal Engine. The installer should set `.uproject` file associations for you.
3. Download this repository as zip, or clone using `git clone --depth=1`
4. Open `PTrain.uproject`. When prompted to compile content, press Yes, and wait for that to complete. After that, the Editor window should appear. It should auto-launch SteamVR. If it does not, you must close Unreal, launch SteamVR manually, and then reopen Unreal. 

## Writing C++ code
1. Right-click `PTrain.uproject` and select Generate Xcode/Visual Studio Project
    - If this option is not available, open `PTrain.uproject` and go to File->Generate Xcode/Visual Studio Project
    - Afterwards, close the editor
2. Open the generated IDE project file and press the Start Debugging (Visual Studio) or Run (Xcode) button
    - This will launch the Editor with the debugger attached so C++ source debugging works
    - You can also use Attach To Process on an already-running Editor from your IDE to debug from there without restarting it

## Usage In Editor
- The `Arena` Level file contains the simulated environment. Simply press Play to start the simulation

## Generating Builds
For CAEN
- Run `build.bat`.The built program will be written to the parent directory (not in the repository folder) named WindowsNoEditor

For other platforms
- Go to File->Package Project, and select the desired target. 
