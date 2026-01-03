#include "raylib.h"
#include <mpv/client.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    InitWindow(800, 450, "Retro music player");

    // 1. Initialize MPV
    mpv_handle *ctx = mpv_create();
    if (!ctx) return 1;

    // 2. CRITICAL OPTIONS
    // Enable ytdl to handle the https links in your .m3u
    mpv_set_option_string(ctx, "ytdl", "yes"); 
    // Force audio-only so it doesn't try to open a video window
    mpv_set_option_string(ctx, "vo", "null"); 
    // This will spit out everything mpv is doing to your terminal
    mpv_set_option_string(ctx, "terminal", "yes");
    mpv_set_option_string(ctx, "msg-level", "all=v");
    // Set volume to 100
    double vol = 100.0;
    mpv_set_property(ctx, "volume", MPV_FORMAT_DOUBLE, &vol);

    mpv_initialize(ctx);

    // 3. Load your playlist
    // "replace" tells mpv to clear the current track and play this immediately
    const char *cmd[] = {"loadfile", "~/Music/Rally House.m3u", NULL};
    mpv_command(ctx, cmd);

    while (!WindowShouldClose()) {
        // 4. Pump MPV Events (Required for playback to start)
        while (1) {
            mpv_event *event = mpv_wait_event(ctx, 0);
            if (event->event_id == MPV_EVENT_NONE) break;
            
            // Log errors to terminal for debugging
            if (event->event_id == MPV_EVENT_LOG_MESSAGE) {
                mpv_event_log_message *msg = event->data;
                printf("[mpv] %s", msg->text);
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PIB-BOY: STREAMING...", 20, 20, 20, GREEN);
            DrawRectangleLines(10, 10, 780, 430, GREEN);
        EndDrawing();
    }

    mpv_terminate_destroy(ctx);
    CloseWindow();
    return 0;
}
