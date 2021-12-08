# COMP-8551 Game Engine Project - Edgar the X-Terminator

## Description
In Edgar the X-Terminator, you play as Edgar, a bug exterminator. Collect stars for points as you fight against a hoard of angry cockroaches.

## Running the Game
From the latest release, download the build from the latest [release](https://github.com/JasonZFHuang/COMP-8551-Project/releases). Then unzip and run **COMP-8551-Project.exe**.

## Building the game
If you prefer to create a build yourself, open Visual Studio. 

At the top, change the build mode to `Release`.

Then, click on Build->Build Solution. This will generate the exe inside the root folder’s x64/Release folder.

Now, you’ll have to copy the resource files (png files, wav files etc.) into this folder. To do this efficiently, we create a bash file in the project called `copy.sh`. It contains a simple `cp` file that copies over all the necessary resource files. 

You can run the bash file on Windows by using Git bash or any Linux shell while the cwd is the project root folder. Note: in case the path doesn’t work, you can try these versions of the commands. They use different versions of the paths.

`cp *.mp3 *.wav *.dll *.png *.fs *.vs *.ttf *.json .\x64\Release `

`cp *.mp3 *.wav *.dll *.png *.fs *.vs *.ttf *.json .\\x64\\Release `

`cp *.mp3 *.wav *.dll *.png *.fs *.vs *.ttf *.json ./x64/Release `


## Gameplay Instructions

### Controls
* WASD to move
* J to shoot
* Q to close the game

The goal is to collect stars for points. Collect stars and aim for a high score!
