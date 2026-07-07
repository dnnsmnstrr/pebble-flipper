#include <pebble.h>

extern uint32_t MESSAGE_KEY_IMAGE_COLOR;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;

static GFont s_time_font;
static GFont s_date_font;

static int s_image_color = 0;

#define KEY_IMAGE_COLOR MESSAGE_KEY_IMAGE_COLOR
#define PERSIST_KEY_IMAGE_COLOR 1

static void load_image() {
    if (s_image_bitmap) {
        gbitmap_destroy(s_image_bitmap);
    }
    switch (s_image_color) {
        case 1:
            s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FLIPPER_ORANGE);
            break;
        case 2:
            s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FLIPPER_BLACK);
            break;
        default:
            s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FLIPPER_WHITE);
            break;
    }
    if (s_image_layer) {
        bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
    }
}

static void apply_color_scheme() {
    if (s_image_color == 2) {
        window_set_background_color(s_main_window, GColorWhite);
        text_layer_set_text_color(s_time_layer, GColorBlack);
        text_layer_set_text_color(s_date_layer, GColorBlack);
    } else {
        window_set_background_color(s_main_window, GColorBlack);
        text_layer_set_text_color(s_time_layer, GColorWhite);
        text_layer_set_text_color(s_date_layer, GColorWhite);
    }
}

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
    Tuple *t = dict_find(iter, KEY_IMAGE_COLOR);
    if (t) {
        s_image_color = t->value->uint8;
        persist_write_int(PERSIST_KEY_IMAGE_COLOR, s_image_color);
        load_image();
        apply_color_scheme();
    }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage dropped: %d", (int)reason);
}

static void outbox_failed_callback(DictionaryIterator *iter, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage send failed: %d", (int)reason);
}

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    static char s_time_buffer[8];
    strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?
                                                     "%H:%M" : "%I:%M", tick_time);
    text_layer_set_text(s_time_layer, s_time_buffer);

    static char s_date_buffer[16];
    strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %d", tick_time);
    text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_JERSEY_56));
    s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HAXR_32));

    s_image_color = persist_exists(PERSIST_KEY_IMAGE_COLOR) ? persist_read_int(PERSIST_KEY_IMAGE_COLOR) : 0;

    int time_height = 56;
    int date_height = 40;
    int time_y = -4;

    s_time_layer = text_layer_create(GRect(0, time_y, bounds.size.w, time_height));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    int date_y = bounds.size.h - date_height - 6;
    s_date_layer = text_layer_create(GRect(0, date_y, bounds.size.w, date_height));
    text_layer_set_background_color(s_date_layer, GColorClear);
    text_layer_set_font(s_date_layer, s_date_font);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

    int image_y = time_y + time_height + 4;
    int image_h = date_y - image_y - 4;
    s_image_layer = bitmap_layer_create(GRect(8, image_y, bounds.size.w - 16, image_h));
    bitmap_layer_set_background_color(s_image_layer, GColorClear);
    bitmap_layer_set_compositing_mode(s_image_layer, GCompOpSet);
    bitmap_layer_set_alignment(s_image_layer, GAlignCenter);

    apply_color_scheme();
    load_image();

    layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_date_layer);
    fonts_unload_custom_font(s_time_font);
    fonts_unload_custom_font(s_date_font);
    bitmap_layer_destroy(s_image_layer);
    gbitmap_destroy(s_image_bitmap);
}

static void init() {
    s_main_window = window_create();

    window_set_background_color(s_main_window, GColorBlack);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    window_stack_push(s_main_window, true);

    update_time();
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_open(128, 128);
}

static void deinit() {
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
