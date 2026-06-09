import cv2
import numpy as np
import face_recognition
import os
import threading
import socket
import time
from picamera2 import Picamera2
from libcamera import Transform
import subprocess
ESP_HOSTNAME = "esp8266-Drone.local"
ESP_PORT = 4210
PC_UDP_PORT = 2020
face_tracker = {}
unknown_tracker = {}
REQUIRED_TIME = 2
shared_variable = "System Booting..."
lock = threading.Lock()
FONT = cv2.FONT_HERSHEY_DUPLEX  # Cleaner, more modern font
FONT_SCALE = 0.7
FONT_THICKNESS = 2
picam2 = Picamera2(0)
picam2.preview_configuration.main.size = (640, 480)
picam2.preview_configuration.main.format = "RGB888"
picam2.preview_configuration.transform = Transform(hflip=1, vflip=1)
picam2.preview_configuration.align()
picam2.configure("preview")
picam2.start()
path = 'res'
classNames = []
images = []
if os.path.exists(path):
    for cl in os.listdir(path):
        curImg = cv2.imread(f'{path}/{cl}')
        if curImg is not None:
            images.append(curImg)
            classNames.append(os.path.splitext(cl)[0])
def findEncodings(images):
    encodeList = []
    for img in images:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        enc = face_recognition.face_encodings(img)
        if enc: encodeList.append(enc[0])
    return encodeList
encodeListKnown = findEncodings(images)
print(f'System Ready. Loaded {len(classNames)} faces.')
def send_udp(command):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(command.encode(), (ESP_HOSTNAME, ESP_PORT))
        sock.close()
    except: pass
def receive_udp():
    global shared_variable
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        sock.bind(("", PC_UDP_PORT))
        while True:
            data, _ = sock.recvfrom(1024)
            with lock: shared_variable = data.decode('utf-8')
    except: pass
threading.Thread(target=receive_udp, daemon=True).start()
while True:
    raw_frame = picam2.capture_array()
    display_img = cv2.cvtColor(raw_frame, cv2.COLOR_RGB2BGR) 
    imgS = cv2.resize(raw_frame, (0, 0), None, 0.25, 0.25)
    facesCurFrame = face_recognition.face_locations(imgS)
    encodesCurFrame = face_recognition.face_encodings(imgS, facesCurFrame)
    current_known_in_frame = []
    unknown_detected_this_frame = False
    for i, (encodeFace, faceLoc) in enumerate(zip(encodesCurFrame, facesCurFrame)):
        matches = face_recognition.compare_faces(encodeListKnown, encodeFace)
        faceDis = face_recognition.face_distance(encodeListKnown, encodeFace)      
        y1, x2, y2, x1 = [v * 4 for v in faceLoc]
        if len(faceDis) > 0 and matches[np.argmin(faceDis)] and faceDis[np.argmin(faceDis)] < 0.50:
            name = classNames[np.argmin(faceDis)].upper()
            current_known_in_frame.append(name)       
            if name not in face_tracker: face_tracker[name] = time.time()
            elapsed = time.time() - face_tracker[name]
            if elapsed < REQUIRED_TIME:
                msg = f"KEEP STILL... {int(REQUIRED_TIME - elapsed + 1)}S"
                clr = (0, 215, 255) # Gold/Yellow
            else:
                msg = "ACCESS GRANTED"
                clr = (0, 255, 0) # Green
                send_udp("open")
            cv2.rectangle(display_img, (x1, y1), (x2, y2), clr, 2)
            cv2.putText(display_img, name, (x1, y1 - 35), FONT, FONT_SCALE, clr, FONT_THICKNESS)
            cv2.putText(display_img, msg, (x1, y1 - 10), FONT, FONT_SCALE - 0.2, clr, 1)       
        else:
            unknown_detected_this_frame = True
            if "unknown" not in unknown_tracker: unknown_tracker["unknown"] = time.time()
            elapsed_u = time.time() - unknown_tracker["unknown"]           
            if elapsed_u < REQUIRED_TIME:
                msg = f"UNKNOWN: SCANNING {int(REQUIRED_TIME - elapsed_u + 1)}S"
                clr = (0, 140, 255) # Orange
            else:
                msg = "ALERT: UNKNOWN DETECTED"
                clr = (0, 0, 255) # Red
                send_udp("alert")
                subprocess.Popen(['aplay', '/home/rajeev/Project/Facerecognition/alert/alert.wav'])
            cv2.rectangle(display_img, (x1, y1), (x2, y2), clr, 2)
            cv2.putText(display_img, msg, (x1, y1 - 10), FONT, FONT_SCALE - 0.1, clr, FONT_THICKNESS)
    face_tracker = {n: t for n, t in face_tracker.items() if n in current_known_in_frame}
    if not unknown_detected_this_frame: unknown_tracker.clear()
    with lock: status = shared_variable
    overlay = display_img.copy()
    cv2.rectangle(overlay, (0, 440), (640, 480), (0, 0, 0), -1)
    cv2.addWeighted(overlay, 0.5, display_img, 0.5, 0, display_img) 
    cv2.putText(display_img, f"DEVICE STATUS: {status}", (15, 465), FONT, 0.6, (255, 255, 255), 1)
    cv2.imshow('Security Console - Drone Hub', display_img)
    if cv2.waitKey(1) & 0xFF == ord('q'): break
picam2.stop()
cv2.destroyAllWindows()

