#/bin/bash
i=1
for sample in samples/*; do
	opencv_createsamples -img $sample -vec vectors/vector${i}.vec -bg negatives.dat -num 200 -bgcolor 255 -bgthresh 10 -maxxangle 1.1 -maxyangle 1.1 -maxzangle 0.5 -maxidev 100 -w 60 -h 60
	((i++))
done

python mergevec.py -v vectors -o samples.vec
rm -rf vectors
