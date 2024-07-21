#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAX_POINTS 10
#define MAX_CLUSTERS 5
#define MAX_ITERATIONS 100

typedef struct {
    float x, y;
} Point;

Point points[MAX_POINTS];
Point centroids[MAX_CLUSTERS];
int labels[MAX_POINTS];
int num_points = 0, num_clusters = 0;
bool show_clusters = false;
bool show_details = false;
bool show_graph = false;
bool setup_complete = false;

typedef enum { EUCLIDEAN, COSINE, JACCARD } DistanceMeasure;
DistanceMeasure current_distance_measure = EUCLIDEAN;

float cluster_colors[MAX_CLUSTERS][3] = {
    {1.0, 0.0, 0.0},  // Red
    {0.0, 1.0, 0.0},  // Green
    {0.0, 0.0, 1.0},  // Blue
    {1.0, 1.0, 0.0},  // Yellow
    {1.0, 0.0, 1.0}   // Magenta
};

char input_buffer[10] = { 0 };
bool entering_points = true;

void display();
void init();
void reshape(int, int);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void generateRandomPoints();
void kmeansClustering();
float calculateDistance(Point, Point);
float calculateCosineSimilarity(Point, Point);
float calculateJaccardDistance(Point, Point);
void drawText(const char*, float, float);
void drawColoredText(const char*, float, float, float, float, float);
void drawAxis();
void drawGraphLines();
void drawAxisLabels();
void drawLegend();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("K-Means Clustering Algorithm");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}

