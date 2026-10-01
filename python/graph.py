import matplotlib
matplotlib.use("QtAgg")

import matplotlib.pyplot as plt
plt.ylim(-1000, 1500)

versions = ["Swamp Version 0", "Swamp Version 1", "Swamp Version 2"]
ratings = [400, 1100, 1400]

plt.plot(versions, ratings, marker='o')
plt.xlabel("Swamp Version")
plt.ylabel("Rating")
plt.yticks([400, 1100, 1400])
plt.title("Swamp Engine Rating by Version")

plt.grid(True)
plt.show()

while True:
    plt.show()

    