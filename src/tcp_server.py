#!/bin/python3

import socket
import time
import os
import sys
import numpy as np
import subprocess
import pathlib
import signal

import json
import posix_ipc
from ctypes import CDLL, c_void_p, c_uint8, c_double, c_int, POINTER, byref

HOST = "0.0.0.0"  # 全てのネットワークから受け付ける
PORT = 50000

libc = None
shm_addr = None

Direction = {
        "STOP"         : { "code" : 0, "msg" : "停止"   },
        "FORWARD"      : { "code" : 1, "msg" : "前進"   },
        "BACK"         : { "code" : 2, "msg" : "後進"   },
        "RIGHT_TURN"   : { "code" : 3, "msg" : "右旋回" },
        "LEFT_TURN"    : { "code" : 4, "msg" : "左旋回" },
        "RIGHT_FRONT"  : { "code" : 5, "msg" : "右前"   },
        "LEFT_FRONT"   : { "code" : 6, "msg" : "左前"   },
        "RIGHT_REAR"   : { "code" : 7, "msg" : "右後ろ" },
        "LEFT_REAR"    : { "code" : 8, "msg" : "左後ろ" },
}

direction_msg_map = {v["code"]: v["msg"] for v in Direction.values()}

'''
return r_duty, l_duty direction
       int int int
'''
def calc_duty_and_direction(x, y):
    """ジョイスティック座標 (x,y) から左右モータデューティと方向コードを返す"""
#    radius = calc_radius(x, y)
#    if radius == 0:
#        return 0, 0, Direction["STOP"]["code"]

    r_duty, l_duty = calc_duty(x, y)
    angle = calc_angle(x, y)
    direction = calc_direction(angle)

    # ここではデューティを整数(0-100)に丸める
    try:
        r_int = int(round(abs(r_duty)))
        l_int = int(round(abs(l_duty)))
    except Exception:
        r_int = 0
        l_int = 0

    return r_int, l_int, direction

def calc_radius(x, y):
    return np.sqrt(x ** 2 + y ** 2)
    
def calc_angle(x, y):
    angle = np.degrees(np.arctan2(y, x))

    angle = (angle + 360) % 360

    return angle

def calc_direction(angle):
    # angle: 0-360 (degree)
    if (0 <= angle < 10) or (350 <= angle < 360):
        return Direction["RIGHT_TURN"]["code"]
    if 10 <= angle < 75:
        return Direction["RIGHT_FRONT"]["code"]
    if 75 <= angle < 105:
        return Direction["FORWARD"]["code"]
    if 105 <= angle < 170:
        return Direction["LEFT_FRONT"]["code"]
    if 170 <= angle < 190:
        return Direction["LEFT_TURN"]["code"]
    if 190 <= angle < 255:
        return Direction["LEFT_REAR"]["code"]
    if 255 <= angle < 285:
        return Direction["BACK"]["code"]
    if 285 <= angle < 350:
        return Direction["RIGHT_REAR"]["code"]

    return Direction["STOP"]["code"]

def calc_duty(x, y):
#    radius = calc_radius(x, y)
#    angle = calc_angle(x, y)

#    forward = np.cos(np.radians(angle)) * radius
#    turn = np.sin(np.radians(angle)) * radius

#    r_duty = forward + turn
#    l_duty = forward - turn

    r_duty = y + x
    l_duty = y - x

    return r_duty, l_duty


def start_server():
    """サーバーソケットを立ち上げて接続待機"""
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # ポート再利用可
    server.bind((HOST, PORT))
    server.listen(1)
    print(f"[INFO] Server started on {HOST}:{PORT}")
    return server


def wait_for_client(server):
    """クライアント接続を待つ"""
    print("[INFO] Waiting for client connection...")
    while True:
        try:
            conn, addr = server.accept()
            print(f"[INFO] Client connected from: {addr}")
            return conn
        except OSError as e:
            print(f"[WARN] Accept failed: {e}")
            time.sleep(1)


