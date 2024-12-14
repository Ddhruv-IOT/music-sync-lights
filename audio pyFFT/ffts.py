# import serial
# import numpy as np
# import matplotlib.pyplot as plt

# # Initialize serial
# ser = serial.Serial('COM8', 115200)
# buffer_size = 512  # Number of samples for FFT
# data = np.zeros(buffer_size)

# while True:
#     try:
#         for i in range(buffer_size):
#             data[i] = int(ser.readline().decode().strip())  # Read raw audio data

#         # Apply FFT to the data
#         fft_data = np.fft.fft(data)
#         freqs = np.fft.fftfreq(buffer_size, d=1/1000)  # Assuming 1 kHz sampling rate

#         # Plot the magnitude spectrum
#         plt.clf()
#         plt.plot(freqs[:buffer_size//2], np.abs(fft_data)[:buffer_size//2])
#         plt.xlabel("Frequency (Hz)")
#         plt.ylabel("Magnitude")
#         plt.pause(0.05)  # Refresh plot
#     except KeyboardInterrupt:
#         print("Exiting...")
#         break


import serial
import numpy as np
import matplotlib.pyplot as plt

# Initialize serial communication (update with your COM port)
ser = serial.Serial('COM8', 115200, timeout=1)

# FFT parameters
buffer_size = 256  # Number of samples for FFT
sampling_rate = 1000  # 1 kHz sampling rate (adjust if needed)
frequencies = [60, 120, 250, 500, 1000, 2000, 4000]  # Standard audio bands

# Set up plot
fig, ax = plt.subplots()
bars = ax.bar(frequencies, [0] * len(frequencies), width=50, color='b')
ax.set_ylim(0, 3000)  # Adjust Y-axis range based on your input signal
ax.set_xlabel('Frequency (Hz)')
ax.set_ylabel('Magnitude')
ax.set_title('Real-Time Audio Frequency Spectrum')

def update_bars(data):
    fft_data = np.fft.fft(data)
    magnitudes = np.abs(fft_data)[:len(data) // 2]
    freqs = np.fft.fftfreq(len(data), d=1 / sampling_rate)

    # Extract magnitudes for specific frequency bands
    band_magnitudes = []
    for f in frequencies:
        idx = np.argmin(np.abs(freqs - f))
        band_magnitudes.append(magnitudes[idx])

    # Update bar heights
    for bar, mag in zip(bars, band_magnitudes):
        bar.set_height(mag)

while True:
    try:
        # Collect data for FFT
        data = [int(ser.readline().decode().strip()) for _ in range(buffer_size)]

        # Update the plot with new data
        update_bars(data)
        plt.pause(0.05)  # Pause to refresh the plot

    except KeyboardInterrupt:
        print("Exiting...")
        break
    except Exception as e:
        print(f"Error: {e}")
        continue
