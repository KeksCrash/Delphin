#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/widget.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    FortuneMessageScene,
    SceneCount,
} FortuneScene;

typedef struct App {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Widget* widget;
    uint32_t random_seed; 
} App;

const char* fortune_messages[] = {
    "Glaube an dich selbst!",
    "Jeder Tag ist eine neue Gelegenheit.",
    "Bleibe positiv und stark.",
    "Erfolg ist nicht endgültig, Misserfolg ist nicht fatal.",
    "Du bist zu erstaunlichen Dingen fähig.",
    "Träume es. Glaube es. Erreiche es.",
    "Herausforderungen machen das Leben interessant.",
    "Das einzige Limit ist dein Geist.",
    "Atme tief durch und mach weiter.",
    "Sei der Grund, warum heute jemand lächelt.",
    "Du bist stärker, als du denkst.",
    "Kleine Schritte jeden Tag führen zu großen Ergebnissen.",
    "Der beste Weg hinaus ist immer hindurch.",
    "Warte nicht auf Gelegenheit. Schaffe sie.",
    "Glaube, du kannst es, und du bist schon halb dort.",
    "Deine Ausstrahlung zieht dein Umfeld an.",
    "Gehe immer weiter, egal was kommt.",
    "Jeder Erfolg beginnt mit der Entscheidung, es zu versuchen.",
    "Der weise Krieger vermeidet den Kampf.",
    "Gewinnen ohne zu kämpfen ist der höchste Sieg.",
    "Der Sieg gehört dem Beharrlichsten.",
    "Träume größer. Handle größer.",
    "Höre nicht auf, wenn du müde bist. Höre auf, wenn du fertig bist.",
    "Träume es. Wünsche es. Tu es!",
    "Frauen mit Würmern und Warzen, lassen sich nur schwer verarzten ;)!",
    "Wenn du nicht mehr weiter weißt, dann Gründe einen Arbeitskreis!!!",
    "Alhamdulillah, inshallah heute wird ein guter Tag <3",
    "Alhamdulillah, inshallah heute wird alles besser! ;)",
    "Alhamdulillah, inshallah jetzt ist alles besser!",
    "Alhamdulillah, inshallah alles geht gut <3! :)",
    "Mach dir keinen Kopf, denn es ist alles gut solange du Gott an deiner Seite hast <3",
};
const uint8_t fortune_count = sizeof(fortune_messages) / sizeof(fortune_messages[0]);

const char* get_random_fortune(App* app) {
    app->random_seed = (app->random_seed + 1) % fortune_count; 
    return fortune_messages[app->random_seed]; 
}

void add_word_wrapped_text(Widget* widget, const char* text, int max_chars_per_line) {
    char line_buffer[max_chars_per_line + 1]; 
    int line_length = 0;
    int y_offset = 15; 

    while (*text) {
        const char* word_start = text;
        int word_length = 0;

        while (*text && *text != ' ' && *text != '\n') {
            word_length++;
            text++;
        }

        if (line_length + word_length > max_chars_per_line) {
            line_buffer[line_length] = '\0';
            widget_add_string_multiline_element(widget, 10, y_offset, AlignLeft, AlignTop, FontPrimary, line_buffer);
            y_offset += 15; 
            line_length = 0;
        }

        if (line_length > 0) {
            line_buffer[line_length++] = ' ';
        }
        strncpy(&line_buffer[line_length], word_start, word_length);
        line_length += word_length;

        while (*text == ' ') text++;
    }

    if (line_length > 0) {
        line_buffer[line_length] = '\0';
        widget_add_string_multiline_element(widget, 10, y_offset, AlignLeft, AlignTop, FontPrimary, line_buffer);
    }
}

void fortune_message_scene_on_enter(void* context) {
    App* app = context;
    widget_reset(app->widget);

    const char* message = get_random_fortune(app);
    add_word_wrapped_text(app->widget, message, 20); 

    view_dispatcher_switch_to_view(app->view_dispatcher, 0);
}

bool fortune_message_scene_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false; 
}

void fortune_message_scene_on_exit(void* context) {
    App* app = context;
    widget_reset(app->widget);
}

void (*const scene_on_enter_handlers[])(void*) = {
    fortune_message_scene_on_enter,
};

bool (*const scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    fortune_message_scene_on_event,
};

void (*const scene_on_exit_handlers[])(void*) = {
    fortune_message_scene_on_exit,
};

static const SceneManagerHandlers scene_manager_handlers = {
    .on_enter_handlers = scene_on_enter_handlers,
    .on_event_handlers = scene_on_event_handlers,
    .on_exit_handlers = scene_on_exit_handlers,
    .scene_num = SceneCount,
};

bool fortune_back_event_callback(void* context) {
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->scene_manager = scene_manager_alloc(&scene_manager_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();
    
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, fortune_back_event_callback);
    
    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, 0, widget_get_view(app->widget));
    
    app->random_seed = furi_get_tick(); 
    return app;
}

static void app_free(App* app) {
    furi_assert(app);
    view_dispatcher_remove_view(app->view_dispatcher, 0);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);
    widget_free(app->widget);
    free(app);
}

int32_t fortune_cookie_app(void* p) {
    UNUSED(p);
    App* app = app_alloc();

    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, FortuneMessageScene);
    view_dispatcher_run(app->view_dispatcher);

    app_free(app);
    return 0;
}
