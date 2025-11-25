#!/usr/bin/env python3

import subprocess
import os
import signal
import pathlib

import posix_ipc
from ctypes import CDLL, c_void_p, c_uint8, c_double, POINTER, byref

libc = None
shm_addr = None

def ctypes_init():
    global libc

    libc = CDLL(f"{pathlib.Path.home()}/c_with_py/src/mylib/mylib.so")

    libc.c_py_shm_open.restype = c_void_p

    libc.shm_write.argtypes = [
        c_void_p,
        c_uint8,
        c_uint8,
        c_uint8,
        c_uint8,
        c_double,
        c_double,
        c_double,
        c_double,
    ]

    libc.shm_write.restype = None

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

    print("start", flush=True)

    ctypes_init()

    p1 = subprocess.Popen(
        ["./main"],
        #stdout = subprocess.DEVNULL,
        stdout = subprocess.DEVNULL,
        stderr = subprocess.DEVNULL,
        preexec_fn = os.setpgrp,
    )

    shm_addr = libc.c_py_shm_open()
    if not shm_addr:
        print("shm Open Error")

    r_duty = c_uint8()
    l_duty = c_uint8()
    direction = c_uint8()
    camera_angle = c_uint8()
    distance_sensor1 = c_double()
    distance_sensor2 = c_double()
    distance_sensor3 = c_double()
    distance_sensor4 = c_double()

    r_duty = c_uint8(50)
    l_duty = c_uint8(45)
    direction = c_uint8(1)
    camera_angle = c_uint8(40)
    distance_sensor1 = c_double(10.1)
    distance_sensor2 = c_double(22.2)
    distance_sensor3 = c_double(33.3)
    distance_sensor4 = c_double(44.4)

    libc.shm_write(
        shm_addr,
        r_duty.value,
        l_duty.value,
        direction.value,
        camera_angle.value,
        distance_sensor1.value,
        distance_sensor2.value,
        distance_sensor3.value,
        distance_sensor4.value
    )

    libc.shm_read(
        shm_addr,
        byref(r_duty),
        byref(l_duty),
        byref(direction),
        byref(camera_angle),
        byref(distance_sensor1),
        byref(distance_sensor2),
        byref(distance_sensor3),
        byref(distance_sensor4),
    )

    print(r_duty.value);
    print(l_duty.value);
    print(direction.value);
    print(camera_angle.value);
    print(distance_sensor1.value);
    print(distance_sensor2.value);
    print(distance_sensor3.value);
    print(distance_sensor4.value);

    r_duty = c_uint8(10)
    l_duty = c_uint8(90)
    direction = c_uint8(8)
    camera_angle = c_uint8(180)
    distance_sensor1 = c_double(100)
    distance_sensor2 = c_double(200)
    distance_sensor3 = c_double(300)
    distance_sensor4 = c_double(400)

    libc.shm_write(
        shm_addr,
        r_duty.value,
        l_duty.value,
        direction.value,
        camera_angle.value,
        distance_sensor1.value,
        distance_sensor2.value,
        distance_sensor3.value,
        distance_sensor4.value
    )

    libc.shm_read(
        shm_addr,
        byref(r_duty),
        byref(l_duty),
        byref(direction),
        byref(camera_angle),
        byref(distance_sensor1),
        byref(distance_sensor2),
        byref(distance_sensor3),
        byref(distance_sensor4),
    )

    print(r_duty.value);
    print(l_duty.value);
    print(direction.value);
    print(camera_angle.value);
    print(distance_sensor1.value);
    print(distance_sensor2.value);
    print(distance_sensor3.value);
    print(distance_sensor4.value);

    os.kill(int(p1.pid), signal.SIGTERM)

if __name__ == "__main__":
    main()
