#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import os, cv2, time, numpy as np, requests, threading, tkinter as tk
from collections import Counter
from datetime import datetime
import torch
from torchvision import transforms
from torchvision.models import vit_b_16
from torchvision.models.vit import ViT_B_16_Weights
import torch.nn as nn
import torchvision.models as models
from PIL import Image


# ───────── DIRECTOARE & CONFIG ─────────
RECEIVED_FOLDER  = "received"
PROCESSED_FOLDER = "processed"
esp32_ip = "192.168.137.10"          # IP implicit ESP32-WROOM

# ───────── HELPER URL ─────────
def url_led():   return f"http://{esp32_ip}/led"
def url_start(): return f"http://{esp32_ip}/start"

# asigură foldere
os.makedirs(PROCESSED_FOLDER, exist_ok=True)

# ───────── TRIMITERE COMENZI ESP32 ─────────
def send_led_command(color):
    try:
        requests.get(f"{url_led()}?color={color.lower()}", timeout=2)
        print(f"📡 LED trimis: {color}")
    except Exception as e:
        print(f"❌ LED error: {e}")

def send_start_command(go: int):
    try:
        requests.get(f"{url_start()}?go={go}", timeout=2)
        print(f"⚙️  START cmd {go}")
    except Exception as e:
        print(f"❌ START error: {e}")

# ───────── PRELUCRĂRI IMAGINI ─────────
def process_all_images():
    for fname in sorted(os.listdir(RECEIVED_FOLDER)):
        path = os.path.join(RECEIVED_FOLDER, fname)
        img  = Image.open(path).convert("RGB")

        if img is None:
            print(f"⚠️ Fișier invalid: {fname}"); os.remove(path); continue
        gray_img = transform_detect(img)
        color_image = transform_color(img)
        color = "Red"
        with torch.no_grad():
            output = model(gray_img.unsqueeze(0))
            probs = torch.softmax(output, dim=1)
            predicted_class = torch.argmax(probs, dim=1).item()
        if predicted_class == 1:
            color = "Red"
        else:
            with torch.no_grad():
                output = color_model(color_image.unsqueeze(0))
                probs = torch.softmax(output, dim=1)
                predicted_class = torch.argmax(probs, dim=1).item()
            color = ['Yellow', 'Blue', 'Green'][predicted_class]
        ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        new_name = f"esp32_{color.lower()}_{ts}.jpg"
        cv2.imwrite(os.path.join(PROCESSED_FOLDER, new_name), img)
        os.remove(path)
        print(f"🎯 {color} → {new_name}")
        if color in ["Red", "Green", "Blue", "Yellow"]:
            send_led_command(color)

# ───────── GUI ─────────
def gui_loop():
    global esp32_ip
    cycle_active = tk.BooleanVar(value=False)

    def toggle_cycle():
        go = 1 if not cycle_active.get() else 0
        send_start_command(go)
        cycle_active.set(not cycle_active.get())
        master_btn.configure(
            bg="#FFD700" if cycle_active.get() else "#2196F3",
            activebackground="#FFD700" if cycle_active.get() else "#2196F3",
            text="OPREȘTE" if cycle_active.get() else "PORNEȘTE")

    def on_led_click(color):
        send_led_command(color)
        status_label.config(text=f"LED {color} aprins", fg="green")

    root = tk.Tk(); root.title("SortoBOT – Control")
    frame = tk.Frame(root); frame.pack(padx=20, pady=15)

    # ── MASTER BUTTON ──
    master_btn = tk.Button(frame, text="PORNEȘTE",
                           bg="#2196F3", fg="white",
                           font=("Arial", 14, "bold"),
                           width=18, pady=5,
                           command=toggle_cycle)
    master_btn.pack(pady=(0,10))

    # ── LED buttons ──
    tk.Label(frame, text="Comandă manuală LED:", font=("Arial", 13)).pack()
    colors = [("Red","#ff4d4d"),("Green","#4CAF50"),("Blue","#2196F3"),("Yellow","#FFD700")]
    for name,hex_c in colors:
        tk.Button(frame, text=name, bg=hex_c,
                  fg="white" if name!="Yellow" else "black",
                  font=("Arial",12), width=15,
                  command=lambda n=name:on_led_click(n)).pack(pady=3)

    status_label = tk.Label(frame, text="", font=("Arial", 12)); status_label.pack(pady=8)

    # ── schimbare IP ──
    ip_frame = tk.Frame(root); ip_frame.pack(pady=5)
    ip_var = tk.StringVar(value=f"IP ESP32: {esp32_ip}")
    tk.Label(ip_frame, textvariable=ip_var, font=("Arial", 12)).pack(side=tk.LEFT, padx=8)

    def change_ip_popup():
        popup = tk.Toplevel(); popup.title("Setează IP"); popup.geometry("250x120")
        tk.Label(popup, text="IP nou:", font=("Arial",11)).pack(pady=5)
        ip_entry = tk.Entry(popup, font=("Arial",12)); ip_entry.insert(0, esp32_ip); ip_entry.pack()
        def confirm():
            nonlocal ip_entry
            esp32_ip = ip_entry.get().strip()
            ip_var.set(f"IP ESP32: {esp32_ip}")
            popup.destroy()
        tk.Button(popup, text="CONFIRM", command=confirm).pack(pady=5)
    tk.Button(ip_frame, text="SCHIMBĂ", command=change_ip_popup).pack(side=tk.LEFT)

    root.mainloop()

# ───────── MAIN LOOP ─────────
if __name__ == "__main__":
    print("🧠 Procesator + GUI activ…")
    threading.Thread(target=gui_loop, daemon=True).start()
    transform_color = transforms.Compose([
            transforms.Resize((224,224)),
            transforms.ToTensor(),
            transforms.Normalize( mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])
    transform_detect = transforms.Compose([
            transforms.Grayscale(num_output_channels=3)
            transforms.Resize((224,224)),
            transforms.ToTensor(),
            transforms.Normalize( mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])

    weights = ViT_B_16_Weights.DEFAULT
    model = vit_b_16(weights=weights)

    in_features = model.heads[0].in_features
    model.heads = nn.Sequential(nn.Linear(in_features, 2))

    model.load_state_dict(torch.load("vit_model.pth"), map_location=torch.device('cpu'))
    model.eval()

    color_model = models.resnet18(pretrained=True) 

    color_model.fc = nn.Linear(color_model.fc.in_features, 3)

    color_model.load_state_dict(torch.load("resnet_color_model.pth"), map_location=torch.device('cpu'))
    color_model.eval()

    while True:
        process_all_images()
        time.sleep(1)

