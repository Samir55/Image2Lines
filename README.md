# Image 2 Lines

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/b06e28bb75234b76838218c1cad86568)](https://www.codacy.com/app/Samir55/Image2Lines?utm_source=github.com&utm_medium=referral&utm_content=Samir55/Image2Lines&utm_campaign=badger)

This is a module of our project for image processing course at Cairo university.

**This is a working in progress project**.

**The implementation of this tool is from the following paper** "A Statistical approach to line segmentation in handwritten documents" Manivannan Arivazhagan, Harish Srinivasan and Sargur Srihari. 

  It can be found at [this link](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.88.5806&rep=rep1&type=pdf).

This tool converts handwritten text image (including straight and skewed lines) to text. Afterwards, It'll afterwards integrated with a machine learning tool to detect characters.

The main aim of this project is to compile and execute (try to repair compilation errors too! :) ) code handwritten on paper

## Steps
*  OTSU Threshold and Binarization.
* Get initial candidate lines using y-axis histogram projection profile and using adaptive threshold between valleys.
These are examples of the output produced from the current code.
![](https://i.imgur.com/961QuKN.jpg)
![](https://i.imgur.com/ZdAIQeO.jpg)

* Apply line drawing algorithm and make decissions about regions that hit the lines using Bivariate Gaussian Distribution.
These is example of the output produced from the current code.
![](https://i.imgur.com/GMqsSxc.jpg)

# Dependencies
* OpenCV: 3.0 or newer.

# Usage as an exe
```Console
mkdir build
cd build
cmake ..
make
./Image2Lines the_path_to_the_img
```
the output will be:
* N final lines, each line is named 'line_i.jpg' where i is the line number.
* 'Initial_lines.jpg' : representing the initial lines drawn on the original image.
* 'Final_lines.jpg' : representing the final lines drawn on the original image.
* 'Contours.jpg' : represnting the detected contours.

# Usage in your code
You should add LineSegmentation.cpp and LineSegmentation.hpp to your project.
```C++
    #include "LineSegmentation.hpp"
    
    // Create an object from LineSegmentation and give to it the image path
    LineSegmentation line_segmentation(img_path);
    
    // Run segment() method to segment lines. The output will be a vector of CV::Mat each representing a line.
    vector<cv::Mat> lines = line_segmentation.segment();
    
```
**NOTE** You should link correctly OpenCV in your working project. if you use CMake see CMakeLists.txt file to see how to add opencv.

# Development
# Build 
```Console
mkdir build
cd build
cmake ..
make
```
# Current issues
* Finding peaks and valleys can cause in some cases the lines to be falsely segmented. If you can improve that problem, feel free to make a pull request.

# TODOs
* Improve detecting peaks and valleys function.
* Use a better binarization technique than the currrently used (OTSU) in order to get better binarization in case of camera images. (I will use my other repository named Binarization).

## Database used
IAM database (Educational use permit).
