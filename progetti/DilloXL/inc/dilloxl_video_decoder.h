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
#ifndef dilloxl_video_decoder_h
#define dilloxl_video_decoder_h

#include "dilloxl_system.h"
#include <SFML/Graphics.hpp>

namespace dilloxl {
  class VideoDecoder {
  public:
    static VideoDecoder& Get();
    VideoDecoder();
    VideoDecoder(const VideoDecoder&) = delete;
    VideoDecoder(VideoDecoder&&) noexcept;
    VideoDecoder& operator=(const VideoDecoder&) = delete;
    VideoDecoder& operator=(VideoDecoder&&) noexcept;
   ~VideoDecoder();
    void feed(const uint8_t*, size_t);
    bool hasFrame() const;
    sf::Image nextFrame() const;
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_video_decoder_h
