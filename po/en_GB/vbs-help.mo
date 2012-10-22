USING VBS

Keyboard controls:
* spacebar - toggles play/pause
* b - enter next subtitle
* m - exit current subtitle
* n - exit current and enter next
* s - export subtitles to teh preset destinaton (overwrites the file is exists)

IMPORTING SUBTITLES

Use the Subtitles menu to import a text-only or a SubRip file and to specify its encoding.

Supported formats:
* Plain text: one line per subtitle. For long subtitles, use the pipe symbol “|” where you want the subtitle split into several lines
* SubRip

From the same menu section you can set import encoding (default is UTF-8).

EXPORTING SUBTITLES

Subtitles are automatically exported each time: 
* a subtitle goes "out" ("n" or "m" key is pressed)
* when the "s" key is pressed
* when the program is terminated

Use the Subtitles menu to set the export destination (default is /tmp/vbs_export.srt).

From the same menu section you can set:
* Export encoding (default is UTF-8)
* Export newline character (default is LF).

EDITING SUBTITLES LIST

Click a subtitle to highlight it.

Use the Edit menu to: 
* Insert blank subtitle before the current (highlighted) one.
* Insert blank subtitle after the current (highlighted) one.
* Delete the current (highlighted) one.
* Clear the timing of the current (highlighted) subtitle.
* Clear the timing of all subtitles after the current (highlighted) one.

Double click start or end time to go to that subtitle. If video is loaded, you will be send 5 seconds befoe the beginning of the subtitle. 

Double click text to edit it inline. 

LOADING VIDEO

Use the Video menu to load video file.

Supported formats: any format which your back-end (GStreamer or MPlayer) can play.

VBS supports two back-ends: 
* MPlayer
* Gstreamer

Support for either or both can be included. If both are included, GStreamer will be default and it can be changed in the Video menu.

In the same menu you can set the video output method:
* For GStreamer, chose between X11 (most conservative, CPU-intensive, but most compatible; default), Xv (less CPU intensive, works with all modern video adapters) and SDL (use only as fail-safe).
* MPlayer video output cannot be changed in VBS; use it's own menu for this.

USING VBS WITHOUT VIDEO

You can use VBS for timing subtitles without loading video (e.g., when watching the video on TV). To set the time to “zero”, use the Edit -> Set Start Time menu when the movie begins. 

USING THE NETWORK

Use the Network menu to enable/disable sending subtitle info to a vbsd server. When enabled, the network mode will be shown as “Send”. If not using the network, turn it off to improve performance (default).

If using the network, in the same menu also set: 
* Network server (FQDN); will only be accepted if can be resolved to IP address.
* TCP port on which vbsd is listening (default: 42)
* Magic key used to communicate with vbsd (default: 1973); must be same on the server side. 

ADDITIONAL TOOLS

Except the current programme (the master client or vbsm), the VBS also includes: 
* The relay server or vbsd – used to relay subtitles from vbsm or vbss to vbss,
* The slave client or vbss – used to project subtitles which are either read from a local SubRip file or received over the network from a vbsd server. 
* The configuration tool or vbsc – used to configure vbss. 

