import pandas as pd
import matplotlib.pyplot as plt

def plot_motion_data(filename="output.csv", output_png="motion_plot.png"):
    # Load CSV data
    df = pd.read_csv(filename, delimiter=';')

    # Create the plot
    plt.figure(figsize=(10, 6))
    plt.plot(df["time(s)"], df["position(m)"], label="Position (m)", linestyle='dashed', color='black')
    plt.plot(df["time(s)"], df["velocity(m/s)"], label="Velocity (m/s)", color='green')
    plt.plot(df["time(s)"], df["acceleration(m*s^-2)"], label="Acceleration (m/s²)", color='red')
    plt.plot(df["time(s)"], df["jerk(m*s^-3)"], label="Jerk (m/s³)", linestyle='dotted', color='blue')

    # Labels and legend
    plt.xlabel("Time (s)")
    plt.ylabel("Value")
    plt.title("Motion Profile")
    plt.legend()
    plt.grid()

    # Save plot
    plt.savefig(output_png, dpi=300)
    plt.show()

# Run the function
plot_motion_data()
