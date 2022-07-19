# Summary
Quick fix to prevent a remote code execution exploit in BlazBlue: Cross Tag Battle on Steam.\
Further reading:\
https://github.com/ThingsNStuffYouKnow/BBTAG-RCE-PoC \
https://github.com/ThingsNStuffYouKnow/BBCF-RCE-PoC

# Usage
Make sure to build as Release x86. Once the game is started, use your favourite DLL-Injector to load the dll into the BBTAG.exe process during runtime.\
If you don't get the success message, the patch is not installed and you could be vulnerable.

# External
https://github.com/SteamRE/open-steamworks
