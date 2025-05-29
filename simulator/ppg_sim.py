import numpy as np
from scipy.signal import butter, filtfilt, find_peaks
import matplotlib.pyplot as plt

def generate_ppg_waveform(duration=10.0, fs=100.0, hr=60):
    t = np.linspace(0, duration, int(duration * fs))
    pulse = 0.6 * np.sin(2*np.pi*(hr/60)*t)
    pulse += 0.2 * np.sin(4*np.pi*(hr/60)*t) + 0.1 * np.sin(6*np.pi*(hr/60)*t)
    pulse += 0.05 * np.random.randn(len(t))
    return t, pulse

def bandpass_filter(data, fs=100.0, lowcut=0.5, highcut=5.0, order=3):
    nyq = 0.5 * fs
    b, a = butter(order, [lowcut/nyq, highcut/nyq], btype='band')
    return filtfilt(b, a, data)

def detect_peaks(ppg, fs=100.0, max_hr=180):
    distance = int(fs * 60 / max_hr)
    peaks, _ = find_peaks(ppg, distance=distance, height=np.std(ppg))
    rr = np.diff(peaks) / fs
    hr = 60 / np.mean(rr) if len(rr)>0 else 0
    return peaks, hr

if __name__ == "__main__":
    t, raw = generate_ppg_waveform(hr=75)
    filt = bandpass_filter(raw)
    peaks, hr = detect_peaks(filt)
    print(f"Estimated HR: {hr:.1f} BPM")
    plt.plot(t, filt, label="Filtered")
    plt.plot(t[peaks], filt[peaks], 'ro', label="Peaks")
    plt.legend(); plt.show()
