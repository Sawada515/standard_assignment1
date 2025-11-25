#!/bin/python3

import cv2
import socket
import argparse

# --- 引数処理 ---
parser = argparse.ArgumentParser()
#parser.add_argument("--ip", type=str, default="192.168.76.38", help="送信先IP") #パソコンIP
parser.add_argument("--ip", type=str, default="192.168.250.14", help="送信先IP") #ラズパイIP
parser.add_argument("--port", type=int, default=55555, help="送信ポート")
parser.add_argument("--width", type=int, default=800, help="カメラ解像度幅")
parser.add_argument("--height", type=int, default=480, help="カメラ解像度高さ")
parser.add_argument("--fps", type=int, default=30, help="フレームレート")
args = parser.parse_args()

# --- UDPソケット ---
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
dest = (args.ip, args.port)

# --- カメラ ---
#cap = cv2.VideoCapture(0)
cap = cv2.VideoCapture(0,cv2.CAP_V4L2)
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))
cap.set(cv2.CAP_PROP_FRAME_WIDTH, args.width)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, args.height)
#cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
#cap.set(cv2.CAP_PROP_FRAME_HEIGHT,240)
cap.set(cv2.CAP_PROP_FPS, args.fps)
#cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)  # 0.25=manual, 0.75=auto
#cap.set(cv2.CAP_PROP_EXPOSURE, -6)         # 値はカメラ依存（試行）

print(cap.get(cv2.CAP_PROP_FRAME_WIDTH), cap.get(cv2.CAP_PROP_FRAME_HEIGHT),cap.get(cv2.CAP_PROP_FPS))

PACKET_SIZE = 1400

print(f"Streaming to {args.ip}:{args.port} at {args.width}x{args.height} {args.fps}FPS")

while True:
    ret, frame = cap.read()
    if not ret:
        continue

    # JPEGに圧縮
    ret, jpeg = cv2.imencode(".jpg", frame, [cv2.IMWRITE_JPEG_QUALITY,80])
    print(cap.get(cv2.CAP_PROP_FRAME_WIDTH), cap.get(cv2.CAP_PROP_FRAME_HEIGHT),cap.get(cv2.CAP_PROP_FPS))
    if not ret:
        continue
    data = jpeg.tobytes()

    # UDPで分割送信
    offset = 0
    while offset < len(data):
        chunk = data[offset:offset+PACKET_SIZE]
        sock.sendto(chunk, dest)
        offset += PACKET_SIZE
        
cap.release() 
