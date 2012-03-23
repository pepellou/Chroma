#include <stdio.h>
#include <stdlib.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>

int open_stream(
	AVFormatContext *context,
	char *url
) {
	if ((avformat_open_input(&context, url, NULL, NULL) != 0) || (avformat_find_stream_info(context, NULL) < 0))
		return EXIT_FAILURE;
	return 0;
}

AVFormatContext *initialize_av() {
	AVFormatContext* context = avformat_alloc_context();
	av_register_all();
	avcodec_register_all();
	avformat_network_init();
	return context;
}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
	FILE *pFile;
	char szFilename[32];
	int  y;

	sprintf(szFilename, "frame%d.ppm", iFrame);
	pFile = fopen(szFilename, "wb");
	if (pFile==NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	for (y=0; y<height; y++)
		fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

	fclose(pFile);
}

int main(int argc, char** argv) {
	AVFormatContext* context = initialize_av();

	if (open_stream(context, "rtsp://192.168.0.6:554") == EXIT_FAILURE)
		return EXIT_FAILURE;

	//search video stream
	AVCodecContext *pCodecCtx;
	    int i;
	    int video_stream_index;
	    for(i =0;i<context->nb_streams;i++){
		if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		    video_stream_index = i;
	    }
	pCodecCtx=context->streams[video_stream_index]->codec;

	int frameFinished;

	AVPacket packet;
	av_init_packet(&packet);

	AVFrame *pFrame;
	AVFrame *pFrameRGB;

	pFrame=avcodec_alloc_frame();
	pFrameRGB=avcodec_alloc_frame();
	if (pFrameRGB==NULL)
		return -1;


	AVOutputFormat* fmt = av_guess_format(NULL, "test2.avi", NULL);
	AVFormatContext* oc = avformat_alloc_context();
	oc->oformat = fmt;
	avio_open2(&oc->pb, "test.avi", AVIO_FLAG_WRITE, NULL, NULL);

	AVStream* stream=NULL;
	int cnt = 0;

    while(av_read_frame(context, &packet)>=0 && cnt <100){//read 100 frames
        if(packet.stream_index == video_stream_index){//packet is video               
printf("PACKET %d (pts, dts, size): %d %d %d\n", cnt, packet.pts, packet.dts, packet.size);
            if(stream == NULL){//create stream in file
                stream = avformat_new_stream(oc, context->streams[video_stream_index]->codec->codec);
                avcodec_copy_context(stream->codec, context->streams[video_stream_index]->codec);
                stream->sample_aspect_ratio = context->streams[video_stream_index]->codec->sample_aspect_ratio;
                avformat_write_header(oc, NULL);
            }
            packet.stream_index = stream->id;

            av_write_frame(oc, &packet);
            cnt++;

		// Decode video frame
/*
		avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
				 &packet);
*/

		// Did we get a video frame?
		if(frameFinished) {
		// Convert the image from its native format to RGB
/*
		img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24, 
		    (AVPicture*)pFrame, pCodecCtx->pix_fmt, 
				pCodecCtx->width, pCodecCtx->height);
 SwsContext* convert_ctx; 
                     convert_ctx = sws_getContext( pCodecCtx->width, 
                                                   pCodecCtx->height, 
                                                   pCodecCtx->pix_fmt, 
                                                   pCodecCtx->width, 
                                                   pCodecCtx->height, 
                                                   PIX_FMT_RGB24, 
                                                   SWS_BILINEAR, NULL, 
NULL, NULL); 
		sws_scale(convert_ctx, pFrame->data, pFrame->linesize, 0, 
                               pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
*/

		// Save the frame to disk
/*
		  SaveFrame(pFrame, pCodecCtx->width, 
			    pCodecCtx->height, cnt);
*/
		}
	}
        av_free_packet(&packet);
        av_init_packet(&packet);
    }
    av_write_trailer(oc);
    avio_close(oc->pb);
    avformat_free_context(oc);

// Free the RGB image
av_free(pFrameRGB);

// Free the YUV frame
av_free(pFrame);

    return (EXIT_SUCCESS);
}
