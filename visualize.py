import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

df = pd.read_csv("detected_particle.csv")
detectors = df["Detector"].unique()

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

colors = {
    "detector 1": "red",
    "detector 2": "green",
    "detector 3": "blue"
}

for det in detectors:
    sub = df[df["Detector"] == det]
    ax.scatter(sub["PosX"], sub["PosY"], sub["PosZ"], label=det, color=colors.get(det, "black"))

ax.set_title("Detected Particles by Detector")
ax.set_xlabel("X Position")
ax.set_ylabel("Y Position")
ax.set_zlabel("Z Position")
ax.legend()
plt.show()
