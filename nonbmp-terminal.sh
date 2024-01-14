#!/usr/bin/env bash
ffmpeg -i '/mnt/c/Users/nooriro/Videos/Captures/명령 프롬프트 2024-01-14 22-04-41.mp4' -t '00:00:20' -vf 'crop=1120:832:0:0' -lossless 0 -loop 0 /mnt/c/Users/nooriro/Downloads/test-crt-nonbmp/nonbmp-terminal.webp

# $ ffmpeg -h full | nl -ba | grep webp
#   2248  libwebp_anim AVOptions:
#   2262  libwebp AVOptions:
#   4444  webp demuxer AVOptions:
# $ ffmpeg -h full | nl -ba | tail -n +2248 | head -n 28
#   2248  libwebp_anim AVOptions:
#   2249    -lossless          <int>        E..V..... Use lossless mode (from 0 to 1) (default 0)
#   2250    -preset            <int>        E..V..... Configuration preset (from -1 to 5) (default none)
#   2251       none                         E..V..... do not use a preset
#   2252       default                      E..V..... default preset
#   2253       picture                      E..V..... digital picture, like portrait, inner shot
#   2254       photo                        E..V..... outdoor photograph, with natural lighting
#   2255       drawing                      E..V..... hand or line drawing, with high-contrast details
#   2256       icon                         E..V..... small-sized colorful images
#   2257       text                         E..V..... text-like
#   2258    -cr_threshold      <int>        E..V..... Conditional replenishment threshold (from 0 to INT_MAX) (default 0)
#   2259    -cr_size           <int>        E..V..... Conditional replenishment block size (from 0 to 256) (default 16)
#   2260    -quality           <float>      E..V..... Quality (from 0 to 100) (default 75)
#   2261
#   2262  libwebp AVOptions:
#   2263    -lossless          <int>        E..V..... Use lossless mode (from 0 to 1) (default 0)
#   2264    -preset            <int>        E..V..... Configuration preset (from -1 to 5) (default none)
#   2265       none                         E..V..... do not use a preset
#   2266       default                      E..V..... default preset
#   2267       picture                      E..V..... digital picture, like portrait, inner shot
#   2268       photo                        E..V..... outdoor photograph, with natural lighting
#   2269       drawing                      E..V..... hand or line drawing, with high-contrast details
#   2270       icon                         E..V..... small-sized colorful images
#   2271       text                         E..V..... text-like
#   2272    -cr_threshold      <int>        E..V..... Conditional replenishment threshold (from 0 to INT_MAX) (default 0)
#   2273    -cr_size           <int>        E..V..... Conditional replenishment block size (from 0 to 256) (default 16)
#   2274    -quality           <float>      E..V..... Quality (from 0 to 100) (default 75)
#   2275
# $ ffmpeg -h full | nl -ba | tail -n +4444 | head -n 7
#   4444  webp demuxer AVOptions:
#   4445    -frame_size        <int>        .D....... force frame size in bytes (from 0 to INT_MAX) (default 0)
#   4446    -framerate         <video_rate> .D....... set the video framerate (default "25")
#   4447    -pixel_format      <string>     .D....... set video pixel format
#   4448    -video_size        <image_size> .D....... set video size
#   4449    -loop              <boolean>    .D....... force loop over input file sequence (default false)
#   4450

# $ ffmpeg -h full | nl -ba | grep dpi
#    747    -dpi               <int>        E..V..... Set image resolution (in dots per inch) (from 0 to 65536) (default 0)
#   1498    -dpi               <int>        E..V..... Set image resolution (in dots per inch) (from 0 to 65536) (default 0)
#   1691    -dpi               <int>        E..V..... set the image resolution (in dpi) (from 1 to 65536) (default 72)
# $ ffmpeg -h full | nl -ba | tail -n +746 | head -n 11
#    746  APNG encoder AVOptions:
#    747    -dpi               <int>        E..V..... Set image resolution (in dots per inch) (from 0 to 65536) (default 0)
#    748    -dpm               <int>        E..V..... Set image resolution (in dots per meter) (from 0 to 65536) (default 0)
#    749    -pred              <int>        E..V..... Prediction method (from 0 to 5) (default none)
#    750       none                         E..V.....
#    751       sub                          E..V.....
#    752       up                           E..V.....
#    753       avg                          E..V.....
#    754       paeth                        E..V.....
#    755       mixed                        E..V.....
#    756
# $ ffmpeg -h full | nl -ba | tail -n +1497 | head -n 11
#   1497  PNG encoder AVOptions:
#   1498    -dpi               <int>        E..V..... Set image resolution (in dots per inch) (from 0 to 65536) (default 0)
#   1499    -dpm               <int>        E..V..... Set image resolution (in dots per meter) (from 0 to 65536) (default 0)
#   1500    -pred              <int>        E..V..... Prediction method (from 0 to 5) (default none)
#   1501       none                         E..V.....
#   1502       sub                          E..V.....
#   1503       up                           E..V.....
#   1504       avg                          E..V.....
#   1505       paeth                        E..V.....
#   1506       mixed                        E..V.....
#   1507
# $ ffmpeg -h full | nl -ba | tail -n +1690 | head -n 8
#   1690  TIFF encoder AVOptions:
#   1691    -dpi               <int>        E..V..... set the image resolution (in dpi) (from 1 to 65536) (default 72)
#   1692    -compression_algo  <int>        E..V..... (from 1 to 32946) (default packbits)
#   1693       packbits                     E..V.....
#   1694       raw                          E..V.....
#   1695       lzw                          E..V.....
#   1696       deflate                      E..V.....
#   1697
