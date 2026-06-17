import numpy as np
import os
from PIL import Image as img 
import sys

def Brightness(array,value):

   
    array = array.astype(np.float32)
    if value < 0 :
        array+=value 
    elif value > 0 :
        array+=value
    array = np.clip(array,0,255)
    return array.astype(np.uint8)
#Brightness(50)

def Contrast(array,value):
    
    array = array.astype(np.float32)
    
    mean = np.mean(array)
    factor = 1 + (value / 100)
    array = (array - mean) * factor + mean
    array = np.clip(array,0,255)
    return array.astype(np.uint8)

#Contrast(100) 

def BandW(array):

    array = array.astype(np.float32)
    r  = array[:,:,0]
    g = array[:,:,1]
    b = array[:,:,2]

    greyscale = 0.2126 * r + 0.7152 * g + 0.0722 * b
    greyscale = np.clip(greyscale,0,255)
    return np.stack([greyscale, greyscale, greyscale], axis=2)

def process_image(input_path, brightness_value, contrast_value, apply_bw):

    image = img.open(input_path)
    array = np.array(image)


    if brightness_value != 0:
        array = Brightness(array,brightness_value)
    if contrast_value != 0:
        array = Contrast(array,contrast_value)
    if apply_bw:
        array = BandW(array)
    base_name = os.path.splitext(os.path.basename(input_path))[0] #finding the name of image that is to be saved folder 
    result_image = img.fromarray(array)
    output_path = f"processed_{base_name}.jpg" # saving the file with name proccessed_what_ever_that was_ectracted form spliting
    result_image.save(output_path)
    print(output_path) # this doesnt print on screen instead sends to stdout in c++ 

if __name__ == "__main__":
    if len(sys.argv) >= 2:
        process_image(
            sys.argv[1],
            int(sys.argv[2]) if len(sys.argv) > 2 else 0,
            int(sys.argv[3]) if len(sys.argv) > 3 else 0,
            bool(int(sys.argv[4])) if len(sys.argv) > 4 else False
        )