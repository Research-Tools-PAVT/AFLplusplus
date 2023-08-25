#!/usr/bin/env python3

import os
import argparse
import subprocess

import csv
from PIL import Image, ImageDraw, ImageFont

def csv_to_checkered_image(csv_file, output_image):
    data = []
    with open(csv_file, 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            data.append([int(cell) for cell in row])

    image_height = len(data)
    image_width = len(data[0])
    square_size = 20  # Size of each square in pixels
    row_number_width = 30  # Width reserved for row numbers

    img = Image.new('RGB', (row_number_width + image_width * square_size, image_height * square_size), (144, 238, 144))
    draw = ImageDraw.Draw(img)
    font = ImageFont.load_default()  # Use a default font for row numbers

    for y in range(image_height):
        for x in range(image_width):
            square_color = (255, 255, 255) if data[y][x] == 0 else (238, 14, 14)
            border_color = (128, 128, 128)
            for i in range(square_size):
                for j in range(square_size):
                    if i == 0 or i == square_size - 1 or j == 0 or j == square_size - 1:
                        img.putpixel((x * square_size + i + row_number_width, y * square_size + j), border_color)
                    else:
                        img.putpixel((x * square_size + i + row_number_width, y * square_size + j), square_color)

        # Print row numbers on the left
        draw.text((5, y * square_size + square_size // 2), str(y + 1), fill=(0, 0, 0), font=font)

    img.save(output_image)
    print(f"Opening image saved as {output_image}")
    # After generating the plot, open the PNG file using shotwell
    subprocess.Popen(["xdg-open", output_image], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, start_new_session=True)
    


def main():
    parser = argparse.ArgumentParser(description='Generate coverage plot')
    parser.add_argument('-i', '--input', default='data.csv', help='Input filepath')
    parser.add_argument('-o', '--output', default='coverage.png', help='Output filepath')
    
    args = parser.parse_args()
    
    input_filepath = args.input
    output_filepath = args.output
    
    if not os.path.exists(input_filepath):
        print(f"Error: Input file '{input_filepath}' not found!")
        return
    
    print(f"Input filepath: {input_filepath}")
    print(f"Output filepath: {output_filepath}")
    csv_to_checkered_image(input_filepath, output_filepath)    

    

if __name__ == '__main__':
    main()

