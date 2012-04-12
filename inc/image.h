#ifndef IMAGE_H
#define IMAGE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define TOP_LEFT 0
#define BOTTOM_LEFT 1

class Image {

	public:
		Image(IplImage *cvImage);
		Image(const char *filePath);
		IplImage *cvImage();
		void setCvImage(IplImage *cvImage);
		void release();
		bool isValid();
		Image *clone();
		Image *cloneJustDimensions(int num_channels = -1, int depth = -1);
		void cloneTo(
			Image *anotherImage,
			Image *mask = NULL
		);
		void splitTo(
			Image *channel1,
			Image *channel2,
			Image *channel3
		);
		void flip();
		void resizeLike(Image *otherImage);
		int originPosition();
		void setOriginPosition(int position);
		void storeDistorsionWith(Image *anotherImage, Image *toStore);
		Image *distorsionWith(Image *anotherImage);
		void storeDifferenceWith(Image *anotherImage, Image *toStore);
		Image *differenceWith(Image *anotherImage);
		Image *mergeToMaximumWith(Image *anotherImage);
		Image *mergeChannelsToMaximum();
		void mergeToMaximumWithAndStore(Image *anotherImage, Image *store);
		void mergeChannelsToMaximumAndStore(Image *store);
		void mergeChannels(
			Image *store, 
			float weight_channel1, 
			float weight_channel2, 
			float weight_channel3
		);

		void binarize(int threshold = 40);
		void negativize();
		Image *cleanIsolatedDots();
		Image *duplicate();

	private:
		IplImage *_cvImage;

};

#endif
