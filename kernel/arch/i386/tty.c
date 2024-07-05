#include <stddef.h>
#include <kernel/tty.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static size_t last_wrote_index;

/* Returns a VGA color code */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

/* Returns a VGA entry */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}


void set_text_color(uint8_t color){
	terminal_color = vga_entry_color(color, VGA_COLOR_BLACK);
}

void set_bg_color(uint8_t color){
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, color);
}

void terminal_initialize(void) {
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	last_wrote_index = 0;
	terminal_clear_all();
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_scroll(void){
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			const size_t prev_index = (y - 1) * VGA_WIDTH + x;
			terminal_buffer[prev_index] = terminal_buffer[index];
		}
	}

	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}

}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	last_wrote_index = index;
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++){
		terminal_putchar(data[i]);
	}

	if (terminal_row == VGA_HEIGHT || 
		last_wrote_index == VGA_HEIGHT * VGA_WIDTH - 1){
		terminal_clear_all();
	}

}

void terminal_newline(void){
	terminal_column = 0;
	terminal_row++;
}

void terminal_clear_all(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	terminal_row = 0;
	terminal_column = 0;
	last_wrote_index = 0;

}

void terminal_clear_char(int size) {
    while (size > 0) {
		// I dont want to delete previous lines
		if (terminal_column == 0){
			return;
		}
		
		terminal_buffer[last_wrote_index] = vga_entry(' ', terminal_color);		
		terminal_column--;
		last_wrote_index--;
		size--;
    }
}