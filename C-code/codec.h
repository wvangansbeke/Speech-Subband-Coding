#ifndef CODEC_H
#define CODEC_H

#define mu2 8192
#define mu3 16384
#define mu4 4096
#define mu5 32768

#define mu2pow 13
#define mu3pow 14
#define mu4pow 12
#define mu5pow 15

#define windowsize2 8
#define windowsize3 8
#define windowsize4 8
#define windowsize5 1

#define windowsize2pow 3
#define windowsize3pow 4
#define windowsize4pow 3
#define windowsize5pow 0

#define windowsize2dec (windowsize2-1)
#define windowsize3dec (windowsize3-1)
#define windowsize4dec (windowsize4-1)
#define windowsize5dec (windowsize5-1)

#define phi2 8192
#define phi3 16384
#define phi4 65536
#define phi5 32768

#define phi2pow 13
#define phi3pow 14
#define phi4pow 16
#define phi5pow 15

#define min_step 1

#define max_step2 1024
#define max_step3 1024
#define max_step4 8192
#define max_step5 32768

#define z_min2 -16
#define z_min3 -16
#define z_min4 -2
#define z_min5 0

#define z_max2 15
#define z_max3 15
#define z_max4 1
#define z_max5 0

#define DSIZE2 (SIZE2+windowsize2)
#define DSIZE3 (SIZE2+windowsize3)
#define DSIZE4 (SIZE2+windowsize4)
#define DSIZE5 (SIZE2+windowsize5)

#define SSIZE SIZE2+1

#endif // CODEC_H
