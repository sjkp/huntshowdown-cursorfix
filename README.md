# How to use
Run huntcursorfix.exe and an empty window appear (just leave it open while you play)

When a process with a window titled "Hunt: Showdown" is running you can use CTRL+SHIFT+h to toggle the red cursor on/off in the game. 

Turning the cursor off also removes it in the menu, which it is why a global hot key to turn it back on is useful. 

# How to build 

cl /EHsc /Zi huntcursorfix.cpp user32.lib 

# Flagged by Anti Virus
Not sure why, probably because it uses APIs commonly used by malware (The tool scans for processes to find the Hunt game). 

Building in debug mode seems to avoid it. Otherwise you have to white list the tool in your anti virus program. 

# Can I get banned for using it
I doubt it, but no guarantees. I'm pretty sure the red cursor is a bug and banning people from removing it themselves when the Hunt developers can fix it would be silly.

# Why C++
Because it was the only way I could make a small executable to fix it.