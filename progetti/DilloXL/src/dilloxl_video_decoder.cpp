/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) 2024 Copyright by Michele Iacobellis
 * A project for students...
 * 
 * This file is part of DILLOXL.
 *
 * DILLOXL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DILLOXL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DILLOXL. If not, see <http://www.gnu.org/licenses/>.
 * 
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_video_decoder.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <mutex>
#include <queue>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * SPECIAL INCLUDES
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
extern "C" {
#include <libavcodec/avcodec.h>
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static dilloxl::VideoDecoder* gpVideoDecoder = nullptr;
dilloxl::VideoDecoder& dilloxl::VideoDecoder::Get()
{
  DILLOXL_CAPTURE_CPU(nullptr == gpVideoDecoder, "Puntatore NULL");
  return *gpVideoDecoder;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATIONS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::VideoDecoder::Impl {
public:
  static const size_t szBSZ = 4096 + AV_INPUT_BUFFER_PADDING_SIZE;
  using Buffer = std::pair<uint8_t[szBSZ], size_t>;
  Impl();
 ~Impl();

  std::thread m_threadDec;
  std::mutex m_mtxQueue;
  std::queue<Buffer> m_qBuf;
  size_t m_szExitThread;
  size_t m_szThreadStarted;
  std::mutex m_mtxImgQueue;
  std::queue<sf::Texture> m_qImg;

  // ffmpeg related stuff
        const AVCodec* m_pCodec;
  AVCodecParserContext* m_pCodecParserCtx;
        AVCodecContext* m_pCodecCtx;
               AVFrame* m_pFrame;
              AVPacket* m_pPkt;

  void m_InitDecoder();
  void m_FiniDecoder();
  void m_Decode(const uint8_t*, size_t);
  void m_BuildFrame();
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::VideoDecoder::VideoDecoder()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
  gpVideoDecoder = this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::VideoDecoder::VideoDecoder(VideoDecoder&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::VideoDecoder& 
  dilloxl::VideoDecoder::operator=(VideoDecoder&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::VideoDecoder::~VideoDecoder()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::VideoDecoder::feed(const uint8_t* pData, size_t szSizeInBytes)
{
  std::fprintf(stderr
    , DILLOXL_TERM_FGDIMM 
      "[DILLOXL]: Dati video %zubytes inviati al thread decoder..."
      DILLOXL_TERM_RESETA "\n", szSizeInBytes);
  
  Impl::Buffer buf;
  DILLOXL_CAPTURE_CPU(szSizeInBytes >= sizeof(buf.first)
    , "I dati video hanno dimensione maggiore di quella del buffer");

  { std::lock_guard<std::mutex> guard{ m_pImpl->m_mtxQueue };
    ::memcpy(buf.first, pData, szSizeInBytes);
    buf.second = szSizeInBytes;
    m_pImpl->m_qBuf.push(buf);
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::VideoDecoder::hasFrame() const
{
  std::lock_guard<std::mutex> guard{ m_pImpl->m_mtxImgQueue };
  return !m_pImpl->m_qImg.empty();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
sf::Texture dilloxl::VideoDecoder::nextFrame() const
{
  std::lock_guard<std::mutex> guard{ m_pImpl->m_mtxImgQueue };
  auto ret = m_pImpl->m_qImg.front(); m_pImpl->m_qImg.pop();
  return ret;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::VideoDecoder::Impl::Impl()
: m_szExitThread    {       0 }
, m_szThreadStarted {       0 }
, m_pCodec          { nullptr }
, m_pCodecParserCtx { nullptr }
, m_pCodecCtx       { nullptr }
, m_pFrame          { nullptr }
, m_pPkt            { nullptr }
{
  m_InitDecoder();
  m_threadDec = std::thread([=]() {
    m_szThreadStarted = 1;
    Buffer buf; bool bHasData = false;
    while (0 == m_szExitThread) {
      { std::lock_guard<std::mutex> guard{ m_mtxQueue };
        if (!m_qBuf.empty()) {
          const auto& out = m_qBuf.front(); m_qBuf.pop();
          if (out.second < sizeof(buf.first)) {
            ::memcpy(buf.first, out.first, out.second);
            buf.second = out.second;
            bHasData = true;
          } else {
            bHasData = false;
            std::fprintf(stderr
              , DILLOXL_TERM_BLKRED
                "[DILLOXL]: Dati video in eccesso !"
                DILLOXL_TERM_RESETA "\n");
          }
        } else {
          bHasData = false;
        }
      }

      if (bHasData) {
        std::fprintf(stderr
          , DILLOXL_TERM_FGDIMM 
            "[DILLOXL]: Dati video %zubytes presi dal thread decoder..."
            DILLOXL_TERM_RESETA "\n", buf.second);
        m_Decode(buf.first, buf.second);
      }
    }
    m_szThreadStarted = 0;
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::VideoDecoder::Impl::~Impl()
{
  m_szExitThread = 1;
  if (m_threadDec.joinable()) { m_threadDec.join(); }
  m_FiniDecoder();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::VideoDecoder::Impl::m_InitDecoder()
{
  m_pPkt = av_packet_alloc();
  if (nullptr == m_pPkt) {

  }

  m_pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
  if (nullptr == m_pCodec) {

  }

  m_pCodecParserCtx = av_parser_init(m_pCodec->id);
  if (nullptr ==m_pCodecParserCtx) {

  }

  m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
  if (nullptr == m_pCodecCtx) {

  }

  /* For some codecs, such as msmpeg4 and mpeg4, width and height
    MUST be initialized there because this information is not
    available in the bitstream. */

  if (avcodec_open2(m_pCodecCtx, m_pCodec, nullptr) < 0) {

  }

  m_pFrame = av_frame_alloc();
  if (nullptr == m_pFrame) {

  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::VideoDecoder::Impl::m_FiniDecoder()
{
  m_BuildFrame();

  // ffmpeg related stuff
       av_parser_close(m_pCodecParserCtx);
  avcodec_free_context(&m_pCodecCtx);
         av_frame_free(&m_pFrame);
        av_packet_free(&m_pPkt);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::VideoDecoder::Impl::m_Decode(const uint8_t* pData, size_t sz)
{
  size_t szDataSize = sz;
  while (szDataSize > 0) {
    auto ret = av_parser_parse2(m_pCodecParserCtx, m_pCodecCtx
      , &m_pPkt->data, &m_pPkt->size, pData, szDataSize
      , AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
    if (ret < 0) {
      fprintf(stderr, "Errore durante l'analisi del flusso video\n");
    } else {
      pData      += ret;
      szDataSize -= ret;
    }
    if (m_pPkt->size) { m_BuildFrame(); }
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::VideoDecoder::Impl::m_BuildFrame()
{
  auto ret = avcodec_send_packet(m_pCodecCtx, m_pPkt);
  if (ret < 0) {
    fprintf(stderr, "Problema nell'invio del dato video ad AVCODEC\n");
  } else {
    while (ret >= 0) {
      ret = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
      if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        return;
      }
      else if (ret < 0) {
        fprintf(stderr, "Errore durante la decodifica!\n");
      } else {
        /*
          m_pCodecCtx->frame_num;
          m_pFrame->data[0];
          m_pFrame->linesize[0];
          m_pFrame->width;
          m_pFrame->height;
        */
#if 0       
        sf::Image image({ 
            uint32_t(m_pFrame->width), uint32_t(m_pFrame->height) }
          , m_pFrame->data[0]);
        { std::lock_guard<std::mutex> guard{ m_mtxImgQueue };
          m_qImg.push(sf::Texture{ image });
        }
#endif        
      }
    }
  }
}

#if 0
static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;

    f = fopen(filename,"wb");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}
#endif
