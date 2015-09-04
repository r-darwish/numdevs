# numdevs
Devices for all numbers.

While the zero byte has its own device, there are 255 other byte values left without a roof. devnums is a kernel module that comes to fix this injustice by creating `/dev/one`, `/dev/two`, `/dev/three`, and so on up until `/dev/two_hundred_and_fifty_five`. All of these new devices can be used just like `/dev/zero`.

## Build
Clone this repository, then just type the `make` command. If everthing went well then you can type `insmod kmod/numdevs.ko` and play with your 255 new devices.

## A word of caution
This driver has not been test thoroughly. When dealing with your new devices do not touch the operational end of The Device. Do not look directly at the operational end of The Device. Do not submerge The Device in liquid, even partially. Most importantly, under no circumstances should you
