# Conveyor Belt Monitoring & Control System

An embedded firmware project simulating a conveyor belt system using *Bare Metal* programming, STM32 MCU and Proteus. It measures speed, counts objects, controls motor velocity, implements an emergency stop, and displays data on a 16×2 LCD.

---

## 🚀 Features

- **Speed Measurement**  
  - Utilizes Timer Input Capture to determine belt speed
- **Motor Speed Control**  
  - Reads potentiometer via ADC  
  - Adjusts motor speed using PWM
- **Object Detection**  
  - Detects passing items using a simulated IR sensor with polling/falling-edge detection
- **Emergency Stop**  
  - Handled using a customed external interrupt driver
- **Display**  
  - Custom 16×2 LCD driver to present real-time metrics

---

## 🛠️ Peripherals & Drivers

| Module                | Driver File           | API Functions                                 |
|-----------------------|------------------------|-----------------------------------------------|
| Timer Input Capture   | `TIM.h/.c`             | `TIC_Init()`, `TIC_GetPeriod()` |
| ADC                   | `ADC.h/.c`             | `ADC_Init(uint8_t ch)`, `ADC_ReadChannel(uint8_t ch)`   |
| PWM                   | `PWM.h/.c`             | `PWM_Init()`, `PWM_SetDutyCycle(uint8_t)`     |
| IR Sensor             | Pushbutton Simulated    | `DetectFallingEdge()` |
| LCD Display           | `LCD.h/.c`             | `LCD_Init()`, `LCD_SetCursor()`, `LCD_Print()` |
| Main Logic            | `main.c`               | Initializes modules, loops, handles emergency stop |

---

## 📸 Snapshots



## ⚙️ Setup & Usage

1. Clone the repository:  
   ```bash
   git clone https://github.com/Ahmed-Hajhamed/Converyor-Belt-Project.git

2. Open the `.DSN` file in Proteus and load `main.hex` into the simulated MCU.
3. Run simulation:

   * Adjust pot for motor speed
   * Send pulses for speed measurement
   * Simulate object passing with IR barrier
   * Trigger emergency stop via interrupt pin
4. See real-time data on the LCD display.

---

## 🎓 Project Insights

* **Driver modularization:** Each peripheral has its own *bare metal* API, enabling reusability.
* **Main controller logic:** Central `main()` coordinates data collection, display, and interrupt handling in a clean loop.
* **Scalability:** System architecture supports future enhancements (e.g., data logging, IoT integration).

---

## 📂 Documentation

In the `docs/` folder, you’ll find:

* Project report with architecture diagrams
* Detailed driver explanations
* Annotated simulation screenshots

---

## 📹 Demo Video

A short Proteus demo video is available in the `docs/` folder to showcase system functionality.

---

## 🤝 Credits & Contributors

* **Ahmed Hajhamed**
* **Mohamed Ahmed**
* **Zeyad Wael**
* **Mohamed Walid**
* **Mahmoud Mohamed**

---

## 📄 License

This project is open-source under the MIT License. See [LICENSE](LICENSE) for details.

---

### ⭐ Enjoyed this project?

Feel free to ⭐ the repo or adapt the code for your own embedded systems work!
