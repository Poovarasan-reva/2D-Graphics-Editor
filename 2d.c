#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 40
#define MAX_OBJECTS 10

// Structure to store properties of each shape
typedef struct {
    char type;             // 'C' = Circle, 'R' = Rectangle, 'L' = Line
    int x1, y1, x2, y2, r; // Coordinates and radius variables
} Shape;

// Global variables
Shape shapes[MAX_OBJECTS]; // Array to store all added objects
int object_count = 0;      // Counter for total active objects
char canvas[ROWS][COLS];   // 2D grid array representing the screen

// Function to fill the canvas with background characters ('_')
void clearCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

// Function to calculate and plot shapes onto the 2D canvas array
void rasterizeShapes() {
    clearCanvas(); // Always start with a fresh blank canvas
    
    // Loop through all shapes stored in our array history
    for (int k = 0; k < object_count; k++) {
        Shape s = shapes[k];
        
        // 1. Drawing a Rectangle Outline
        if (s.type == 'R') { 
            for (int i = s.y1; i <= s.y2; i++) {
                for (int j = s.x1; j <= s.x2; j++) {
                    // Check boundaries to avoid crashing out of array limits
                    if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
                        // Plot '*' only on the borders of the rectangle
                        if (i == s.y1 || i == s.y2 || j == s.x1 || j == s.x2)
                            canvas[i][j] = '*';
                    }
                }
            }
        } 
        // 2. Drawing a Circle Outline
        else if (s.type == 'C') { 
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    // Standard Circle Distance Formula: sqrt((x-h)^2 + (y-k)^2)
                    double dist = sqrt(pow(j - s.x1, 2) + pow(i - s.y1, 2));
                    // If the distance matches the radius, place a border pixel
                    if (abs((int)dist - s.r) == 0) {
                        canvas[i][j] = '*';
                    }
                }
            }
        } 
        // 3. Drawing a Line (Simplified to Straight Horizontal or Vertical)
        else if (s.type == 'L') { 
            if (s.y1 == s.y2) { // Horizontal line condition
                for (int j = s.x1; j <= s.x2; j++) canvas[s.y1][j] = '*';
            } else if (s.x1 == s.x2) { // Vertical line condition
                for (int i = s.y1; i <= s.y2; i++) canvas[i][s.x1] = '*';
            }
        }
    }
}

// Function to print the processed canvas array to the screen
void displayCanvas() {
    rasterizeShapes(); // Calculate all shapes before printing
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n"); // Move to the next row
    }
}

// Function to delete an object by index and shift the remaining ones
void deleteObject(int index) {
    if (index >= 0 && index < object_count) {
        // Shift all elements on the right side one position to the left
        for (int i = index; i < object_count - 1; i++) {
            shapes[i] = shapes[i + 1];
        }
        object_count--; // Reduce total active shape count
        printf("Object deleted successfully.\n");
    } else {
        printf("Invalid Object ID!\n");
    }
}

int main() {
    int choice, id;
    
    // Infinite menu loop until user exits
    while (1) {
        printf("\n--- 2D Graphics Editor ---\n");
        printf("1. Add Rectangle\n2. Add Circle\n3. Add Line\n4. Display Canvas\n5. Delete Object\n6. Modify Object\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Handling structural data additions
        if (choice >= 1 && choice <= 3) {
            if (object_count >= MAX_OBJECTS) {
                printf("Canvas storage full!\n");
                continue;
            }
            
            // Map choice to character types
            shapes[object_count].type = (choice == 1) ? 'R' : (choice == 2) ? 'C' : 'L';
            
            // Get data parameters based on user shape choice
            if (choice == 1) {
                printf("Enter top-left (x1 y1) and bottom-right (x2 y2): ");
                scanf("%d %d %d %d", &shapes[object_count].x1, &shapes[object_count].y1, &shapes[object_count].x2, &shapes[object_count].y2);
            } else if (choice == 2) {
                printf("Enter center (x1 y1) and radius r: ");
                scanf("%d %d %d", &shapes[object_count].x1, &shapes[object_count].y1, &shapes[object_count].r);
            } else {
                printf("Enter start (x1 y1) and end (x2 y2): ");
                scanf("%d %d %d %d", &shapes[object_count].x1, &shapes[object_count].y1, &shapes[object_count].x2, &shapes[object_count].y2);
            }
            object_count++; // Move tracking index forward
        } 
        else if (choice == 4) {
            displayCanvas(); // Re-render and print
        } 
        else if (choice == 5) {
            printf("Enter Object ID to delete (0 to %d): ", object_count - 1);
            scanf("%d", &id);
            deleteObject(id); // Fire deletion function
        } 
        else if (choice == 6) {
            printf("Enter Object ID to modify (0 to %d): ", object_count - 1);
            scanf("%d", &id);
            if (id >= 0 && id < object_count) {
                // Modifying values in-place inside the tracking structure array
                printf("Enter new primary coordinates (x1 y1): ");
                scanf("%d %d", &shapes[id].x1, &shapes[id].y1);
                printf("Object updated. Display canvas to view changes.\n");
            } else {
                printf("Invalid ID!\n");
            }
        } 
        else if (choice == 7) {
            exit(0); // Safely close program execution
        }
    }
    return 0;
}