def handle_client(conn):
    # ctypes のポインタ用変数（C 側からの読み取り用）
    r_duty_c = c_uint8()
    l_duty_c = c_uint8()
    direction_c = c_uint8()
    camera_angle_c = c_uint8()
    distance_sensor1 = c_double()
    distance_sensor2 = c_double()
    distance_sensor3 = c_double()
    distance_sensor4 = c_double()
    
    """接続中のクライアントと通信を行う（JSON対応版）"""
    conn.settimeout(2)
    last_send_check_time = 0.0
    while True:
        try:
            data = conn.recv(1024)
            if not data:
                print("[INFO] Client disconnected.")
                break

            try:
                # JSONをデコード
                msg = json.loads(data.decode())
                cmd_type = msg.get("cmd", "")
                payload = msg.get("data", {})
            except json.JSONDecodeError:
                print("[WARN] Invalid JSON received.")
                continue

            print(f"[RX] Command: {cmd_type}, Data: {payload}")

            # ==== コマンド別処理 ====
            if cmd_type == "CONTROL":
                timestamp = payload.get("timestamp", 0)
                joystick_x = payload.get("joystick_x", 0)
                joystick_y = payload.get("joystick_y", 0) 
                camera_angle = payload.get("camera_angle", 0) 

                # 計算
                r_duty, l_duty, direction = calc_duty_and_direction(joystick_x, joystick_y)
                robot_angle = calc_angle(joystick_x, joystick_y)

                libc.shm_write_motor_data(
                    shm_addr,
                    c_uint8(r_duty),
                    c_uint8(l_duty),
                    c_uint8(direction),
                    c_uint8(camera_angle),
                )

                direction_message = direction_msg_map.get(direction, "不明な動作")

                # 受け取ったデータの表示
                print(
                    f"  → 動作: {direction_message}, ジョイスティックX: {joystick_x},ジョイスティックy: {joystick_y}, ロボット角度: {robot_angle}, カメラ角度: {camera_angle}"
                )

                libc.shm_read(
                    shm_addr,
                    byref(r_duty_c),
                    byref(l_duty_c),
                    byref(direction_c),
                    byref(camera_angle_c),
                    byref(distance_sensor1),
                    byref(distance_sensor2),
                    byref(distance_sensor3),
                    byref(distance_sensor4),
                )

                print(distance_sensor1.value)
                print(distance_sensor2.value)
                print(distance_sensor3.value)
                print(distance_sensor4.value)

                # 返信内容
                response = {
                    "status": "OK",
                    "ack": "CONTROL_RECEIVED",
                    "direction": f"{direction}",
                    "timestamp": f"{timestamp}",
                    "測距1": f"{distance_sensor1.value}",
                    "測距2": f"{distance_sensor2.value}",
                    "測距3": f"{distance_sensor3.value}",
                    "測距4": f"{distance_sensor4.value}",
                }

            # シャットダウン
            elif cmd_type == "SHUTDOWN":
                #print("[INFO] Shutdown command received! Preparing to shutdown...")
                response = {"status": "OK", "ack": "SHUTTING_DOWN"}
                conn.send((json.dumps(response) + "\n").encode())
                conn.close()

                return      #goto main

            else:
                response = {
                    "status": "ERROR",
                    "message": f"Unknown command '{cmd_type}'",
                }

            if time.time() - last_send_check_time >= 0.02:
                # JSONとして返信する
                conn.send((json.dumps(response) + "\n").encode())
                last_send_check_time = time.time()
    
            time.sleep(0.1)

        except socket.timeout:
            continue
        except (ConnectionResetError, BrokenPipeError, OSError):
            print("[ERROR] Connection lost.")

            server_cleanup_and_shutdown()

            conn.close()


def server_cleanup_and_shutdown():
    try:
        print("[INFO] Cleaning up resources...")
        time.sleep(0.5)

        print("[INFO] System shutting down in 3 seconds...")
        time.sleep(1)

        #os.system("sudo shutdown -h now")
    except Exception as e:
        print(f"[ERROR] Failed to shutdown: {e}")
    finally:    
        sys.exit(0)


def ctypes_init():
    global libc

    libc = CDLL(f"{pathlib.Path.home()}/c_with_py/src/mylib/mylib.so")

    libc.c_py_shm_open.restype = c_void_p

    libc.shm_write_motor_data.argtypes = [
        c_void_p,
        c_uint8,
        c_uint8,
        c_uint8,
        c_uint8,
    ]

    libc.shm_write_motor_data.restype = None

    libc.shm_read.argtypes = [
        c_void_p,
        POINTER(c_uint8),
        POINTER(c_uint8),
        POINTER(c_uint8),
        POINTER(c_uint8),
        POINTER(c_double),
        POINTER(c_double),
        POINTER(c_double),
        POINTER(c_double),
    ]

    libc.shm_read.restype = None


def main():
    global libc
    global shm_addr
    global null_ptr

    null_ptr = c_void_p(0)

    p1 = subprocess.Popen(
        ["./main"],
        stdout = subprocess.DEVNULL,
        stderr = subprocess.DEVNULL,
        preexec_fn = os.setpgrp,
    )

    p2 = subprocess.Popen(
        [sys.executable, "./src/python_sender.py"],
        stdout = subprocess.DEVNULL,
        stderr = subprocess.DEVNULL,
        preexec_fn = os.setpgrp,
    )

    ctypes_init()

    time.sleep(0.5);

    shm_addr = libc.c_py_shm_open()
    if not shm_addr:
        print("shm Open Error")

        sys.exit(0)

    while True:
        try:
            server = start_server()
            conn = wait_for_client(server)
            handle_client(conn)

        except Exception as e:
            print(f"[FATAL] Server error: {e}")

        finally:
            try:
                #os.kill(int(p1.pid), signal.SIGINT)
                #os.kill(int(p2.pid), signal.SIGKILL)
                server.close()

            except:
                pass

            print("[INFO] Restarting server in 3 seconds...")
            time.sleep(3)


if __name__ == "__main__":
    main()
