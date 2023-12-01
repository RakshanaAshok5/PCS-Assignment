#include <stdio.h>
#include <omp.h>
double calculateTrapezoidArea(double base1, double base2, double height) {
 return 0.5 * (base1 + base2) * height;
}
int main() {
 const int numTrapezoids = 1000000;
 const double base1 = 2.0;
 const double base2 = 5.0;
 const double height = 3.0;
 double totalArea = 0.0;
 double startTime, endTime;
 // Record start time
 startTime = omp_get_wtime();
 #pragma omp parallel for reduction(+:totalArea)
 for (int i = 0; i < numTrapezoids; ++i) {
 // Each thread calculates the area of its assigned trapezoid
 double trapezoidArea = calculateTrapezoidArea(base1, base2, height);
 // Sum up the areas using reduction clause
 totalArea += trapezoidArea;
 }
 // Record end time
 endTime = omp_get_wtime();
 printf("Total area of trapezoids: %f\n", totalArea);
 printf("Execution time: %f seconds\n", endTime - startTime);
 return 0;
}