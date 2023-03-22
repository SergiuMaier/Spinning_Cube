/*------------------------------------- 
    Author: SergiuMaier
    22/03/2023
-------------------------------------*/
#include <iostream>
#include <math.h>
#include <string.h>

// Define the cube dimensions and console window dimensions:
float cubeWidth = 20;// size of the cube
int width = 160;     // width of the console window
int height = 44;     // height of the console window

// Define arrays:
float zBuffer[160*44];// buffer used to store the distance of each surface from the camera
char buffer[160 * 44];  // buffer used to store the ASCII characters for each surface

int backgroundASCIICode = ' '; // ASCII code for the background character
float K1 = 40;// constant used in the projection formula
int distanceFromCam = 100;// distance from the camera to the cube

// Define variables for the cube rotation speed and current rotation angles
float incrementSpeed = 0.75; // speed at which the cube rotates
float A, B, C;// angles used to rotate the cube 

// Define variables for the position of each surface of the cube in 3D space:
float x, y, z;// coordinates of each point on the cube
float ooz;    // one over z value used in the projection formula
float xp, yp; // coodronates of the console window
int idx;      // index of the current pixel in the buffer and z-buffer

// Function to calculate the x coordinate of a point on the cube based on its i, j, and k coordinates
float calculateX(int i, int j, int k) 
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

// Function to calculate the y coordinate of a point on the cube based on its i, j, and k coordinates
float calculateY(int i, int j, int k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}

// Function to calculate the z coordinate of a point on the cube based on its i, j, and k coordinates
float calculateZ(int i, int j, int k)
{
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

// Function to calculate the position of a surface on the cube and draw it on the console window
void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) 
{
    // Calculate the position of the surface in 3D space
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;
    
    // Convert the 3D position to 2D coordinates on the console window using a projection formula
    ooz = 1 / z;
    xp = (int)(width / 2 - 2 * cubeWidth + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);
    
    // Calculate the index of the pixel on the console window
    idx = xp + yp * width;

    // Check if the surface is closer to the camera than any other surface previously drawn at that pixel
    if (idx >= 0 && idx < width * height)
    {    
        // If it is, update the z-buffer and character buffer
        if (ooz > zBuffer[idx]) 
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    std::cout << "\x1b[2J";// Clear the console window
    
    while (1) 
    {   
        // Reset the arrays
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        // Loop through each point on the cube's surface and calculate its position in 3D space
        // For each point, calculate its position on the 2D screen using perspective projection
        // Then, update the buffer and z-buffer arrays if the point is closer to the camera than what was previously drawn
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) 
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) 
            {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateForSurface(cubeWidth, cubeY, cubeX, 'O');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '#');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '@');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, '~');
                calculateForSurface(cubeX, cubeWidth, cubeY, '"');
            }
        }

        std::cout << "\x1b[H";// Move the cursor to the top-left of the terminal window

        // Draw the contents of the buffer array to the screen
        for (int k = 0; k < width * height; k++) 
        {
            putchar(k % width ? buffer[k] : 10);
        }

        // Update the angles used to calculate the 3D positions of the cube's points
        A += 0.005;
        B += 0.005;
    }

    return 0;
}