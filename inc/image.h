#ifndef IMAGE_H
#define IMAGE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define TOP_LEFT 0
#define BOTTOM_LEFT 1

class Image {

	public:
		Image();
		Image(IplImage *cvImage);
		Image(const char *filePath);

		IplImage *getInput();
		int getOriginPosition();
		bool isValid();

		void setInput(IplImage *cvImage);
		void setInput(const char *filePath);

		Image *clone();
		Image *cloneJustDimensions(int num_channels = -1, int depth = -1);
		Image *distorsionWith(Image *anotherImage);
		Image *differenceWith(Image *anotherImage);
		Image *mergeToMaximumWith(Image *anotherImage);
		Image *mergeChannelsToMaximum();
		Image *cleanIsolatedDots();
		Image *duplicate();

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
		void setOriginPosition(int position);
		void storeDistorsionWith(Image *anotherImage, Image *toStore);
		void storeDifferenceWith(Image *anotherImage, Image *toStore);
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

		void release();

	private:
		IplImage *_input;

};

#endif
