# WIPE DETECTOR
This application finds cuts and wipes in video files.

This project is based on constructing “spatio-temporal” image which contains video content for each frame along the ordinate axis, versus time along the abscissa. A simple version of constructing the image consists of copying a column (or row), or weighted average of a few, directly into the STI(Spatio-Temporal Image).

## STI by Copying Pixels

Copy the center column from each frame into a new matrix to hold the image called STI. The STI has the same number of rows r as does the original video frame, but a number of columns equal to the number of frames f in the video.

Result image shown below

![alt text](https://github.com/berkeliboz/Wipe-Detector/blob/master/images/STI_by_copying_pixels.PNG)


## STI by Histogram Differences

One problem with the method outlined above is how to find that nice edge our eye sees so clearly. Also, if we rely on the pixels themselves, small movements can muddy the STI.

Instead of comparing raw pixel data, comparison using histogram of colors is used as method. Histograms are insensitive to problems such as movement and occlusion. 

If we replace the color RGB space by the chromacity then the image is much more characteristic of the surfaces being imaged, rather than of the
light illuminating those surfaces. 
	

    {r,g} = {R,G}/(R + G + B)

   Created color histogram is in 2D array with R on one axis and G on other with an interval of [0, 1].
   
Now, suppose we wish to compare one column in a frame with the same column in the previous frame. Then we should compare the histogram Ht at time t with the histogram Ht−1 for the previous frame. Here, we make a 2D histogram Ht just for the particular column we’re working on, and the histogram Ht−1 for a histogram for the same column, but for the previous frame. Let’s get a measure of histogram difference. One measure would be the Euclidean distance between all the entries in the histogram: the square root of the sum-of- squares of the differences between entries. But a better value turns out to be the so-called “histogram intersection”:
![alt text](https://github.com/berkeliboz/Wipe-Detector/blob/master/images/Capture.PNG)

Since we compared histograms for a single column, and obtained a scalar I, each column in the whole frame can give us its own scalar, at the current time frame. So an STI made out of I values could have a number of rows equal to the number of columns in the video frame, and number of columns equal to the number of frames.

If one histogram is similar to other, then I value is ~ 1, if else ~ 0.


Result image shown below

![alt text](https://github.com/berkeliboz/Wipe-Detector/blob/master/images/STI_by_histogram_differences.PNG)

