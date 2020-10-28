# This repository features a very basic python script to do object detection as well as a c++ file showing how to capture videos with your webcam and stream the frames

## Requirements .
1. System:
⋅⋅* Opencv3.2 or higher 
⋅⋅* Cmake.

2. The python script requires:
    ``` bash
    $ pip3 install numpy
    ```
3. Get the weights:
    ``` bash
    $ wget https://pjreddie.com/media/files/yolov3-tiny.weights
    ```

    or 

    ``` bash
    $ wget https://pjreddie.com/media/files/yolov3.weights
    ```


### to run and compile the c++ file:
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$./basicOpenCVFunc
```

### the python flags are:
`-i or --image`, meaning you are going to pass an image

or

`-wc or --wecam`, meaning you are going to pass a webcam index

### to run the python script:
```bash
$ python3 main.py -wc=[numberOfYourCam->usually 0] -c=cfg/yolov3.cfg -w=weights/yolov3.weights -cl=data/coco.names
```
or

```bash
$ python3 main.py -i=[pathToYourImage] -c=cfg/yolov3.cfg -w=weights/yolov3.weights -cl=data/coco.names
```