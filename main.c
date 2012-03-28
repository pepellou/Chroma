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
	if (pFile == NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	for (y = 0; y < height; y++)
		fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

	fclose(pFile);
}

static int64_t my_guess_correct_pts(AVCodecContext *ctx,
                                 int64_t reordered_pts, int64_t dts)
{
    int64_t pts = AV_NOPTS_VALUE;

    if (dts != AV_NOPTS_VALUE) {
        ctx->pts_correction_num_faulty_dts += dts <= ctx->pts_correction_last_dts;
        ctx->pts_correction_last_dts = dts;
    }
    if (reordered_pts != AV_NOPTS_VALUE) {
        ctx->pts_correction_num_faulty_pts += reordered_pts <= ctx->pts_correction_last_pts;
        ctx->pts_correction_last_pts = reordered_pts;
    }
    if ((ctx->pts_correction_num_faulty_pts<=ctx->pts_correction_num_faulty_dts || dts == AV_NOPTS_VALUE)
       && reordered_pts != AV_NOPTS_VALUE)
        pts = reordered_pts;
    else
        pts = dts;

    return pts;
}

int my_avcodec_decode_video2(
	AVCodecContext *avctx, 
	AVFrame *picture,
	int *got_picture_ptr,
	AVPacket *avpkt
) {
	int ret;
	*got_picture_ptr= 0;
	if((avctx->coded_width||avctx->coded_height) && av_image_check_size(avctx->coded_width, avctx->coded_height, 0, avctx))
		return -1;
	avctx->pkt = avpkt;
	int cond1 = (avctx->codec->capabilities & CODEC_CAP_DELAY);
	int cond2 = avpkt->size;
	int cond3 = (avctx->active_thread_type&FF_THREAD_FRAME);
	int condition = cond1 || cond2 || cond3;
	if(condition){
		ret = avctx->codec->decode(avctx, picture, got_picture_ptr, avpkt);
		picture->pkt_dts= avpkt->dts;
		if (*got_picture_ptr){
			avctx->frame_number++;
			picture->best_effort_timestamp = my_guess_correct_pts(avctx,
				picture->pkt_pts,
				picture->pkt_dts);
		}
	}else
		ret= 0;
	return ret;
}

static AVFrame *alloc_picture(enum PixelFormat pix_fmt, int width, int height) {
	AVFrame *picture;
	uint8_t *picture_buf;
	int size;

	picture = avcodec_alloc_frame();
	if (!picture)
		return NULL;
	size = avpicture_get_size(pix_fmt, width, height);
	picture_buf = av_malloc(size);
	if (!picture_buf) {
		av_free(picture);
		return NULL;
	}
	avpicture_fill((AVPicture *) picture, picture_buf, pix_fmt, width, height);
	return picture;
}


int main(int argc, char** argv) {
	AVFormatContext* context = initialize_av();

	if (open_stream(context, "rtsp://192.168.0.6:554") == EXIT_FAILURE)
		return EXIT_FAILURE;

	//search video stream
	AVCodecContext *pCodecCtx;
	int i;
	int video_stream_index;
	for (i = 0; i < context->nb_streams; i++) {
		if (context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			video_stream_index = i;
	}
	pCodecCtx = context->streams[video_stream_index]->codec;

AVCodec *pCodec;

// Find the decoder for the video stream
pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
if(pCodec==NULL) {
  fprintf(stderr, "Unsupported codec!\n");
  return -1; // Codec not found
}
// Open codec
if(avcodec_open(pCodecCtx, pCodec)<0)
  return -1; // Could not open codec

	int frameFinished;

	AVPacket packet;
	av_init_packet(&packet);

	AVFrame *pFrame;
	AVFrame *pFrameRGB;

	pFrame = avcodec_alloc_frame();
	pFrameRGB = avcodec_alloc_frame();
	if (pFrameRGB == NULL)
		return -1;


	AVOutputFormat* fmt = av_guess_format(NULL, "test2.avi", NULL);
	AVFormatContext* oc = avformat_alloc_context();
	oc->oformat = fmt;
	avio_open2(&oc->pb, "test.avi", AVIO_FLAG_WRITE, NULL, NULL);

	AVStream* stream = NULL;
	int cnt = 0;

	while (av_read_frame(context, &packet) >= 0 && cnt < 100) {
		if (packet.stream_index == video_stream_index) {
			printf("PACKET %d (pts, dts, size): %d %d %d\n", cnt, packet.pts, packet.dts, packet.size);
			if (stream == NULL) {
				stream = avformat_new_stream(oc, context->streams[video_stream_index]->codec->codec);
				avcodec_copy_context(stream->codec, context->streams[video_stream_index]->codec);
				stream->sample_aspect_ratio = context->streams[video_stream_index]->codec->sample_aspect_ratio;
				avformat_write_header(oc, NULL);
			}
			packet.stream_index = stream->id;
			av_write_frame(oc, &packet);
			cnt++;

			AVFrame *pF;
			pF = avcodec_alloc_frame();
			AVFrame *pFRGB;
			pFRGB=avcodec_alloc_frame();
/*
			if (pFRGB==NULL)
				return -1;
			avpicture_fill((AVPicture *)pFRGB, buf, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
				SaveFrame(
					picture, 
					pCodecCtx->width, 
					pCodecCtx->height, 
					cnt
				);
*/

				uint8_t *buf;
				int nB;
				nB=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
				buf=(uint8_t *)av_malloc(nB*sizeof(uint8_t));
				AVPacket avpkt;
				av_init_packet(&avpkt);
				avpkt.data = buf;
				avpkt.size = nB;
				avpkt.flags = AV_PKT_FLAG_KEY;
				my_avcodec_decode_video2(pCodecCtx, pF, &frameFinished, &avpkt);

			if (frameFinished) {
/*
				img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24, 
					(AVPicture*)pFrame, pCodecCtx->pix_fmt, 
					pCodecCtx->width, pCodecCtx->height);

*/
				static struct SwsContext *convert_ctx;
				convert_ctx = sws_getContext(
					pCodecCtx->width, 
					pCodecCtx->height, 
					pCodecCtx->pix_fmt, 
					pCodecCtx->width, 
					pCodecCtx->height, 
					PIX_FMT_RGB24, 
					SWS_BILINEAR, 
					NULL, 
					NULL, 
					NULL
				); 
				sws_scale(
					convert_ctx, 
					pF->data, 
					pF->linesize, 
					0, 
					pCodecCtx->height, 
					pFRGB->data, 
					pFRGB->linesize
				);
				SaveFrame(
					pFRGB, 
					pCodecCtx->width, 
					pCodecCtx->height, 
					cnt
				);
			}
		}
		av_free_packet(&packet);
		av_init_packet(&packet);
	}
	av_write_trailer(oc);
	avio_close(oc->pb);
	avformat_free_context(oc);

	av_free(pFrameRGB);
	av_free(pFrame);

	return (EXIT_SUCCESS);
}
