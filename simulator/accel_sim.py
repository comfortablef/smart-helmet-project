import numpy as np

def generate_accel_sequence(duration=5.0, fs=100.0):
    t = np.linspace(0, duration, int(duration*fs))
    # 정상 주행: 0g
    accel = np.zeros_like(t)
    # 2초 지점에 급감속 이벤트 삽입 (–2g)
    idx = int(2*fs)
    accel[idx:idx+5] = -19.6  # –2g in m/s²
    return t, accel

def detect_collision(accel, threshold=-15.0):
    events = np.where(accel < threshold)[0]
    return len(events)>0

if __name__=="__main__":
    t, seq = generate_accel_sequence()
    coll = detect_collision(seq)
    print("Collision detected!" if coll else "No collision")
