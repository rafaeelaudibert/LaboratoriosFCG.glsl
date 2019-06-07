// Allowed libraries
#include <stdio.h>
#include <math.h>

// Helper functions, using define because of their one-liner nature
#define MAX(a, b, c) (a > b ? (a > c ? a : c) : (b > c ? b : c))            // Maximum between 3 numbers
#define DIST(ax, ay, bx, by) (sqrt(pow((ax - bx), 2) + pow((ay - by), 2)))  // Euclidean distance between 2 points
#define EDGE_EQUATIONS(A, x, B, y, C) (A * x + B * y + C)                   // Edge equations
#define GEQ(a, b, c, n) (a >= n && b >= n && c >= n)                        // Returns if 3 numbers are grater or equal than a number n

// A simple atoi() function
int atoi(char* str)
{
    int res = 0; // Initialize result
    int sign = 1; // Initialize sign as positive
    int i = 0; // Initialize index of first digit

    // If number is negative, then update sign
    if (str[0] == '-') {
        sign = -1;
        i++; // Also update index of first digit
    }

    // Iterate through all digits and update the result
    for (; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    // Return result with sign
    return sign * res;
}

int main(int argc, char* argv[]){

    // Parse command line
    int L  = atoi(argv[1]), A  = atoi(argv[2]),
        ax = atoi(argv[3]), ay = atoi(argv[4]),
        bx = atoi(argv[5]), by = atoi(argv[6]),
        cx = atoi(argv[7]), cy = atoi(argv[8]);

    // Edge Equations = (x, y) = (vy, -vx)
    int ee_abx = by - ay, ee_aby = -(bx - ax);
    int ee_bcx = cy - by, ee_bcy = -(cx - bx);
    int ee_cax = ay - cy, ee_cay = -(ax - cx);

    // Size of the vectors
    float ab_size = DIST(ax, ay, bx, by);
    float bc_size = DIST(bx, by, cx, cy);
    float ca_size = DIST(cx, cy, ax, ay);

    // Maximum vector size (this is the maximum distance between any point in the triangle
    // It is used to check against the 3 triangle points being colinears
    float max_size = MAX(ab_size, bc_size, ca_size);

    // Variables used in the printing loop.
    int f_ab = 0, f_bc = 0, f_ca = 0; // Hold the edge equations
    float d_a = 0, d_b = 0, d_c = 0;  // Hold the distance from the point to each of the triangle vertex

    // Print .pbm header
    printf("P1\n");
    printf("%d %d\n", L, A);

    // Iterate image on each column for each row
    for (int i = A; i > 0; i--){
        for (int j = 1; j <= L; j++){

            // Calculate edge equations
            //    A1  * x + B1  * y + C1
            f_ab = EDGE_EQUATIONS(ee_aby, i, ee_abx, j, - ee_abx * ax - ee_aby * ay);
            f_bc = EDGE_EQUATIONS(ee_bcy, i, ee_bcx, j, - ee_bcx * bx - ee_bcy * by);
            f_ca = EDGE_EQUATIONS(ee_cay, i, ee_cax, j, - ee_cax * cx - ee_cay * cy);

            // Calculate the point's distance to each of the triangle's vertex
            d_a = DIST(j, i, ax, ay);
            d_b = DIST(j, i, bx, by);
            d_c = DIST(j, i, cx, cy);

            // The point is inside the triangle, if:
            //   * the point is at most the maximum possible distance from the triangle (think like a bounding circle), and;
            //   * if fab, fbc and fca are greater or equal to 0 (or the opposite)
            if (MAX(d_a, d_b, d_c) <= max_size && (GEQ(f_ab, f_bc, f_ca, 0) || GEQ(-f_ab, -f_bc, -f_ca, 0))) {
                printf("0 ");
            }
            else {
                printf("1 ");
            }
        }
        printf("\n");
    }
}
