# StreetConnect Protocol 🚨📍
> **Intelligent Surplus Distribution & Sheltering Network**

StreetConnect Protocol is a zero-digital-literacy IoT solution designed to bridge the gap between homeless individuals needing immediate assistance and local donors/NGOs. Using an ESP32 edge device with a street button, real-time geolocation alerts are dispatched to nearby aid providers.
![Street connect hardware](doc/project%20hardware.png)

## 🛠️ Project Features
* **Zero-Literacy Hardware Node**: Single push-button operation, piezo buzzer audio feedback, and visual LED status indicators (Blue: Processing, Green: Dispatched, Red: Standby).
* **Real-time Geofenced Alerting**: Proximity-matching algorithm built on Cloud Firestore ensures alerts target donors within immediate geographical radii[cite: 1, 2].
* **Interactive Donor Dashboard**: Web portal featuring live feeds, status management (Pending, In-Transit, Delivered), and analytics powered by Chart.js.

## 📂 Repository Structure
```text
StreetConnect/
├── firmware/
│   └── main.ino          # ESP32 C++ microcontroller & Firebase sync code
├── frontend/
│   └── index.html        # Web dashboard application with Firebase integration
├── docs/
│   └── presentation.pptx # Project slides and documentation
├── LICENSE               # MIT License
└── README.md             # Project overview & documentation
