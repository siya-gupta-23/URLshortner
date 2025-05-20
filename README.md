# SMART URL SHORTENER

A simple and smart tool to convert long URLs into short, shareable links using a C++ backend and a web frontend.

## 🚀 Features
- Generate short codes for long URLs
- Redirection support for short URLs
- CORS-enabled backend with JSON handling
- Responsive frontend using HTML, CSS, JS
- SweetAlert2 popups and loading spinner

## 🔧 Tech Stack
- C++, cpp-httplib, nlohmann/json
- HTML, CSS, JavaScript

## ▶️ Run Locally
1. Compile: `g++ -I./cpp-httplib -I./json -o shortenurl main.cpp -lws2_32`
2. Run: `./shortenurl`
3. Open `index.html` in your browser


### 🧩 Prerequisites:
- C++ compiler (TDM-GCC, MinGW, or MSVC)
- Git (optional for cloning)
- Browser (for frontend)

###  Backend (C++):

1. Download `cpp-httplib` and `nlohmann/json` single-header files into respective folders.
2. Compile using:
   ```bash
  g++ -I./cpp-httplib -I./json -o shortenurl main.cpp -lws2_32
  ./shortenurl

