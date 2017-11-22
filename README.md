# Image 2 Lines
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
![](https://i.imgur.com/ZdAIQeO.jpg)
![](https://i.imgur.com/B7Zld79.jpg)
* Apply line drawing algorithm and make decissions about regions that hit the lines using Bivariate Gaussian Distribution.

## Tools used:
We'll use C++ and OpenCV.

## Database used
IAM database.


