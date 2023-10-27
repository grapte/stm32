# stm32 micro-controller projects

These projects and reports are developed with Keil uVision IDE, and ran on STM32F746G Discovery boards.  

Used devices:
- PmodKYPD
- STM32F746G-DISCO
- Parallax Standard Servo (#900-00005)
- Parallax Continuous Rotation Servo (#900-00008)
- Bluetooth UART client Board

## File Descriptions
```
.
├── Project     // final project complete with acompaning script
│   ├── blueuart.py     // python script for pc that sends image data to board
│   ├── main.c -> uVision Projects/uart/main.c
│   └── output.txt      // a recording of blueuart.py output
├── Reports
│   ├── Keypad lab report.pdf   // a lab report for programming a passowrd lock on a digilent-pmod device
│   ├── Lab report 1.pdf        // first lab going into detail describing the process of designing combinational systems
│   ├── Servo Motor Lab Report.pdf // 900-00005/900-00008 servos are actuated by using the arduino pinout
│   └── UART Lab Report.pdf     // lab report on the final project
├── readme.md
└── uVision Projects
    ├── ...                    // various small projects that are involved in the reports
```
