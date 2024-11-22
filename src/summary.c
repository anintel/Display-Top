#include <ncurses.h>
#include "summary.h"

WINDOW *display_summary(int *level){

    WINDOW *win = newwin(getmaxy(stdscr)/2, getmaxx(stdscr), 0, 0);
    keypad(win, TRUE);
    scrollok(win, TRUE);
    box(win, 0, 0);

    mvwprintw(win, 1, 1, "DRM SUMMARY");

    // Display static information
    mvwprintw(win, 3, 1, "Display Configuration:");
    mvwprintw(win, 4, 1, "Active CRTCs: crtc-0, crtc-1");
    mvwprintw(win, 5, 1, "Connected Outputs: DP-1: Connected, HDMI-A-1: Disconnected");
    mvwprintw(win, 6, 1, "Encoders: Encoder-0: crtc-0, Encoder-1: DP-1");

    mvwprintw(win, 8, 1, "GPU Information:");
    mvwprintw(win, 9, 1, "GPU Model: Intel Iris Xe Graphics");
    mvwprintw(win, 10, 1, "Frequency: Base: 300 MHz, Boost: 900 MHz");

    mvwprintw(win, 12, 1, "Power Management:");
    mvwprintw(win, 13, 1, "Runtime PM: Active");
    mvwprintw(win, 14, 1, "PSR: Enabled");
    mvwprintw(win, 15, 1, "FBC: Enabled");

    mvwprintw(win, 17, 1, "Display Capabilities:");
    mvwprintw(win, 18, 1, "supports_tv: no, has_hdcp: yes, has_dmc: yes, has_dsc: no");

    mvwprintw(win, 20, 1, "Error and Debug:");
    mvwprintw(win, 21, 1, "GPU Errors: No active errors");
    mvwprintw(win, 22, 1, "Hotplug Storm: No storm detected");
    mvwprintw(win, 23, 1, "System State: Stable, No active warnings");
    wrefresh(win);
    
    // wgetch(win);

    return win;
}