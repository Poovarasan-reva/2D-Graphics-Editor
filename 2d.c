#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 50
#define HEIGHT 20
#define MAX_SHAPES 100

// Shape types
typedef enum { LINE, RECTANGLE } ShapeType;

// Structure to store shape data
typedef struct {
    ShapeType type;
    int x1, y1, x2, y2; // Coordinates
    int active;         // 1 if active, 0 if deleted
} Shape;

// Global Variables
char canvas[HEIGHT][WIDTH];
Shape shapes[MAX_SHAPES];
int shape_count = 0;

// Function Prototypes
void init_canvas();
void display_canvas();
void redraw_all_shapes();
void draw_line_on_canvas(int x1, int y1, int x2, int y2);
void add_line();
void delete_shape();
void clear_input_buffer();

int main() {
    int choice;

    init_canvas();

    while (1) {
        printf("\n--- 2D GRAPHICS EDITOR ---\n");
        printf("1. Display Canvas\n");
        printf("2. Add Line\n");
        printf("3. Delete Shape\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                display_canvas();
                break;
            case 2:
                add_line();
                break;
            case 3:
                delete_shape();
                break;
            case 4:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Utility to prevent infinite menu loops on bad inputs
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fills the canvas with underscores '_'
void init_canvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = '_';
        }
    }
}

// Prints the current canvas to the console
void display_canvas() {
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

// Bresenham's Line Algorithm to plot asterisks '*'
void draw_line_on_canvas(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            canvas[y1][x1] = '*';
        }
        
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Clears canvas and redraws only the active shapes
void redraw_all_shapes() {
    init_canvas(); 
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].active) {
            if (shapes[i].type == LINE) {
                draw_line_on_canvas(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
            }
        }
    }
}

// Handles user input for adding a line
void add_line() {
    if (shape_count >= MAX_SHAPES) {
        printf("Error: Maximum shape limit reached!\n");
        return;
    }

    int x1, y1, x2, y2;
    printf("Enter Start Coordinates (X1 Y1) [0-%d 0-%d]: ", WIDTH-1, HEIGHT-1);
    if (scanf("%d %d", &x1, &y1) != 2) {
        printf("Invalid input coordinates!\n");
        clear_input_buffer();
        return;
    }
    
    printf("Enter End Coordinates (X2 Y2) [0-%d 0-%d]: ", WIDTH-1, HEIGHT-1);
    if (scanf("%d %d", &x2, &y2) != 2) {
        printf("Invalid input coordinates!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Bound Validation Protection
    if (x1 < 0 || x1 >= WIDTH || y1 < 0 || y1 >= HEIGHT ||
        x2 < 0 || x2 >= WIDTH || y2 < 0 || y2 >= HEIGHT) {
        printf("Error: Coordinates are out of canvas boundaries!\n");
        return;
    }

    // Save shape details
    shapes[shape_count].type = LINE;
    shapes[shape_count].x1 = x1;
    shapes[shape_count].y1 = y1;
    shapes[shape_count].x2 = x2;
    shapes[shape_count].y2 = y2;
    shapes[shape_count].active = 1;
    shape_count++;

    redraw_all_shapes();
    printf("Line added successfully!\n");
}

// Deletes a shape by marking its active flag to 0
void delete_shape() {
    printf("\n--- Active Shapes ---\n");
    int active_found = 0;
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].active) {
            printf("ID: %d | Type: Line | Points: (%d,%d) to (%d,%d)\n", 
                   i, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
            active_found = 1;
        }
    }

    if (!active_found) {
        printf("No shapes available to delete.\n");
        return;
    }

    int id;
    printf("Enter the ID of the shape to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID type!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (id >= 0 && id < shape_count) {
        if (shapes[id].active) {
            shapes[id].active = 0; 
            redraw_all_shapes();   
            printf("Shape %d deleted successfully!\n", id);
        } else {
            printf("Shape already deleted!\n");
        }
    } else {
        printf("Invalid ID range!\n");
    }
}