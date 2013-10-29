#!/bin/bash
#The order of the .cl files seems to be important in order to generate the same output as the standard Makefile build
#That said I cant decipher exactly where the order is coming from so its hardcoded for the moment
#If it turns out the order is not actually needed then a simple "../common/opencl/*.cl" will suffice
cat ../common/opencl/x264-cl.h ../common/opencl/bidir.cl ../common/opencl/downscale.cl ../common/opencl/weightp.cl ../common/opencl/intra.cl ../common/opencl/subpel.cl ../common/opencl/motionsearch.cl | perl ../tools/cltostr.pl x264_opencl_source > ../common/oclobj.h