#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
  // Reassign the red, green and blue fields for each RGBTRIPLE
  // to the average of the three values
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = round((image[i][j].rgbtRed + image[i][j].rgbtGreen +
                            image[i][j].rgbtBlue) / 3.0);
    }
  }

  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
  // Temporary storage for pixel swapping
  RGBTRIPLE temp;

  // Swap leftmost unswapped pixel for rightmost, moving to the middle from both
  // ends, stopping at the appropriate place
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width / 2; j++)
    {
      temp = image[i][j];
      image[i][j] = image[i][width - (j + 1)];
      image[i][width - (j + 1)] = temp;
    }
  }

  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
  // Track sums for averaging
  int blue_total, green_total, red_total;
  float num_pix;
  // Adjust number of iterations to handle edge pixles
  int k, l, end_k, end_l;

  // Store the averaged values until ready to overwrite original pixels
  RGBTRIPLE(*averages)
  [width] = calloc(height, width * sizeof(RGBTRIPLE));
  if (image == NULL)
  {
    printf("Not enough memory to blur image.\n");
    return;
  }

  // Sum each color value for each pixel with those of all its contiguous pixles
  // being sure not to incorporate garbage data if the "center" pixel is on an edge
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      blue_total = green_total = red_total = 0;
      num_pix = 0.0;

      k = (i == 0) ? 0 : -1;
      end_k = (i == height - 1) ? 0 : 1;

      while (k <= end_k)
      {
        l = (j == 0) ? 0 : -1;
        end_l = (j == width - 1) ? 0 : 1;

        while (l <= end_l)
        {
          blue_total += image[i + k][j + l].rgbtBlue;
          green_total += image[i + k][j + l].rgbtGreen;
          red_total += image[i + k][j + l].rgbtRed;
          num_pix++;
          l++;
        }
        k++;
      }
      // Determine the average for each color and store it in averages
      averages[i][j].rgbtBlue = round(blue_total / num_pix);
      averages[i][j].rgbtGreen = round(green_total / num_pix);
      averages[i][j].rgbtRed = round(red_total / num_pix);
    }
  }

  // Overwrite pixels in original image with averages
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      image[i][j] = averages[i][j];
    }
  }

  free(averages);

  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
  // Variables for calculating weighted sums
  int blue_sum_x, green_sum_x, red_sum_x;
  int blue_sum_y, green_sum_y, red_sum_y;
  int w_sum_blue, w_sum_green, w_sum_red;
  // Adjust number of iterations to handle edge pixles
  int k, l, end_k, end_l;

  // Store the weighed sums until ready to overwrite original pixels
  RGBTRIPLE(*w_sums)
  [width] = calloc(height, width * sizeof(RGBTRIPLE));
  if (image == NULL)
  {
    printf("Not enough memory to perform edge detection.\n");
    return;
  }

  // Calculate Gx of each hue for current pixel only using relevant surrounding pixels
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      blue_sum_x = green_sum_x = red_sum_x = 0;
      blue_sum_y = green_sum_y = red_sum_y = 0;

      k = (i == 0) ? 0 : -1;
      end_k = (i == height - 1) ? 0 : 1;

      while (k <= end_k)
      {
        l = (j == 0) ? 0 : -1;
        end_l = (j == width - 1) ? 0 : 1;

        while (l <= end_l)
        {
          if (l == -1)
          {
            if (k == -1 || k == 1)
            {
              blue_sum_x += -1 * image[i + k][j + l].rgbtBlue;
              green_sum_x += -1 * image[i + k][j + l].rgbtGreen;
              red_sum_x += -1 * image[i + k][j + l].rgbtRed;
            }

            if (k == 0)
            {
              blue_sum_x += -2 * image[i + k][j + l].rgbtBlue;
              green_sum_x += -2 * image[i + k][j + l].rgbtGreen;
              red_sum_x += -2 * image[i + k][j + l].rgbtRed;
            }
          }

          if (l == 1)
          {
            if (k == -1 || k == 1)
            {
              blue_sum_x += image[i + k][j + l].rgbtBlue;
              green_sum_x += image[i + k][j + l].rgbtGreen;
              red_sum_x += image[i + k][j + l].rgbtRed;
            }

            if (k == 0)
            {
              blue_sum_x += 2 * image[i + k][j + l].rgbtBlue;
              green_sum_x += 2 * image[i + k][j + l].rgbtGreen;
              red_sum_x += 2 * image[i + k][j + l].rgbtRed;
            }
          }

          if (k == -1)
          {

            if (l == -1 || l == 1)
            {
              blue_sum_y += -1 * image[i + k][j + l].rgbtBlue;
              green_sum_y += -1 * image[i + k][j + l].rgbtGreen;
              red_sum_y += -1 * image[i + k][j + l].rgbtRed;
            }

            if (l == 0)
            {
              blue_sum_y += -2 * image[i + k][j + l].rgbtBlue;
              green_sum_y += -2 * image[i + k][j + l].rgbtGreen;
              red_sum_y += -2 * image[i + k][j + l].rgbtRed;
            }
          }

          if (k == 1)
          {

            if (l == -1 || l == 1)
            {
              blue_sum_y += image[i + k][j + l].rgbtBlue;
              green_sum_y += image[i + k][j + l].rgbtGreen;
              red_sum_y += image[i + k][j + l].rgbtRed;
            }

            if (l == 0)
            {
              blue_sum_y += 2 * image[i + k][j + l].rgbtBlue;
              green_sum_y += 2 * image[i + k][j + l].rgbtGreen;
              red_sum_y += 2 * image[i + k][j + l].rgbtRed;
            }
          }
          l++;
        }
        k++;
      }

      // Combine Gx and Gy and store in w_sums for each hue of the current pixel
      w_sum_blue = round(sqrt(pow(blue_sum_x, 2) + pow(blue_sum_y, 2)));
      w_sum_green = round(sqrt(pow(green_sum_x, 2) + pow(green_sum_y, 2)));
      w_sum_red = round(sqrt(pow(red_sum_x, 2) + pow(red_sum_y, 2)));

      w_sums[i][j].rgbtBlue = w_sum_blue > 255 ? 255 : w_sum_blue;
      w_sums[i][j].rgbtGreen = w_sum_green > 255 ? 255 : w_sum_green;
      w_sums[i][j].rgbtRed = w_sum_red > 255 ? 255 : w_sum_red;
    }
  }

  // Overwrite pixels in original image with weighted sums
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      image[i][j] = w_sums[i][j];
    }
  }

  free(w_sums);

  return;
}
