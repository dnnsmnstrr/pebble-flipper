#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;

  

// Custom fonts
static GFont s_time_font;
static GFont s_date_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?
                                                    "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time_buffer);

  // Write the current date into a buffer
  static char s_date_buffer[16];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %d", tick_time);

  // Display the date
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Custom fonts
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HAXR_56));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HAXR_24));

  // Image
  s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FLIPPER);

  // Layout
  int time_height = 56;
  int date_height = 30;

  // Time pinned to top edge
  int time_y = -8;
  s_time_layer = text_layer_create(GRect(0, time_y, bounds.size.w, time_height));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Date pinned to bottom edge
  int date_y = bounds.size.h - date_height;
  s_date_layer = text_layer_create(GRect(0, date_y, bounds.size.w, date_height));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Image centered in the space between time and date
  int image_y = time_y + time_height;
  int image_h = date_y - image_y;

  s_image_layer = bitmap_layer_create(GRect(0, image_y, bounds.size.w, image_h));
  bitmap_layer_set_background_color(s_image_layer, GColorClear);
  bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
  bitmap_layer_set_compositing_mode(s_image_layer, GCompOpSet);
  bitmap_layer_set_alignment(s_image_layer, GAlignCenter);

  layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);

  // Unload custom fonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  
  // Unload image
  bitmap_layer_destroy(s_image_layer);
  gbitmap_destroy(s_image_bitmap);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set the background color
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
