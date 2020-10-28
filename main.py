import cv2
import argparse
import numpy as np

# function to get the output layer names 
# in the architecture
def get_output_layers(net):
    
    layer_names = net.getLayerNames()
    
    output_layers = [layer_names[i[0] - 1] for i in net.getUnconnectedOutLayers()]

    return output_layers

# function to draw bounding box on the detected object with class name
def draw_bounding_box(img, class_id, confidence, x, y, x_plus_w, y_plus_h, classes, COLORS):

    label = str(classes[class_id])

    color = COLORS[class_id]

    cv2.rectangle(img, (x,y), (x_plus_w,y_plus_h), color, 2)

    cv2.putText(img, label, (x-10,y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

def arg_configuration():
    # handle command line arguments
    ap = argparse.ArgumentParser()

    input_group = ap.add_mutually_exclusive_group(required=True)

    input_group.add_argument('-i', '--image', 
                    help = 'path to input image')
    input_group.add_argument('-wc', '--webcam', 
                    help = 'path to your webcam, 0 is mostly valid')


    ap.add_argument('-c', '--config', required=True,
                    help = 'path to yolo config file')
    ap.add_argument('-w', '--weights', required=True,
                    help = 'path to yolo pre-trained weights')
    ap.add_argument('-cl', '--classes', required=True,
                    help = 'path to text file containing class names')
    args = ap.parse_args()

    return args

def do_image_processing(net,classes,COLORS, imagePassed, isFromWebCam):
    """
    docstring
    """
    # read input image
    image = cv2.imread(imagePassed) if not isFromWebCam else imagePassed

    Width = image.shape[1]
    Height = image.shape[0]
    scale = 0.00392



    # create input blob 
    blob = cv2.dnn.blobFromImage(image, scale, (416,416), (0,0,0), True, crop=False)

    # set input blob for the network
    net.setInput(blob)



    # run inference through the network
    # and gather predictions from output layers
    outs = net.forward(get_output_layers(net))

    # initialization
    class_ids = []
    confidences = []
    boxes = []
    conf_threshold = 0.5
    nms_threshold = 0.4

    # for each detetion from each output layer 
    # get the confidence, class id, bounding box params
    # and ignore weak detections (confidence < 0.5)
    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]
            if confidence > 0.5:
                center_x = int(detection[0] * Width)
                center_y = int(detection[1] * Height)
                w = int(detection[2] * Width)
                h = int(detection[3] * Height)
                x = center_x - w / 2
                y = center_y - h / 2
                class_ids.append(class_id)
                confidences.append(float(confidence))
                boxes.append([x, y, w, h])
    # apply non-max suppression
    indices = cv2.dnn.NMSBoxes(boxes, confidences, conf_threshold, nms_threshold)

    # go through the detections remaining
    # after nms and draw bounding box
    for i in indices:
        i = i[0]
        box = boxes[i]
        x = box[0]
        y = box[1]
        w = box[2]
        h = box[3]
        
        draw_bounding_box(image, class_ids[i], confidences[i], round(x), round(y), round(x+w), round(y+h), classes, COLORS)

    # display output image    
    cv2.imshow("object detection", image)

    # wait until any key is pressed
    if not isFromWebCam: 
        cv2.waitKey() 


def do_video_processing(net,classes,COLORS, webCamPassed, doDetection):
    """
    docstring
    """
    cap= cv2.VideoCapture(webCamPassed)
    while True:        
        # Capture frame-by-frame
        ret, frame = cap.read()
        if ret==False:
            break
        if not doDetection:
            # Display the resulting frame
            cv2.imshow('frame',frame)
        else:
            #process the resulting frame
            do_image_processing(net,classes,COLORS,frame,True)

        #allowing the programmer to quit the process by pressing q
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

def main():
    args= arg_configuration()

    # reading classes names 
    classes = None
    with open(args.classes, 'r') as f:
        classes = [line.strip() for line in f.readlines()]

    # generating colors for each of classes within the class file
    COLORS = np.random.uniform(0, 255, size=(len(classes), 3))

    # reading weights and its corresponding config file
    net = cv2.dnn.readNet(args.weights, args.config)




    if args.webcam:
        #start processing the frames of your webcam, pass true to the doDetection variable
        #for detecting objects within each frame, false to output the the camera captured content
        do_video_processing(net,classes,COLORS,int(args.webcam),doDetection=False)
    elif args.image:
        do_image_processing(net,classes,COLORS,args.image,False)
    else:
        raise Exception("Check your command line arguments")
    # release resources
    cv2.destroyAllWindows()

main()