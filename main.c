#include <stdio.h>
#include <stdlib.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>


int main(int argc, char** argv) {


    AVFormatContext* context = avformat_alloc_context();
    int video_stream_index;

    av_register_all();
    avcodec_register_all();
    avformat_network_init();

    //open rtsp
    if(avformat_open_input(&context, "rtsp://192.168.0.6",NULL,NULL) != 0){
        return EXIT_FAILURE;
    }

    if(avformat_find_stream_info(context,NULL) < 0){
        return EXIT_FAILURE;
    }

    //search video stream
    int i;
    for(i =0;i<context->nb_streams;i++){
        if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            video_stream_index = i;
    }

    AVPacket packet;
    av_init_packet(&packet);

    //open output file
    AVOutputFormat* fmt = av_guess_format(NULL,"test2.avi",NULL);
    AVFormatContext* oc = avformat_alloc_context();
    oc->oformat = fmt;
    avio_open2(&oc->pb, "test.avi", AVIO_FLAG_WRITE,NULL,NULL);

    AVStream* stream=NULL;
    int cnt = 0;
    //start reading packets from stream and write them to file
printf("Gonna play\n");

    av_read_play(context);//play RTSP
printf("Playing\n");
    while(av_read_frame(context,&packet)>=0 && cnt <100){//read 100 frames
printf("In loop, frame=%d\n", cnt);
        if(packet.stream_index == video_stream_index){//packet is video               
            if(stream == NULL){//create stream in file
                stream = avformat_new_stream(oc,context->streams[video_stream_index]->codec->codec);
                avcodec_copy_context(stream->codec,context->streams[video_stream_index]->codec);
                stream->sample_aspect_ratio = context->streams[video_stream_index]->codec->sample_aspect_ratio;
                avformat_write_header(oc,NULL);
            }
            packet.stream_index = stream->id;

            av_write_frame(oc,&packet);
            cnt++;
        }
        av_free_packet(&packet);
        av_init_packet(&packet);
    }
    av_read_pause(context);
    av_write_trailer(oc);
    avio_close(oc->pb);
    avformat_free_context(oc);

    return (EXIT_SUCCESS);
}
