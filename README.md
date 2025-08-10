Secure access control using GSM-based dynamic OTP verification and real-time validation. This project implements a microcontroller-based time-limited authentication system where a password-protected interface sends a dynamically generated OTP via GSM, which must be entered within a limited timeframe to grant access. 🧠 Project Objective To design and develop an OTP-based time-restricted access control system using GSM technology with LPC2148 microcontroller, ensuring enhanced security for restricted access environments.

🧩 System Overview flowchart TD A[User Enters Static Password] --> B{Is Password Correct?} B -- Yes --> C[Generate OTP via RTC] C --> D[Send OTP to Registered Mobile via GSM] D --> E[User Enters OTP] E --> F{Is OTP Correct & Timely?} F -- Yes --> G[Access Granted: Activate Motor/Door] F -- No --> H[Access Denied: Reset to Initial State] B -- No --> I[Access Denied: Retry]

🛠️ Hardware Requirements

🧠 LPC2148 Microcontroller

📡 GSM Module (M660A)

🖥️ 16x2 LCD Display

🎛️ 4x4 Matrix Keypad

🔘 Switch

💾 EEPROM (AT24C256)

💡 LED / Bulb / DC Motor with L293D Driver IC

💻 Software Requirements Embedded C Programming

KEIL µVision Compiler

Flash Magic (for flashing hex file)

🔄 Core Workflow System prompts for a secure static password via keypad.

If correct, the system:

Generates an OTP using RTC.

Sends OTP to a registered mobile via GSM module.

User must enter OTP within limited time.

On successful verification:

Access is granted (e.g., motor turns ON).

If OTP is invalid or delayed:

Access is denied, and system resets.

💬 GSM AT Commands Used txt Copy code AT // Check GSM connection ATE0 // Turn off echo AT+CMGF=1 // Set text mode AT+CMGS="MOBILE_NO" // Send message Use CTRL+Z after typing the message to send OTP via HyperTerminal during testing.

Password & OTP Flow Static password is stored in EEPROM.

OTP is generated after static password validation.

OTP expiry is checked using RTC module.

All incorrect attempts or timeout reset the system.

🧪 Testing Strategy Each module should be tested individually:

✅ LCD display (char, string, integer)

✅ Keypad input display

✅ EEPROM read/write

✅ UART transmission with interrupt

✅ External interrupt with LED

✅ GSM module AT command response

✅ RTC time readout

✅ Final integration with projectmain.c
