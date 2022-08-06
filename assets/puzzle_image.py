#!/usr/bin/env python3
""" puzzle_image """

from PIL import Image

# --------------------------------------------------------------------------------

board = [
     "1.png",  "2.png",  "3.png",  "4.png",
     "5.png",  "6.png",  "7.png",  "8.png",
     "9.png", "10.png", "11.png", "12.png",
    "13.png", "14.png", "15.png", "0.png"
]

image = Image.new("RGB", size=(240, 240))

for i, boardName in enumerate(board):
    x = i % 4
    y = i // 4
    tile = Image.open(boardName)
    image.paste(tile, (x * 60, y * 60))

image.save("puzzle.png")