void init() {
    glClearColor(0.9, 0.9, 0.9, 1.0);  // Light gray
    glPointSize(10.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (!setup_complete) {
        // Initial setup page
        drawColoredText("Enter the number of points (1-10):", 200, 400, 0.1, 0.1, 0.6);
        drawColoredText("Enter the number of clusters (1-5):", 200, 350, 0.1, 0.1, 0.6);
        drawColoredText("Press Enter to confirm", 200, 300, 0.6, 0.1, 0.1);
        char buffer[50];
        if (entering_points) {
            sprintf_s(buffer, sizeof(buffer), "Points: %s", input_buffer);
        }
        else {
            sprintf_s(buffer, sizeof(buffer), "Clusters: %s", input_buffer);
        }
        drawColoredText(buffer, 500, 375, 0.1, 0.1, 0.1);
    }
    else if (!show_clusters && !show_details && !show_graph) {
        // Distance measure selection page
        drawColoredText("Choose Distance Measure:", 300, 350, 0.1, 0.1, 0.6);
        drawColoredText("1. Euclidean", 350, 300, 0.1, 0.6, 0.1);
        drawColoredText("2. Cosine Similarity", 350, 250, 0.1, 0.6, 0.1);
        drawColoredText("3. Jaccard Distance", 350, 200, 0.1, 0.6, 0.1);
    }
    else if (show_clusters && !show_details && !show_graph) {
        // Point details page
        drawColoredText("Point Details", 350, 550, 0.1, 0.1, 0.6);
        for (int i = 0; i < num_points; i++) {
            float r = cluster_colors[labels[i]][0];
            float g = cluster_colors[labels[i]][1];
            float b = cluster_colors[labels[i]][2];
            char buffer[200];
            sprintf_s(buffer, sizeof(buffer), "Point %d: (%.2f, %.2f) - Cluster %d", i + 1, points[i].x, points[i].y, labels[i] + 1);
            drawColoredText(buffer, 50, 500 - i * 30, r, g, b);
        }
        drawColoredText("Left Click to see the graph", 350, 50, 0.1, 0.1, 0.6);
    }
    else if (show_graph) {
        // Draw graph box
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(50, 100);
        glVertex2f(750, 100);
        glVertex2f(750, 550);
        glVertex2f(50, 550);
        glEnd();

        drawAxis();
        drawGraphLines();
        drawAxisLabels();
        drawLegend();

        // Draw points
        for (int i = 0; i < num_points; i++) {
            glColor3f(cluster_colors[labels[i]][0], cluster_colors[labels[i]][1], cluster_colors[labels[i]][2]);
            glBegin(GL_POINTS);
            glVertex2f(points[i].x, points[i].y);
            glEnd();
        }
        // Draw centroids
        glColor3f(0.0, 0.0, 0.0);
        for (int i = 0; i < num_clusters; i++) {
            glBegin(GL_POINTS);
            glVertex2f(centroids[i].x, centroids[i].y);
            glEnd();
        }
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (setup_complete && !show_clusters && !show_details && !show_graph) {
            show_clusters = true;
        }
        else if (show_clusters && !show_details && !show_graph) {
            show_graph = true;
        }
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (!setup_complete) {
        if (key == 13) {  // Enter key
            if (entering_points) {
                num_points = atoi(input_buffer);
                memset(input_buffer, 0, sizeof(input_buffer));
                entering_points = false;
            }
            else {
                num_clusters = atoi(input_buffer);
                if (num_points > 0 && num_clusters > 0) {
                    setup_complete = true;
                    generateRandomPoints();
                    kmeansClustering();
                }
            }
        }
        else if (key >= '0' && key <= '9') {
            int len = strlen(input_buffer);
            if (len < sizeof(input_buffer) - 1) {
                input_buffer[len] = key;
                input_buffer[len + 1] = '\0';
            }
        }
    }
    else if (!show_clusters && !show_details && !show_graph) {
        if (key == '1') {
            current_distance_measure = EUCLIDEAN;
            show_clusters = true;
        }
        else if (key == '2') {
            current_distance_measure = COSINE;
            show_clusters = true;
        }
        else if (key == '3') {
            current_distance_measure = JACCARD;
            show_clusters = true;
        }
        if (show_clusters) {
            kmeansClustering();
        }
    }
    glutPostRedisplay();
}

void generateRandomPoints() {
    for (int i = 0; i < num_points; i++) {
        points[i].x = 100 + rand() % 650;
        points[i].y = 150 + rand() % 400;
    }
    for (int i = 0; i < num_clusters; i++) {
        centroids[i].x = 100 + rand() % 650;
        centroids[i].y = 150 + rand() % 400;
    }
}

float calculateDistance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float calculateCosineSimilarity(Point a, Point b) {
    float dot_product = a.x * b.x + a.y * b.y;
    float magnitude_a = sqrt(a.x * a.x + a.y * a.y);
    float magnitude_b = sqrt(b.x * b.x + b.y * b.y);
    return 1.0 - dot_product / (magnitude_a * magnitude_b);
}

float calculateJaccardDistance(Point a, Point b) {
    float intersection = fmin(a.x, b.x) * fmin(a.y, b.y);
    float union_value = fmax(a.x, b.x) * fmax(a.y, b.y);
    return 1.0 - intersection / union_value;
}

void kmeansClustering() {
    int iterations = 0;
    int changed;

    do {
        changed = 0;

        // Assign points to the nearest centroid
        for (int i = 0; i < num_points; i++) {
            float min_dist;
            if (current_distance_measure == EUCLIDEAN) {
                min_dist = calculateDistance(points[i], centroids[0]);
            }
            else if (current_distance_measure == COSINE) {
                min_dist = calculateCosineSimilarity(points[i], centroids[0]);
            }
            else {
                min_dist = calculateJaccardDistance(points[i], centroids[0]);
            }

            int label = 0;
            for (int j = 1; j < num_clusters; j++) {
                float dist;
                if (current_distance_measure == EUCLIDEAN) {
                    dist = calculateDistance(points[i], centroids[j]);
                }
                else if (current_distance_measure == COSINE) {
                    dist = calculateCosineSimilarity(points[i], centroids[j]);
                }
                else {
                    dist = calculateJaccardDistance(points[i], centroids[j]);
                }
                if (dist < min_dist) {
                    min_dist = dist;
                    label = j;
                }
            }
            if (labels[i] != label) {
                labels[i] = label;
                changed = 1;
            }
        }

        // Update centroids
        Point new_centroids[MAX_CLUSTERS] = { 0 };
        int counts[MAX_CLUSTERS] = { 0 };

        for (int i = 0; i < num_points; i++) {
            new_centroids[labels[i]].x += points[i].x;
            new_centroids[labels[i]].y += points[i].y;
            counts[labels[i]]++;
        }

        for (int i = 0; i < num_clusters; i++) {
            if (counts[i] != 0) {
                centroids[i].x = new_centroids[i].x / counts[i];
                centroids[i].y = new_centroids[i].y / counts[i];
            }
        }

        iterations++;
    } while (changed && iterations < MAX_ITERATIONS);
}

void drawText(const char* text, float x, float y) {
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void drawColoredText(const char* text, float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void drawAxis() {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    // X-axis
    glVertex2f(50, 100);
    glVertex2f(750, 100);
    // Y-axis
    glVertex2f(50, 100);
    glVertex2f(50, 550);
    glEnd();

    drawColoredText("X-Axis", 730, 110, 0.0, 0.0, 0.0);
    drawColoredText("Y-Axis", 20, 530, 0.0, 0.0, 0.0);
}

void drawGraphLines() {
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_LINES);
    for (int i = 100; i <= 550; i += 50) {
        glVertex2f(50, i);
        glVertex2f(750, i);
    }
    for (int i = 50; i <= 750; i += 50) {
        glVertex2f(i, 100);
        glVertex2f(i, 550);
    }
    glEnd();
}

void drawAxisLabels() {
    char buffer[10];
    for (int i = 100; i <= 750; i += 50) {
        sprintf_s(buffer, sizeof(buffer), "%d", i - 50);
        drawColoredText(buffer, i, 90, 0.0, 0.0, 0.0);
    }
    for (int i = 100; i <= 550; i += 50) {
        sprintf_s(buffer, sizeof(buffer), "%d", i - 50);
        drawColoredText(buffer, 30, i, 0.0, 0.0, 0.0);
    }
}

void drawLegend() {
    for (int i = 0; i < num_clusters; i++) {
        glColor3f(cluster_colors[i][0], cluster_colors[i][1], cluster_colors[i][2]);
        glBegin(GL_QUADS);
        glVertex2f(650, 500 - i * 30);
        glVertex2f(670, 500 - i * 30);
        glVertex2f(670, 480 - i * 30);
        glVertex2f(650, 480 - i * 30);
        glEnd();
        char buffer[10];
        sprintf_s(buffer, sizeof(buffer), "Cluster %d", i + 1);
        drawColoredText(buffer, 680, 485 - i * 30, 0.0, 0.0, 0.0);
    }
}
