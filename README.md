# Mission2

# README: Center Line Detection with X Coordinate and Stepper Motor Control

This project contains two major components:

1. **Center Line Detection and Positioning Using OpenCV**: 
   - A Python script that captures video from a webcam, detects vertical lines in real time, and calculates the distance of the most central line from the center of the frame.
   - Uses line stabilization to ensure the detected line is consistent across frames.

2. **Stepper Motor Control with Arduino**: 
   - An Arduino sketch that controls two stepper motors and moves them based on limit switches and degree inputs.
   - Designed for precise control of the stepper motors' movements.

## Requirements

### Python Dependencies:
- OpenCV (`pip install opencv-python`)
- NumPy (`pip install numpy`)

### Arduino Hardware:
- 2 Stepper motors
- Limit switches for both axes (X and Y)
- LED indicators
- Arduino board (configured with the correct pins for motor control)

## Python: Center Line Detection and Positioning

The Python script performs the following tasks:
- Initializes webcam input.
- Detects vertical lines using Hough Line Transform.
- Merges similar lines and finds the most central vertical line.
- Displays the X-coordinate of the central line relative to the center of the frame.
- Outputs the FPS (frames per second) on the screen.

### How It Works:
1. **Line Detection**: Uses Canny edge detection and Hough Line Transform to detect vertical lines in the webcam feed.
2. **Line Stabilization**: Merges similar lines over multiple frames to reduce flickering.
3. **Center Line Calculation**: Calculates the distance between the most central line and the center of the frame.
4. **Display**: The X-coordinate of the detected central line is displayed on the frame, along with the FPS.

### Usage:
1. Run the Python script:
   ```bash
   python line_detection.py
   ```
2. The video feed from the webcam will open. Press 'q' to exit.

## Arduino: Stepper Motor Control

The Arduino sketch controls two stepper motors, handling:
- Setting the home position using limit switches.
- Moving the motors to specific positions based on input degrees.
- LED indicators showing motor state (yellow for idle, red for setting home, green for active movement).

### How It Works:
1. **State 1 (Set Home)**: Moves the motors to the home position, controlled by limit switches.
2. **State 2 (Idle)**: Waits for degree input from the user.
3. **State 3 (Process)**: Moves the motors to the desired position based on the degree input.

### Pin Configuration:
- Stepper Motor 1: 
  - Enable: Pin 9
  - Direction: Pin 10
  - Step: Pin 11
- Stepper Motor 2: 
  - Enable: Pin 6
  - Direction: Pin 7
  - Step: Pin 8
- LED Indicators:
  - Yellow: Pin 3
  - Green: Pin 30
  - Red: Pin 5
- Limit Switches:
  - X-axis: Pin 12
  - Y-axis: Pin 13

### Usage:
1. Upload the Arduino sketch to the board.
2. Connect the stepper motors and limit switches as per the pin configuration.
3. Provide input for the degree of rotation via Serial communication.

## License
This project is licensed under the MIT License.

## Authors
- [Group1]
