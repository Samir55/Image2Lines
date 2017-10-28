# Image 2 Char

This is the **first module** of converting handwritten text image to code.

We'll use Histogram approach for line segmentation, proposed in [this paper](http://www.cvc.uab.es/icdar2009/papers/3725a651.pdf) by Rodolfo P. dos Santos, Gabriela S. Clemente, Tsang Ing Ren and George D.C.

## Steps
### I. Line Segmentation
* Preprocessing: Noise reduction and binarization.
* Y Histogram projection for line detection.
* False line exclusion.
* Repair the lines.
### II. Word Segmentation
### III. Char Segmentation
